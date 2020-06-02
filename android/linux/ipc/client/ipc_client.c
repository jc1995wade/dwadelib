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

#include "ipc_client.h"
#include <time.h>

#define BUFF_MAX_NUM  4096 
#define IPC_LOCAL_IP_ADDRESS "127.0.0.1"
#define LOCAL_DEBUG_ON 0

static int serverfd;
static ipc_callback_list_t response_callback_list;
static ipc_callback_list_t register_callback_list;
static pthread_t createthread;

#include "ipc_client_internal.c"

static void process_sync_call(ipc_callback_node_t *callback_node, ipc_param_t  *params) {
    int i, count;
    int param_type;
    int param_size  = sizeof(ipc_param_t);

    printf("process_sync_call:\n");
    count = get_param_count(callback_node->param_types);

    for (i = 0; i < count; i++) {
        param_type = IPC_PARAM_TYPE_GET(callback_node->param_types, i);
        if ((param_type == IPC_PARAM_TYPE_VALUE_OUTPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_INOUT)) {
            memcpy(&callback_node->params[i], &params[i], param_size);

        } else if ((param_type == IPC_PARAM_TYPE_MEMREF_OUTPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_INOUT)) {
                callback_node->params[i].memref.size = params[i].memref.size;
                memcpy(callback_node->params[i].memref.buffer, &params[i].memref.buffer, params[i].memref.size);               
        }
    }
    pthread_mutex_lock(&callback_node->mutex);
    pthread_cond_signal(&callback_node->cond);
    pthread_mutex_unlock(&callback_node->mutex);
}

static void process_async_call(ipc_callback_node_t *callback_node, ipc_param_t  *params){
    int i, count;
    int param_type;
    int param_size  = sizeof(ipc_param_t);

    printf("process_async_call:\n");
    count = get_param_count(callback_node->param_types);

    for (i = 0; i < count; i++) {
        param_type = IPC_PARAM_TYPE_GET(callback_node->param_types, i);
        if ((param_type== IPC_PARAM_TYPE_VALUE_OUTPUT) ||
            (param_type== IPC_PARAM_TYPE_VALUE_INOUT)) {
            memcpy(&callback_node->params[i], &params[i], param_size);

        } else if ((param_type== IPC_PARAM_TYPE_MEMREF_OUTPUT) ||
                   (param_type== IPC_PARAM_TYPE_MEMREF_INOUT)) {
                callback_node->params[i].memref.size = params[i].memref.size;
                memcpy(&callback_node->params[i].memref.buffer, &params[i].memref.buffer, params[i].memref.size);               
        }
    }

    /* call async func */
    if (callback_node->callback != NULL) {
        callback_node->callback(callback_node->param_types, callback_node->params);
    }
    free(callback_node);
}

static char    buff[BUFF_MAX_NUM];
static void* start_receive(void * arg) {
    int      connfd;
    int      n;
    uint32_t cmd_id;
    uint32_t param_types;
    ipc_param_t  params[IPC_NUM_PARAMS]; 
    struct list_node *call_node;
    ipc_callback_node_t * callback_node;

    printf("start_receive:\n");
    while(1){
         if( (connfd = accept(serverfd, (struct sockaddr*)NULL, NULL)) == -1) {
            printf("accept socket error: %s(errno: %d)\n",strerror(errno),errno);
            continue;
         }
         n = recv(connfd, buff, BUFF_MAX_NUM, 0);
         display_buff(buff, n);
         if (ipc_convert_data(buff, n, &cmd_id, params, &param_types) == 0) {
             printf("start_receive: cmd_id 0x%x\n", cmd_id);
             display_params(params, param_types);
             if ((cmd_id & IPC_CMD_RESPOND) > 0) {
                 uint32_t id = cmd_id & IPC_CMD_RESPOND_MASK;
                 /* respond */
                 pthread_mutex_lock(&response_callback_list.mutex);
                 list_for_every(&response_callback_list.list, call_node) {
			callback_node = containerof(call_node, ipc_callback_node_t, node);
			if (callback_node->cmd_id == id) {
				list_delete(&callback_node->node);
				printf("start_receive: found %d\n", callback_node->cmd_id);
				break;
			}
		 }             
                 pthread_mutex_unlock(&response_callback_list.mutex);
                 if (callback_node != NULL) {
                     if (callback_node->callback == NULL) {
                         /* synch call */
                         process_sync_call(callback_node, params);
                     } else {
                           /* async acall */
                           process_async_call(callback_node, params);
                     }     
                 } else {
                     printf("start_receive: not found %d \n", cmd_id);
                 }
             } else if ((cmd_id & IPC_CMD_UNSOLICITE_MASK) > 0) {
                 /* unsolicite */
                 printf("start_receive: unsolicite cmd_id 0x%x\n", cmd_id);
                 pthread_mutex_lock(&register_callback_list.mutex);
                 list_for_every(&register_callback_list.list, call_node) {
			callback_node = containerof(call_node, ipc_callback_node_t, node);
			if (callback_node->cmd_id == (cmd_id)) {
                            printf("start_receive: found %d\n", callback_node->cmd_id);
			    break;
			}
		 }             
                 pthread_mutex_unlock(&register_callback_list.mutex);
                 if (callback_node != NULL) {
                     process_async_call(callback_node, params);
                 }
             }
         }
         close(connfd);
    }
}

