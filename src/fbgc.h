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
#ifndef FBGC_H
#define FBGC_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include "../include/queue.h"
#include "../include/vector.h"
#include "../include/bool_vector.h"

#include "../include/utility.h"


#include "../include/tokens.h"
#include "../include/error.h"

#include "../include/fbgc_object.h"

#include "../include/memory.h"

#include "../include/linked_list.h"
#include "../include/array_object.h"
#include "../include/double_object.h"
#include "../include/int_object.h"
#include "../include/logic_object.h"
#include "../include/complex_object.h"	
#include "../include/str_object.h"
#include "../include/map_object.h"
#include "../include/matrix_object.h"
#include "../include/tuple_object.h"
#include "../include/cmodule_object.h"
#include "../include/fun_object.h"
#include "../include/range_object.h"
#include "../include/cstruct_object.h"
#include "../include/class_object.h"

#include "../include/symbol_table.h"
#include "../include/field.h"


#include "../include/relexer.h"
#include "../include/grammar.h"
#include "../include/parser.h"
#include "../include/interpreter.h"

extern struct fbgc_object * current_field;
extern struct fbgc_object * __fbgc_nil;
struct fbgc_object * fbgc_load_module(const char * module_name,const char * fun_name, uint8_t load_key);
struct fbgc_object * fbgc_load_file(const char * file_name,const char * fun_name, uint8_t load_key);
int8_t parse_tuple_content(struct fbgc_cfun_arg * cfun_arg, const char * format, ...);
int8_t parse_keywordargs_content(struct fbgc_cfun_arg * cfun_arg,const char ** keywords, const char * format, ...);
struct parser_packet;

#define fbgc_nil_object __fbgc_nil

struct fbgc_runtime{
	struct fbgc_class_object main_field;
	struct fbgc_vector x;
};


#ifdef  __cplusplus
}
#endif

#endif