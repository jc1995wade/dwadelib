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

#include "ipc_common.h"


int ipc_client_init();

int ipc_client_register_callback(uint32_t cmd_id, ipc_callback_t callback, ipc_param_t  *params, uint32_t param_types);

void ipc_client_unregister_callback(uint32_t cmd_id);

int ipc_client_request(uint32_t cmd_id, ipc_callback_t callback, ipc_param_t  *params, uint32_t param_types);





