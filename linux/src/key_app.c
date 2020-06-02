/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License,  Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,  software
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,  either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/err.h>
#include <linux/input.h>
#include <asm/unistd.h>
#include <asm/unistd.h>
#include <machine/asm.h>


#define _DEBUG_ 2
#if (_DEBUG_==1)
    #define debug(...) printf(__VA_ARGS__)
	#define dbg_info(...)  printf(__VA_ARGS__)
#elif (_DEBUG_ ==2)
#define debug(...) printf(__VA_ARGS__)
#define dbg_info(...)  //printf(__VA_ARGS__)
#else
    #define debug(...) LOGD(__VA_ARGS__)
    #define dbg_info(...) LOGD(__VA_ARGS__)
#endif

#define MAX_DEVICES                (16)
#define EVIOCSSUSPENDBLOCK         _IOW('E', 0x91, int)

#define KEY_PRESS_TIME_STEP        (5)
#define KEY_CLICK_GAP              (200000)
#define KEY_LONG_PRESS_TIME       (3000000) //(3000000)

#define KEY_RELEASE    (0)
#define KEY_PRESS      (1)
#define KEY_REPEAT     (2)

#define MAX_KEYS       (4)

#define POLL_TIMEOUT (0)
#define POLL_SUCCESS (1)

enum {
	SINGEL_KEY_PRESS = 0,
	SINGEL_KEY_RELEASE,
	LONG_KEY_PRESS,
};

struct key_info
{
    int code;
    int status;
    int cnt;
};

struct keys_info
{
    struct key_info key[MAX_KEYS];
    struct timeval start_time;
    struct timeval end_time;
	int press_cnt;
	int keys_status;
};

struct keys_info keys;
static struct pollfd ev_fds[MAX_DEVICES];
static unsigned ev_count = 0;
int lock_cnt =0;

void keys_debug_info(void) {
	int i;
	for(i=0; i<MAX_KEYS; i++) {
		debug("code:%d,cnt:%d,status:%d\n", keys.key[i].code, keys.key[i].cnt, keys.key[i].status);
	}
	debug("press_cnt:%d\n", keys.press_cnt);
}

int ev_init(void) {
    DIR *dir;
    struct dirent *de;
    int fd;

    dir = opendir("/dev/input");
    if (dir != 0) {
        while ((de = readdir(dir))) {
            if (strncmp(de->d_name , "event", 5)) {
                continue;
            }
            fd = openat(dirfd(dir), de->d_name, O_RDONLY);
            if (fd < 0) {
                continue;
            }
            ev_fds[ev_count].fd = fd;
            ev_fds[ev_count].events = POLLIN;
            ioctl(fd, EVIOCSSUSPENDBLOCK, 1);
            ev_count++;
            if (ev_count == MAX_DEVICES) {
                break;
            }
        }
        closedir(dir);
    }
    return 0;
}

void ev_exit(void) {
    while (ev_count > 0) {
        close(ev_fds[--ev_count].fd);
    }
}

/* wait: 0 dont wait; -1 wait forever; >0 wait ms */
int ev_get(struct input_event *ev, int wait_ms) {
    int r;
    unsigned n;

    do {
        r = poll(ev_fds, ev_count, wait_ms);
        if (r > 0) {
            for (n = 0; n < ev_count; n++) {
                if (ev_fds[n].revents & POLLIN) {
                    r = read(ev_fds[n].fd, ev, sizeof(*ev));
                    if (r == sizeof(*ev)) {
                        dbg_info("ev=[%d][%d][%d]\n", ev->type, ev->code, ev->value);
                        return 0;
                    }
                    else {
                        debug("input_event size=[%d]\n", r);
                    }
                }
            }
        }
    } while (wait_ms == -1);
    return -1;
}

int get_key_info(int index)
{
    int i;
    int cnt = 0;

    for (i=0; i<MAX_KEYS; i++) {
        if(keys.key[i].cnt != 0) {
            if(index == cnt){
                break;
            }
            cnt++;
        }
    }

    return i;
}

int get_key_press_num(void)
{
    int i;
    int cnt = 0;

    for (i=0; i<MAX_KEYS; i++) {
        if(KEY_RELEASE != keys.key[i].status) {
            cnt++;
        }
    }

    return cnt;
}

long long timeval_diff(struct timeval big,  struct timeval small) {
    return (long long)(big.tv_sec-small.tv_sec)*1000000 + big.tv_usec - small.tv_usec;
}

void keys_init(void) {
    int i;
    for (i=0; i<MAX_KEYS; i++) {
        keys.key[i].code = 0;
        keys.key[i].status = KEY_RELEASE;
        keys.key[i].cnt = 0;
    }
	keys.press_cnt = 0;
}

