// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#ifndef INTERPRETER_H
#define INTERPRETER_H

#ifdef __cplusplus
extern "C" {
#endif


struct interpreter_packet{
	struct fbgc_object ** sp;
	struct fbgc_object * current_scope;
	struct fbgc_ll_base * pc;
	int sctr;
	int fctr;
};


extern struct interpreter_packet * global_interpreter_packet;

/*! @fn uint8_t interpreter(struct fbgc_object ** field_obj)
	
	@param field : The corresponding field object to interpret
*/
uint8_t interpreter(struct fbgc_object ** field);


struct fbgc_object * call_fun_object(struct fbgc_object * fun);


/*! @fn uint8_t interpreter(struct fbgc_object ** field_obj)
	
	@param field : The corresponding field object to interpret
*/
struct fbgc_object * run_code(struct interpreter_packet * ip);



#ifdef LOG_INTERPRETER
#define INTERPRETER_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define INTERPRETER_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _INTERPRETER_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _INTERPRETER_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define INTERPRETER_LOGV(...)
#define INTERPRETER_LOGD(...)
#define _INTERPRETER_LOGV(...)
#define _INTERPRETER_LOGD(...)
#endif

#ifdef  __cplusplus
}
#endif

#endif