#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

void set_mark_bit_one(struct fbgc_object *);
void set_mark_bit_zero(struct fbgc_object *);


#ifdef  __cplusplus
}
#endif

#endif
