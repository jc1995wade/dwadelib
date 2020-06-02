/*
* Copyright (c) 2019, Spreadtrum Communications.
*
* The above copyright notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANYÂ 
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THEÂ 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ipc_server.h"
#include <time.h>

#define BUFF_MAX_NUM  4096 
#define IPC_LOCAL_IP_ADDRESS "127.0.0.1"
#define LOCAL_DEBUG_ON 0

static int serverfd;
/* for async call */
static ipc_callback_list_t response_callback_list;
static ipc_callback_list_t register_callback_list;
static pthread_t createthread;

#include "ipc_server_internal.c"
#include "ipc_server_command.c"

static void process_async_command(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types) {
    int i;
    ipc_callback_node_t * callback_node;
    struct list_node *call_node;
 
    printf("process_async_command: 0x%x\n", cmd_id);
    callback_node = (ipc_callback_node_t *) malloc(sizeof(ipc_callback_node_t));
    if (callback_node == NULL) {
        printf("process_async_command: malloc callback failed!\n"); 
        return;
    }
    list_initialize(&callback_node->node);
    pthread_cond_init(&callback_node->cond, NULL);
    pthread_mutex_init(&callback_node->mutex, NULL);
    callback_node->cmd_id = cmd_id;
    for (i = 0; i < IPC_NUM_PARAMS; i++) {
        callback_node->params[i] = params[i];
    }
    callback_node->param_types  = param_types;
    pthread_mutex_lock(&response_callback_list.mutex);
    list_add_tail(&response_callback_list.list, &callback_node->node);
    pthread_mutex_unlock(&response_callback_list.mutex);

    process_sync_command(cmd_id, params, param_types, false);

}

static void process_command_call(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types){

     printf("process_command_call: 0x%x\n", cmd_id);
     if ((param_types & IPC_PARAM_TYPE_CALLBABK) > 0) {
         /* async call */
         process_async_command(cmd_id, params, param_types);
     } else {
         /* sync call */
         process_sync_command(cmd_id, params, param_types, true);
     }
}

static void process_register(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types){

   int i;
    ipc_callback_node_t * callback_node;
    struct list_node *call_node;
 
    printf("process_register: 0x%x\n", cmd_id);
    callback_node = (ipc_callback_node_t *) malloc(sizeof(ipc_callback_node_t));
    if (callback_node == NULL) {
        printf("process_register: malloc callback failed!\n"); 
        return;
    }
    list_initialize(&callback_node->node);
    pthread_cond_init(&callback_node->cond, NULL);
    pthread_mutex_init(&callback_node->mutex, NULL);
    callback_node->cmd_id = cmd_id;
    for (i = 0; i < IPC_NUM_PARAMS; i++) {
        callback_node->params[i] = params[i];
    }
    callback_node->param_types  = param_types;
    pthread_mutex_lock(&register_callback_list.mutex);
    list_add_tail(&register_callback_list.list, &callback_node->node);
    pthread_mutex_unlock(&register_callback_list.mutex);

    process_register_call(cmd_id, params, param_types);
}

static char    buff[BUFF_MAX_NUM];
static void *start_receive(void *arg) {
    int      connfd;
    int      n;
    uint32_t cmd_id;
    uint32_t param_types;
    ipc_param_t  params[IPC_NUM_PARAMS]; 
    
    printf("start_receive\n");
    while(1){
         if( (connfd = accept(serverfd, (struct sockaddr*)NULL, NULL)) == -1) {
            printf("start_receive: accept socket error %s(errno: %d)\n",strerror(errno),errno);
            continue;
         }
         n = recv(connfd, buff, BUFF_MAX_NUM, 0);
         display_buff(buff, n);
         display_params(params, param_types);
         if (ipc_convert_data(buff, n, &cmd_id, params, &param_types) == 0) {
             printf("start_receive: cmd_id 0x%x\n", cmd_id);
             display_params(params, param_types);
             if ((cmd_id & IPC_CMD_UNSOLICITE_MASK) > 0) {
                 /* process register */
                 process_register(cmd_id, params, param_types);                                  
             } else {
                 /* process command */
                 process_command_call(cmd_id, params, param_types);
                 
             }
         }
         close(connfd);
    }
}

