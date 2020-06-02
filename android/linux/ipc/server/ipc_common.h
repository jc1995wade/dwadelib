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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include "list.h"
#include <pthread.h>
#include "ipc_comm_command.h"
#pragma once



/* Parameter Type Constants */
#define IPC_PARAM_TYPE_NONE             0
#define IPC_PARAM_TYPE_VALUE_INPUT      1
#define IPC_PARAM_TYPE_VALUE_OUTPUT     2
#define IPC_PARAM_TYPE_VALUE_INOUT      3
#define IPC_PARAM_TYPE_MEMREF_INPUT     5
#define IPC_PARAM_TYPE_MEMREF_OUTPUT    6
#define IPC_PARAM_TYPE_MEMREF_INOUT     7

#define IPC_PARAM_TYPE_CALLBABK         0x80000000

/*
 * The macro TEE_PARAM_TYPES can be used to construct a value that you can
 * compare against an incoming paramTypes to check the type of all the
 * parameters in one comparison, like in the following example:
 * if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
 *                                  TEE_PARAM_TYPE_MEMREF_OUPUT,
 *                                  TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE)) {
 *      return TEE_ERROR_BAD_PARAMETERS;
 *  }
 */
#define IPC_PARAM_TYPES(t0,t1,t2,t3,t4,t5) \
   ((t0) | ((t1) << 4) | ((t2) << 8) | ((t3) << 12) | ((t4) << 12) | ((t5) << 12)) 

#define IPC_ONE_PARAM_TYPES(t0) \
   ((t0) | ((IPC_PARAM_TYPE_NONE) << 4) | ((IPC_PARAM_TYPE_NONE) << 8) | ((IPC_PARAM_TYPE_NONE) << 12) \
         | ((IPC_PARAM_TYPE_NONE) << 12) | ((IPC_PARAM_TYPE_NONE) << 12)) 

#define IPC_TWO_PARAM_TYPES(t0,t1) \
   ((t0) | ((t1) << 4) | ((IPC_PARAM_TYPE_NONE) << 8) | ((IPC_PARAM_TYPE_NONE) << 12) \
         | ((IPC_PARAM_TYPE_NONE) << 12) | ((IPC_PARAM_TYPE_NONE) << 12)) 

#define IPC_THREE_PARAM_TYPES(t0,t1,t2) \
   ((t0) | ((t1) << 4) | ((t2) << 8) | ((IPC_PARAM_TYPE_NONE) << 12) \
         | ((IPC_PARAM_TYPE_NONE) << 12) | ((IPC_PARAM_TYPE_NONE) << 12)) 

#define IPC_FOUR_PARAM_TYPES(t0,t1,t2,t3) \
   ((t0) | ((t1) << 4) | ((t2) << 8) | ((t3) << 12) | ((IPC_PARAM_TYPE_NONE) << 12) | ((IPC_PARAM_TYPE_NONE) << 12)) 

#define IPC_FIVE_PARAM_TYPES(t0,t1,t2,t3,t4) \
   ((t0) | ((t1) << 4) | ((t2) << 8) | ((t3) << 12) | ((t4) << 12) | ((IPC_PARAM_TYPE_NONE) << 12))

/*
 * The macro TEE_PARAM_TYPE_GET can be used to extract the type of a given
 * parameter from paramTypes if you need more fine-grained type checking.
 */
#define IPC_PARAM_TYPE_GET(t, i) ((((uint32_t)t) >> ((i)*4)) & 0xF)

#define IPC_NUM_PARAMS  6

/* define socket port */
#define IPC_CLIENT_SERVER_PORT 6667
#define IPC_SERVER_CLIENT_PORT 6668

/*
 * This union describes one parameter passed by the Trusted Core Framework
 * to the entry points TA_OpenSessionEntryPoint or
 * TA_InvokeCommandEntryPoint or by the TA to the functions
 * TEE_OpenTASession or TEE_InvokeTACommand.
 *
 * Which of the field value or memref to select is determined by the
 * parameter type specified in the argument paramTypes passed to the entry
 * point.
*/
typedef union {
	struct {
		void *buffer;
		uint32_t size;
	} memref;
	struct {
		uint32_t a;
		uint32_t b;
	} value;
} ipc_param_t;


/* callback for asynchronous call */
typedef void (*ipc_callback_t)(uint32_t, ipc_param_t *);

/*  callback node */
typedef struct {
    struct list_node node;
    uint32_t         cmd_id;
    ipc_callback_t   callback;
    ipc_param_t      params[IPC_NUM_PARAMS];
    uint32_t         param_types;
    pthread_cond_t   cond;
    pthread_mutex_t  mutex;
} ipc_callback_node_t;

/*  callback list */
typedef struct {
    struct list_node list;
    pthread_mutex_t  mutex;
} ipc_callback_list_t;



