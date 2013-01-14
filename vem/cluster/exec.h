/*
This file is part of cphVB and copyright (c) 2012 the cphVB team:
http://cphvb.bitbucket.org

cphVB is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation, either version 3 
of the License, or (at your option) any later version.

cphVB is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the 
GNU Lesser General Public License along with cphVB. 

If not, see <http://www.gnu.org/licenses/>.
*/

#include <cphvb.h>

//Public function pointer to the Node VEM
extern cphvb_execute exec_vem_execute;


/* Initialize the VEM
 *
 * @return Error codes (CPHVB_SUCCESS)
 */
cphvb_error exec_init(const char *component_name);


/* Shutdown the VEM, which include a instruction flush
 *
 * @return Error codes (CPHVB_SUCCESS)
 */
cphvb_error exec_shutdown(void);


/* Register a new user-defined function.
 *
 * @lib Name of the shared library e.g. libmyfunc.so
 *      When NULL the default library is used.
 * @fun Name of the function e.g. myfunc
 * @id Identifier for the new function. The bridge should set the
 *     initial value to Zero. (in/out-put)
 * @return Error codes (CPHVB_SUCCESS)
 */
cphvb_error exec_reg_func(char *fun, cphvb_intp *id);


/* Execute a list of instructions (blocking, for the time being).
 * It is required that the VEM supports all instructions in the list.
 *
 * @instruction A list of instructions to execute
 * @return Error codes (CPHVB_SUCCESS)
 */
cphvb_error exec_execute(cphvb_intp count, cphvb_instruction inst_list[]);


/* Execute one instruction locally.
 *
 * @opcode   The opcode of the instruction
 * @operands The local operands in the instruction
 * @ufunc  The user-defined function struct when opcode is CPHVB_USERFUNC.
 */
void exec_local_inst(cphvb_opcode opcode, cphvb_array *operands[],
                     cphvb_userfunc *ufunc);

