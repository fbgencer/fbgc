#include "fbgc.h"

struct fbgc_object * current_field = NULL;
static struct fbgc_object __nil__ = {.type = NIL};
struct fbgc_object * __fbgc_nil = &__nil__;
struct fbgc_object ** __fbgc_runtime_program_stack = NULL;