void keys_buffer_overflow_process(void)
{
    debug("Key buffer overflow, reset key !!!\n");
    keys_init();
    lock_cnt =0;
	//my_suspend("key_lock_manager");
}


void single_key_handle(int code, int is_press, int cnt) {
	char *str[3] = {"single", "double", "triple"};
    if(cnt <= 3 && cnt > 0) {
        debug("%s key:[%d] %s===yantt\n", str[cnt-1], code, is_press ? "press":"release");
    }
}

void multiple_key_handler(int code, int is_press, int cnt) {
	char *str[3] = {"two", "three", "four"};
	int index;
	if(cnt <= 4 && cnt > 1) {
		debug("%s keys:", str[cnt-2]);
		for(; cnt > 0; cnt--) {
			index = get_key_info(cnt-1);
			if(index < MAX_KEYS) {
				debug("%d ", keys.key[index].code);
			}
		}
		debug("press\n");
	}
	//keys_debug_info();
}

void long_key_handle(int code, int is_press, int cnt) {
    int i;
    int index;
    struct key_info key_info[MAX_KEYS] = {0};

	keys.keys_status = LONG_KEY_PRESS;
    for(i=0; i<cnt; i++) {
        index = get_key_info(i);
        key_info[i].code = keys.key[index].code;
        key_info[i].cnt = keys.key[index].cnt;
        key_info[i].status = keys.key[index].status;
    }

    if( (1 == cnt) && (key_info[0].cnt != 1 )) {
        single_key_handle(key_info[0].code, key_info[0].status, key_info[0].cnt);
    } else if (!cnt){
		debug("long_key_handle: cnt:%d\n",cnt);
	} else {
        debug("long key: cnt:%d ===yantt", cnt);
        for(i=0; i<cnt; i++) {
            debug("[%d]", key_info[i].code);
        }
        debug("%s\n", is_press ? "press":"release");
    }
}

void single_long_key_release(int code, int is_press, int cnt) {
    debug("long key:%d release\n", code);
}

void key_release_modify(int code, int is_press, int cnt)
{
	//debug("key release modify\n");
	//debug("release [%d] from %d keys\n", code, cnt);
	if(1 == cnt) {
		keys_init();
	}
}

void check_poll_timeout(long long tm) {
    int i;
    int cnt = 0;
    int index;

    //get how many keys pressed
    for (i=0; i<MAX_KEYS; i++) {
        if(keys.key[i].cnt != 0) {
            cnt++;
        }
    }

    //no key was pressed
    if(!cnt) {
        return;
    }
	//debug("check_poll_timeout\n");
    //long keys was pressed
    if(tm >= KEY_LONG_PRESS_TIME) {
        cnt = 0;
        for (i=0; i<MAX_KEYS; i++) {
            if(keys.key[i].cnt != 0 && keys.key[i].status) {
                cnt++;
            }
        }
        long_key_handle(0, 1, cnt);
        for (i=0; i<MAX_KEYS; i++) {
            keys.key[i].cnt = 0;
        }
    } else if(tm >= KEY_CLICK_GAP) {
		//keys_debug_info();
		//debug("check_poll_timeout:cnt:%d\n",cnt);
        if (1 == cnt) {
            index = get_key_info(0);
            if(index >= MAX_KEYS) {
                dbg_info("check error\n");
                return;
            } else if( KEY_RELEASE == keys.key[index].status) {
                //single key released
                single_key_handle(keys.key[index].code, keys.key[index].status, keys.key[index].cnt);
                keys.key[index].cnt = 0;
                keys.key[index].code = 0;
            }
         }
    }
}