int ipc_client_init(){
    struct sockaddr_in    servaddr;

    pthread_mutex_init(&response_callback_list.mutex, NULL);
    list_initialize(&response_callback_list.list);
    pthread_mutex_init(&register_callback_list.mutex, NULL);
    list_initialize(&register_callback_list.list);

    /* server socket params */
    if( (serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("ipc_client_init： create server socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(IPC_SERVER_CLIENT_PORT);

    if( bind(serverfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("ipc_client_init: bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 1;
    }

    if( listen(serverfd, 10) == -1){
        printf("ipc_client_init：listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 1;
    }

    pthread_create(&createthread, NULL, start_receive, NULL);

    return 0;
}

int ipc_client_register_callback(uint32_t cmd_id, ipc_callback_t callback, ipc_param_t  *params, uint32_t param_types) {
    void * buff;
    int param_size;
    int size, i = 0, count = 0;
    ipc_callback_node_t * callback_node;
    
    printf("ipc_client_register_callback: cmd_id 0x%x\n", cmd_id);
    display_params(params, param_types);
    param_size  = sizeof(ipc_param_t);
    callback_node = (ipc_callback_node_t *) malloc(sizeof(ipc_callback_node_t));
    if (callback_node == NULL) {
        printf("ipc_client_register_callback: malloc callback failed!\n"); 
        return;
    }
    list_initialize(&callback_node->node);

    buff = ipc_convert_param(cmd_id, params, param_types, &size);
    if ((buff == NULL) || (size == 0)) {
        printf("ipc_client_register_callback: params errors!\n");
        free(callback_node);
        return 1;
    }
    
    if (ipc_send_data(buff, size) == 1) {
        printf("ipc_client_register_callback: send data error!\n");
        free(callback_node);
        return 1;
    }
    pthread_cond_init(&callback_node->cond, NULL);
    pthread_mutex_init(&callback_node->mutex, NULL);
    callback_node->cmd_id = cmd_id;
    callback_node->callback = callback;
    for (i = 0; i < IPC_NUM_PARAMS; i++) {
        callback_node->params[i] = params[i];
    }
    callback_node->param_types  = param_types;
    pthread_mutex_lock(&register_callback_list.mutex);
    list_add_tail(&register_callback_list.list, &callback_node->node);
    pthread_mutex_unlock(&register_callback_list.mutex);

   return 0;
}

void ipc_client_unregister_callback(uint32_t cmd_id){

}

int ipc_client_request(uint32_t cmd_id, ipc_callback_t callback, ipc_param_t  *params, uint32_t param_types) {

    void * buff;
    int param_type;
    int param_size;
    int size, i = 0, count = 0;
    ipc_callback_node_t * callback_node;
    struct list_node *call_node;

    printf("ipc_client_request: cmd_id 0x%x\n", cmd_id);
    display_params(params, param_types);
    param_size  = sizeof(ipc_param_t);
    callback_node = (ipc_callback_node_t *) malloc(sizeof(ipc_callback_node_t));
    if (callback_node == NULL) {
        printf("ipc_client_request: malloc callback failed!\n"); 
        return 1;
    }
    list_initialize(&callback_node->node);

    /* add async mark */
    if (callback != NULL) {
        param_types |= IPC_PARAM_TYPE_CALLBABK;
    }

    buff = ipc_convert_param(cmd_id, params, param_types, &size);

    if ((buff == NULL) || (size == 0)) {
        printf("ipc_client_request:: params errors!\n");
        free(callback_node);
        return 1;
    }
    
    if (ipc_send_data(buff, size) == 1) {
        printf("ipc_client_request: send data error!\n");
        free(buff);
        free(callback_node);
        return 1;
    }
    pthread_cond_init(&callback_node->cond, NULL);
    pthread_mutex_init(&callback_node->mutex, NULL);
    callback_node->cmd_id = cmd_id;
    callback_node->callback = callback;
    for (i = 0; i < IPC_NUM_PARAMS; i++) {
        callback_node->params[i] = params[i];
    }
    callback_node->param_types  = param_types;
    printf("ipc_client_request: wait for return\n");
    pthread_mutex_lock(&response_callback_list.mutex);
    list_add_tail(&response_callback_list.list, &callback_node->node);
    pthread_mutex_unlock(&response_callback_list.mutex);
    /*  synchronous call*/
    if (callback == NULL ) {
       /* wait for 100s */
       int status;
       struct timespec time_to_giveup;
       clock_gettime(CLOCK_REALTIME, &time_to_giveup);
       time_to_giveup.tv_sec += 100;
       printf("ipc_client_request: wait for %ld\n", time_to_giveup.tv_sec);
       pthread_mutex_lock(&callback_node->mutex);
       status = pthread_cond_timedwait(&callback_node->cond, &callback_node->mutex, &time_to_giveup);
       pthread_mutex_unlock(&callback_node->mutex);
       if (status == ETIMEDOUT) {
           printf("ipc_client_request: sync timeout !");
           free(callback_node);
           return 1;
       }

       /* copy paramterms */
       printf("ipc_client_request: sync return \n");
       count = get_param_count(param_types);
       for (i = 0; i < count; i++) {
           param_type = IPC_PARAM_TYPE_GET(callback_node->param_types, i);
           if ((param_type == IPC_PARAM_TYPE_VALUE_OUTPUT) ||
                (param_type == IPC_PARAM_TYPE_VALUE_INOUT)) {
                memcpy(&params[i], &callback_node->params[i], param_size);

        } else if ((param_type == IPC_PARAM_TYPE_MEMREF_OUTPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_INOUT)) {
                params[i].memref.size = callback_node->params[i].memref.size;
                memcpy(&params[i].memref.buffer, callback_node->params[i].memref.buffer, params[i].memref.size);               
        }
    }
       
    }

    return 0;
}
/* for test */
void tcallback(uint32_t param_types, ipc_param_t *params) {
    printf("tcallback: param_types 0x%x\n", param_types);
    display_params(params, param_types);
}

int main(int argc, char** argv) {

    uint32_t cmd_id = IPC_CMD_REQUEST_INIT;
    ipc_param_t  params[IPC_NUM_PARAMS]; 
    uint32_t param_types ;
    int param_size = sizeof(ipc_param_t);
    char buff[40]= {78, 45, 68, 110, 34,56,90, 88};

    memset(params, 0, param_size);
    param_types = IPC_TWO_PARAM_TYPES(IPC_PARAM_TYPE_VALUE_INPUT,IPC_PARAM_TYPE_VALUE_OUTPUT);
    params[0].value.a = 10;
    params[0].value.b = 20;
    params[1].value.a = 30;
    params[1].value.b = 40;
    ipc_client_init();
    ipc_client_request(cmd_id, NULL, params, param_types);
    display_params(params, param_types);

    cmd_id = IPC_CMD_REQUEST_INIT;
    memset(params, 0, param_size);
    param_types = IPC_TWO_PARAM_TYPES(IPC_PARAM_TYPE_VALUE_INPUT,IPC_PARAM_TYPE_VALUE_OUTPUT);
    params[0].value.a = 60;
    params[0].value.b = 70;
    params[1].value.a = 80;
    params[1].value.b = 90;
    ipc_client_request(cmd_id, tcallback, params, param_types);

    cmd_id = IPC_CMD_INIT_UNSOLICITE;
    memset(params, 0, param_size);
    param_types = IPC_TWO_PARAM_TYPES(IPC_PARAM_TYPE_VALUE_INPUT,IPC_PARAM_TYPE_MEMREF_OUTPUT);
    params[0].value.a = 160;
    params[0].value.b = 170;
    params[1].memref.size = 40;
    params[1].memref.buffer = (void *) buff;
    display_params(params, param_types);
    ipc_client_register_callback(cmd_id, tcallback, params, param_types);

    pthread_join(createthread, NULL);
    return 0;
}

