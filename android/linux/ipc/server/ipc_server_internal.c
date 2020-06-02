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

static int get_param_count(uint32_t param_types) {
    int i, count = 0;
 
    for (i = 0; i < IPC_NUM_PARAMS; i++) {
        if (IPC_PARAM_TYPE_GET(param_types, i) == IPC_PARAM_TYPE_NONE) {
            break;
        }
        count++;
    }
    return count;
}

static void display_buff(char *buff, int size) {
  if (LOCAL_DEBUG_ON) {
     int i = 0;
     printf("display buff: %d\n", size);
     for (i = 0; i < size; i += 4) {
         printf("%x, %x, %x, %x\n", buff[i + 0], buff[i + 1], buff[i + 2], buff[i + 3]);
     }
  }
}
static void display_params(ipc_param_t  *params, uint32_t param_types) {
if (LOCAL_DEBUG_ON) {
        int count = get_param_count(param_types);
        int param_type, i;
        printf("display_params: param_types 0x%x\n", param_types);
        for (i = 0; i < count; i++) {
           param_type = IPC_PARAM_TYPE_GET(param_types, i);
           if ((param_type == IPC_PARAM_TYPE_VALUE_INPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_OUTPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_INOUT)) {
                printf("display_params: type %d, a %d, b %d\n", param_type, params[i].value.a, params[i].value.b);
            } else if ((param_type == IPC_PARAM_TYPE_MEMREF_INPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_OUTPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_INOUT)) {
                 
               printf("display_params: type %d, size %d\n", param_type, params[i].memref.size);
               printf("display_params: %d, %d, %d, %d, %d, %d, %d, %d \n", ((char *)params[i].memref.buffer)[0], ((char *)params[i].memref.buffer)[1],
                       ((char *)params[i].memref.buffer)[2], ((char *)params[i].memref.buffer)[3],((char *)params[i].memref.buffer)[4], ((char *)params[i].memref.buffer)[5],
                       ((char *)params[i].memref.buffer)[6], ((char *)params[i].memref.buffer)[7]); 
            }
        }
    }
}
static int ipc_convert_data(char *buff, int size, uint32_t *cmd_id, ipc_param_t  *params, uint32_t *param_types) {

    int i = 0, count = 0;
    int pos = 0;
    int param_type;
    int uint32_size = 0;
    int param_size = 0;
    
    printf("ipc_convert_data: %d\n", size);
    uint32_size = sizeof(uint32_t);
    param_size  = sizeof(ipc_param_t);

    /* get command id*/
    memcpy(cmd_id, buff, uint32_size);
    pos += uint32_size;
    /* get parameter type*/
    memcpy(param_types, buff + pos, uint32_size);
    pos += uint32_size;
    /* get parameter count*/
    count = get_param_count(*param_types);

    if (count == 0) {
        printf("ipc_convert_data: count is zero!!!\n");
    }

    /* calculate momery size */
    for (i = 0; i < count; i++) {
        param_type = IPC_PARAM_TYPE_GET(*param_types, i);
        if ((param_type == IPC_PARAM_TYPE_VALUE_INPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_OUTPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_INOUT)) {
            memcpy(&params[i], buff + pos, param_size);
            pos += param_size;
        } else if ((param_type == IPC_PARAM_TYPE_MEMREF_INPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_OUTPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_INOUT)) {
                memcpy(&params[i].memref.size, buff+pos, uint32_size);
                pos += uint32_size;
                params[i].memref.buffer = (void *) (buff+pos);               
                pos += params[i].memref.size;
        }
    }
    return 0;
}

static void * ipc_convert_param(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types, int *out_size) {

    int size;
    char *buff = NULL;
    int i = 0, count = 0;
    int pos = 0;
    int param_type;
    int uint32_size = 0;
    int param_size = 0;

    uint32_size = sizeof(uint32_t);
    param_size  = sizeof(ipc_param_t);
    /* get parameter count*/
    count = get_param_count(param_types);

    if (count == 0) {
        printf("ipc_convert_param: count is zero!!!\n");
    }

    /* parameter type size and command id size*/
    size = uint32_size + uint32_size;

    /* calculate momery size */
    for (i = 0; i < count; i++) {
        param_type = IPC_PARAM_TYPE_GET(param_types, i);
        if ((param_type == IPC_PARAM_TYPE_VALUE_INPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_OUTPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_INOUT)) {
            size += param_size;
        } else if ((param_type == IPC_PARAM_TYPE_MEMREF_INPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_OUTPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_INOUT)) {
               size += uint32_size + params[i].memref.size;
        }
    }
    /* allocate momery */
    buff = (char *) malloc(size);
    if (buff == NULL) {
        printf("ipc_convert_param, malloc %d failed\n", size);
        return NULL;
    }
    pos = 0;
    /* copy data */
    memcpy(buff, &cmd_id, uint32_size);
    pos += uint32_size;
    memcpy(buff+pos, &param_types, uint32_size);
    pos += uint32_size;
    for (i = 0; i < count; i++) {
        param_type = IPC_PARAM_TYPE_GET(param_types, i);
        if ((param_type == IPC_PARAM_TYPE_VALUE_INPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_OUTPUT) ||
            (param_type == IPC_PARAM_TYPE_VALUE_INOUT)) {
            memcpy(buff+pos, &params[i].value, param_size);
            pos += param_size;

        } else if ((param_type == IPC_PARAM_TYPE_MEMREF_INPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_OUTPUT) ||
                   (param_type == IPC_PARAM_TYPE_MEMREF_INOUT)) {
               memcpy(buff+pos, &params[i].memref.size, uint32_size);
               pos += uint32_size;
               memcpy(buff+pos, params[i].memref.buffer, params[i].memref.size);
               pos += params[i].memref.size;
        }
    }
    *out_size = size;
    return buff;      
}

static int ipc_send_data(void * buff, int size) {
    int clientfd;
    struct sockaddr_in    servaddr;

    printf("ipc_send_data: send msg to server: %d\n", size);

    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("ipc_server_init： create client socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 1;
    }
    display_buff(buff, size);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(IPC_SERVER_CLIENT_PORT);
    if( inet_pton(AF_INET, IPC_LOCAL_IP_ADDRESS, &servaddr.sin_addr) <= 0){
        printf("ipc_send_data： client inet_pton error for %s\n", IPC_LOCAL_IP_ADDRESS);
        close(clientfd);
        return 1;
    }

    if( connect(clientfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("ipc_send_data：connect error: %s(errno: %d)\n",strerror(errno),errno);
        close(clientfd);
        return 1;
    }

    if( send(clientfd, buff, size, 0) < 0)
    {
        printf("ipc_send_data: send msg error: %s(errno: %d)\n", strerror(errno), errno);
        close(clientfd);
        return 1;
    }
    close(clientfd);
    return 0;
}