void check_key_status(struct input_event *ev, long long tm) {
    int i,j;

    /* Discard events that shouldn't happen */
    int count = 0;
    int space = 0;
    for(count=0; count<MAX_KEYS; count++) {
        if (keys.key[count].code == ev->code ){
            if (keys.key[count].status == ev->value) {
                debug("Discare repeat event[%d][%d][%d]\n", ev->type, ev->code, ev->value);
                return;
            }
        }
        else {
            space++;
        }
    }
	
    if (MAX_KEYS == space && KEY_RELEASE == ev->value) {
        debug("No key pressed, discare release event[%d][%d][%d]\n", ev->type, ev->code, ev->value);
        return;
    }
	

	if(KEY_PRESS == ev->value){
		/* Add wake lock */
		//my_resume("key_lock_manager"); 
		lock_cnt += 1;
		debug("D.....key lock_cnt=[%d][%d][%d][%d]\n", lock_cnt, ev->type, ev->code, ev->value);
	}else if(KEY_RELEASE == ev->value){
		/*Release wake lock */
		//my_suspend("key_lock_manager");
		lock_cnt -= 1;
        debug("U.....key lock_cnt=[%d][%d][%d][%d]\n", lock_cnt, ev->type, ev->code, ev->value);
	}

    //check if the key was pressed
    for(i=0; i<MAX_KEYS; i++) {
        if(keys.key[i].code == ev->code) {
			//dbg_info("%d find key:%d\n", i, ev->code);
            break;
        }
    }

    //if not be pressed, get info array space index
    if(MAX_KEYS == i) {
        for(i=0; i<MAX_KEYS; i++) {
            if( (keys.key[i].code == 0) || (KEY_RELEASE == keys.key[i].status)) {
                if (!get_key_press_num()) {
                    //new key pressed in wait click gap time
                    j = get_key_info(0);
                    if(j < MAX_KEYS) {
						dbg_info("if(j < MAX_KEYS)\n");
                        single_key_handle(keys.key[j].code, keys.key[j].status, keys.key[j].cnt);
                    } else {
						dbg_info("get_key_info return %d\n",j);
					}
                } else {
					dbg_info("get_key_press_num return 0\n");
				}
                keys.key[i].cnt = 0;
                break;
            }
        }
    }

    if(MAX_KEYS == i) {
        debug("too many keys pressed\n");
        return;
    }

    if (KEY_REPEAT != ev->value) {
		j = get_key_press_num();
		dbg_info("j=%d,ev->value=%d, keys.press_cnt=%d,keys.key[i].cnt=%d\n", j, ev->value, keys.press_cnt, keys.key[i].cnt);
        if (!j || KEY_PRESS == ev->value) {
            while (gettimeofday(&keys.start_time,  (struct timezone *)0) < 0) {;}
        } else if(!keys.press_cnt && j > 1) {
			keys.press_cnt = j;
			multiple_key_handler(ev->code, ev->value, j);
			keys.key[i].cnt = 0;
			keys.key[i].code = 0;
			while (gettimeofday(&keys.start_time,  (struct timezone *)0) < 0) {;}
		} else if(keys.press_cnt && j >= 1) {
			keys.press_cnt = j-1;
			keys.key[i].cnt = 0;
			keys.key[i].code = 0;
			key_release_modify(ev->code, KEY_RELEASE, j);
			//all keys release from mutiple key process
			if(j == 1)return;
        } else if(!keys.press_cnt && j == 1) {
			if(keys.keys_status == LONG_KEY_PRESS) {
				single_long_key_release(keys.key[i].code, 0, 1);
				keys.keys_status = 0;
			}
		}
        if(!keys.key[i].status) {
            keys.key[i].cnt += !!ev->value;
        }
    }
	dbg_info("j=%d,ev->value=%d, keys.press_cnt=%d,keys.key[i].cnt=%d\n", j, ev->value, keys.press_cnt, keys.key[i].cnt);
    keys.key[i].status = ev->value;
    keys.key[i].code = ev->code;
	//keys_debug_info();
}

void input_keys_analyse(int type, struct input_event *ev) {
    long long time_diff_temp;
    struct timeval now_time = {0, 0};
    int i;
    int press_cnt = 0;
    static int value = 0;

    while (gettimeofday(&keys.end_time,  (struct timezone *)0) < 0) {;}
    time_diff_temp = timeval_diff(keys.end_time, keys.start_time);

    
    if (POLL_TIMEOUT == type) {
        check_poll_timeout(time_diff_temp);
    } else {    
        //if(value != ev->value){
            dbg_info("ev->value:%d\n",ev->value);
            value = ev->value;
        //}
        check_key_status(ev,time_diff_temp);
    }
}

void *input_thread(void *cookie) {
    int fd = -1;
    for(;;) {
        struct input_event ev;
        do {
            fd = ev_get(&ev,KEY_PRESS_TIME_STEP);
            if(-1 == fd) {
                input_keys_analyse(POLL_TIMEOUT, &ev);
            }
            else if(EV_KEY == ev.type) {
                input_keys_analyse(POLL_SUCCESS, &ev);
            }
            else if(EV_SYN == ev.type && SYN_DROPPED == ev.code) {
                keys_buffer_overflow_process();
            }
        }while(ev.type != EV_KEY || ev.code > KEY_MAX);
    }
}

int main(int argc, char *argv[]) {
    pthread_t t_input;
    int ret;
    debug("start key listen\n");
    ev_init();
    if(0 == ev_count) {
        debug("open input file failed:%d\n", ev_count);
        return -1;
    }
    ret = pthread_create(&t_input, NULL, input_thread, NULL);
    if(ret) {
        debug("thread input_thread create failed\n");
        ev_exit();
        return -1;
    }

    keys_init();
    pthread_join(t_input, NULL);
    return 0;
}
