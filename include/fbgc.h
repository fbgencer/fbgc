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
#include <stddef.h>
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
#include <setjmp.h>

#include "../fbgc_config.h"

#include "queue.h"
#include "vector.h"
#include "bool_vector.h"

#include "utility.h"


#include "tokens.h"
#include "error.h"

#include "fbgc_object.h"

#include "memory.h"

#include "linked_list.h"
#include "array_object.h"
#include "double_object.h"
#include "int_object.h"
#include "logic_object.h"
#include "complex_object.h"	
#include "str_object.h"
#include "map_object.h"
#include "matrix_object.h"
#include "tuple_object.h"
#include "cmodule_object.h"
#include "fun_object.h"
#include "range_object.h"
#include "cstruct_object.h"
#include "class_object.h"

#include "symbol_table.h"
#include "field.h"


#include "fbgc_runtime.h"

#include "relexer.h"
#include "grammar.h"
#include "parser.h"
#include "interpreter.h"


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