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


typedef uint8_t bool;
#define true 1
#define false 0


#include "headers/tokens.h"
#include "headers/error.h"

#include "headers/fbgc_object.h"

#include "headers/memory.h"
#include "headers/garbage_collector.h"

#include "headers/linked_list.h"
#include "headers/array_object.h"
#include "headers/double_object.h"
#include "headers/int_object.h"
#include "headers/logic_object.h"
#include "headers/complex_object.h"	
#include "headers/str_object.h"
#include "headers/map_object.h"
#include "headers/matrix_object.h"
#include "headers/tuple_object.h"
#include "headers/cmodule_object.h"
#include "headers/fun_object.h"
#include "headers/range_object.h"
#include "headers/cstruct_object.h"
#include "headers/class_object.h"

#include "headers/operator.h"

#include "headers/symbol_table.h"
#include "headers/field_object.h"


#include "headers/relexer.h"
#include "headers/grammar.h"
#include "headers/parser.h"
#include "headers/interpreter.h"

extern struct fbgc_object * current_field;
extern struct fbgc_object * __fbgc_nil;
struct fbgc_object * fbgc_load_module(const char * module_name,const char * fun_name, uint8_t load_key);
struct fbgc_object * fbgc_load_file(const char * file_name,const char * fun_name, uint8_t load_key);
int8_t parse_tuple_content(struct fbgc_cfun_arg * cfun_arg, const char * format, ...);
struct parser_packet;

#define fbgc_nil_object __fbgc_nil


#ifdef  __cplusplus
}
#endif

#endif