int ipc_server_init(){
    struct sockaddr_in    servaddr;

    pthread_mutex_init(&response_callback_list.mutex, NULL);
    list_initialize(&response_callback_list.list);
    pthread_mutex_init(&register_callback_list.mutex, NULL);
    list_initialize(&register_callback_list.list);

    /* server socket params */
    if( (serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("ipc_server_init： create server socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(IPC_CLIENT_SERVER_PORT);

    if( bind(serverfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("ipc_server_init: bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 1;
    }

    if( listen(serverfd, 10) == -1){
        printf("ipc_server_init：listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 1;
    }

    pthread_create(&createthread, NULL, start_receive, NULL);

    return 0;
}

int ipc_server_respond_async_call(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types) {
    void * buff;
    int param_size;
    int size, i = 0, count = 0;
    struct list_node *call_node;
    ipc_callback_node_t * callback_node;

    printf("ipc_server_respond_async_call: cmd_id 0x%x\n", cmd_id);
    display_params(params, param_types);
    param_size  = sizeof(ipc_param_t);
    

    buff = ipc_convert_param(cmd_id, params, param_types, &size);

    if ((buff == NULL) || (size == 0)) {
        printf("ipc_server_respond_async_call: params errors!\n");
        return 1;
    }
   
    if (ipc_send_data(buff, size) == 1) {
        printf("ipc_server_respond_async_call: send data error!\n");
        return 1;
    }
    /* remove node */
    pthread_mutex_lock(&response_callback_list.mutex);
    list_for_every(&response_callback_list.list, call_node) {
       callback_node = containerof(call_node, ipc_callback_node_t, node);
       if (callback_node->cmd_id == cmd_id) {
	   list_delete(&callback_node->node);
	   printf("ipc_server_respond_async_call: found %d\n", callback_node->cmd_id);
	   break;
	}
    }             
    pthread_mutex_unlock(&response_callback_list.mutex);
    free(callback_node);
    free(buff);
    return 0;   
}

int ipc_server_respond_call(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types){
    void * buff;
    int param_size;
    int size, i = 0, count = 0;

    printf("ipc_server_respond_call: cmd_id 0x%x\n", cmd_id);
    display_params(params, param_types);
    param_size  = sizeof(ipc_param_t);
    

    buff = ipc_convert_param(cmd_id, params, param_types, &size);

    if ((buff == NULL) || (size == 0)) {
        printf("ipc_server_respond_call: params errors!\n");
        return 1;
    }
   
    if (ipc_send_data(buff, size) == 1) {
        printf("ipc_server_respond_call: send data error!\n");
        return 1;
    }

    free(buff);
    return 0;   
}

int ipc_server_respond_register(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types) {
    void * buff;
    int param_size;
    int size, i = 0, count = 0;
    struct list_node *call_node;
    ipc_callback_node_t * callback_node;

    printf("ipc_server_respond_register: cmd_id 0x%x\n", cmd_id);
    display_params(params, param_types);
    param_size  = sizeof(ipc_param_t);
    /* search for cmd_id */
    pthread_mutex_lock(&register_callback_list.mutex);
    list_for_every(&register_callback_list.list, call_node) {
       callback_node = containerof(call_node, ipc_callback_node_t, node);
       if (callback_node->cmd_id == cmd_id) {
	   printf("ipc_server_respond_register: found %d\n", callback_node->cmd_id);
	   break;
	}
    }             
    pthread_mutex_unlock(&register_callback_list.mutex);  
    if (callback_node == NULL) {
        printf("ipc_server_respond_register: nor found 0x%x\n", cmd_id);
        return 1;
   }

    buff = ipc_convert_param(cmd_id, params, param_types, &size);

    if ((buff == NULL) || (size == 0)) {
        printf("ipc_server_respond_register: params errors!\n");
        return 1;
    }
   
    if (ipc_send_data(buff, size) == 1) {
        printf("ipc_server_respond_register: send data error!\n");
        return 1;
    }
    free(buff);
    return 0;   
}

/* for test */
int main(int argc, char** argv) {

    uint32_t cmd_id = IPC_CMD_REQUEST_INIT;
    ipc_param_t  params[IPC_NUM_PARAMS]; 
    uint32_t param_types ;

    ipc_server_init();

    pthread_join(createthread, NULL);
    return 0;
}

