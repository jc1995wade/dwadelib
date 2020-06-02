1. ipc的client -> server的通信方式如下：
  1) client -> server的函数同步调用
  2) client -> server的函数同步调用
  3) server -> client主动上报注册的消息；

2. 函数需要定义对应的命令， 分别在client/server的ipc_comm_command.h文件中， 例如：

   /* add request command ID */
#define IPC_CMD_REQUEST_INIT   1 << IPC_CMD_RESPOND_BIT_NUM
#define IPC_CMD_REQUEST_OPEN   2 << IPC_CMD_RESPOND_BIT_NUM      
#define IPC_CMD_REQUEST_CLOSE  3 << IPC_CMD_RESPOND_BIT_NUM  

3. 注册和异步函数的回调函数需要定义成如下形式：

   void (*func)(uint32_t, ipc_param_t *);

   可参考ipc_client.c中的例子：

   void tcallback(uint32_t param_types, ipc_param_t *params)

4. client调用服务端的函数时， 需要调用如下函数

   int ipc_client_request(uint32_t cmd_id, ipc_callback_t *callback, ipc_param_t  *params, uint32_t param_types)

   参数说明：

   cmd_id 表示命令， 如 2中定义的
 
   callback        定义了回调函数， 如3中说明；有回调函数表示异步， 否则表示同步；

   params    定义了两类数据类型； 最多传递6个参数；
                               传值；    只是输入类型， value.a , value.b 表示输入
                                        只是输出类型， value.a , value.b 表示输出                              
                                        同时输入输出， value.a , value.b 表示输入输出
                               传数组；  只是输入类型， memref.buffer, 表示数组， memref.size 表示大小
                                        只是输出类型， memref.buffer, 表示数组， memref.size 表示大小
                                        同时输入输出， memref.buffer, 表示数组， memref.size 表示大小

   param_types  每四位定义了一种参数类型， 共7种类型如下：

#define IPC_PARAM_TYPE_NONE             0
#define IPC_PARAM_TYPE_VALUE_INPUT      1
#define IPC_PARAM_TYPE_VALUE_OUTPUT     2
#define IPC_PARAM_TYPE_VALUE_INOUT      3
#define IPC_PARAM_TYPE_MEMREF_INPUT     5
#define IPC_PARAM_TYPE_MEMREF_OUTPUT    6
#define IPC_PARAM_TYPE_MEMREF_INOUT     7


5. client注册服务端的函数时， 需要调用如下函数， callback是返回的封装函数；

 int ipc_client_register_callback(uint32_t cmd_id, ipc_callback_t *callback, ipc_param_t  *params, uint32_t param_types);

6. server端处理client的函数调用时， 需要在ipc_server_command.c下列函数中增加处理函数， 

 process_sync_command(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types, bool is_sync)

7. server端处理client的函数调用进行异步处理时， 处理如 6 中处理外， 需要在处理完毕后， 调用如下函数：

   int ipc_server_respond_async_call(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types)

8. server端处理client的注册函数时，需要处理如下函数：

   static void process_register_call(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types）

   发送消息时， 调用如下：

   int ipc_server_respond_register(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types)

9. 测试说明

   1) 修改ipc_server_command.c中的#if 0 改为 #if 1, 

   2) gcc -o client -pthread ipc_client.c

   3) gcc -o server -pthread ipc_server.c

   4) 就可测试client 发送各种消息

   
