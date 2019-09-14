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

#define size_t unsigned int	

#include "headers/tokens.h"
#include "headers/error.h"

#include "headers/fbgc_object.h"

#include "headers/memory.h"
#include "headers/garbage_collector.h"


#include "headers/linked_list.h"
#include "headers/array_object.h"
#include "headers/jumper_object.h"
#include "headers/ref_object.h"
#include "headers/double_object.h"
#include "headers/int_object.h"
#include "headers/str_object.h"
#include "headers/matrix_object.h"
#include "headers/tuple_object.h"
#include "headers/cmodule_object.h"
#include "headers/fun_object.h"
#include "headers/range_object.h"


#include "headers/operator.h"

#include "headers/symbol_table.h"
#include "headers/field_object.h"
#include "headers/opcodes.h"


#include "headers/relexer.h"
#include "headers/grammar.h"
#include "headers/parser.h"
#include "headers/interpreter.h"



#ifdef  __cplusplus
}
#endif

#endif