#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_object{
    fbgc_token type;
    struct fbgc_object * next;
};


#define is_object_null(obj)(obj == NULL)
#define get_fbgc_object_type(obj)( (obj->type & 0x7F))

//This only for debug case!
#define object_type_as_str(obj)(obj != NULL ? object_name_array[get_fbgc_object_type(obj)] : "NULL")

#define cast_fbgc_object_as_if(x) (((struct fbgc_ref_object*) x))

struct fbgc_object * new_fbgc_object(fbgc_token);
size_t get_fbgc_object_size(struct fbgc_object * );

int convert_fbgc_object_to_int(struct fbgc_object * );
double convert_fbgc_object_to_double(struct fbgc_object * );
char * convert_fbgc_object_to_string(struct fbgc_object * );


struct fbgc_object * get_length_fbgc_object(struct fbgc_object * t);

void claim_ownership(struct fbgc_object * self);
void print_fbgc_object(struct fbgc_object *);
void printf_fbgc_object(struct fbgc_object * self);
void free_fbgc_object(struct fbgc_object *);

#ifdef  __cplusplus
}
#endif

#endif