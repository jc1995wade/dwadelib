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


static void process_sync_command(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types, bool is_sync) {
    printf("process_sync_command: 0x%x\n", cmd_id);
    switch (cmd_id) {
        case IPC_CMD_REQUEST_INIT:
            /* init */
            break;

        case IPC_CMD_REQUEST_OPEN:
            /* open */
            break;

        default:
            break;
    }
    params[1].value.a = 80;
    params[1].value.b = 90;
    cmd_id |=  IPC_CMD_RESPOND;
    if (is_sync) {
        ipc_server_respond_call(cmd_id, params, param_types);
#if 0
    } else {
        /* only for test async */
        ipc_server_respond_async_call(cmd_id, params, param_types);
#endif
    }    
}

static void process_register_call(uint32_t cmd_id, ipc_param_t  *params, uint32_t param_types){
    printf("process_register_call: 0x%x\n", cmd_id);
    switch (cmd_id) {
        case IPC_CMD_REQUEST_INIT:
            /* init */
            break;

        case IPC_CMD_INIT_UNSOLICITE:
            /* open */
            break;

        default:
            break;
    }

#if 0
    /* only for test register */
    ipc_server_respond_register(cmd_id, params, param_types);
#endif     
}

