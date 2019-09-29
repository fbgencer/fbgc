#ifndef FBGC_CSTRUCT_OBJECTH
#define FBGC_CSTRUCT_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif


// Do not change order of the objects in the struct
// last object is always size_t size!

struct fbgc_cstruct_object{
    struct fbgc_object base;
    size_t block_size; // block size of defined object
    const struct fbgc_cmodule * parent;
    void * cstruct; // defined in C 
};


#define cast_fbgc_object_as_cstruct(x)( (struct fbgc_cstruct_object *) x)

#define block_size_fbgc_cstruct_object(x)(cast_fbgc_object_as_cstruct(x)->block_size)

#define sizeof_fbgc_cstruct_object(x) (sizeof(struct fbgc_cstruct_object)+ ( block_size_fbgc_cstruct_object(x) ) )



struct fbgc_object * new_fbgc_cstruct_object(size_t bsz,const struct fbgc_cmodule * parent);




#ifdef  __cplusplus
}
#endif

#endif
