#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define set_mark_bit_one(x)(x |= 0x80)
#define set_mark_bit_zero(x)(x &= 0x7F)

#define get_fbgc_object_mark_bit(x)( x & 0x80)




struct fbgc_garbage_object{
    struct fbgc_object base;
    size_t size;
};
#define cast_fbgc_object_as_garbage(x)(((struct fbgc_garbage_object*) x))


#ifdef  __cplusplus
}
#endif

#endif
