#ifndef FBGC_LOGIC_OBJECTH
#define FBGC_LOGIC_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_logic_object{
    struct fbgc_object base;
    char content;
};
extern const struct fbgc_object_property_holder fbgc_logic_object_property_holder;

#define cast_fbgc_object_as_logic(x)(((struct fbgc_logic_object*) x))
#define sizeof_fbgc_logic_object(x)(sizeof(struct fbgc_logic_object))


struct fbgc_object * new_fbgc_logic_object(int);

struct fbgc_object * operator_fbgc_logic_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

uint8_t print_fbgc_logic_object(struct fbgc_object * obj);
void free_fbgc_logic_object(struct fbgc_object * );
struct fbgc_object * fbgc_logic_object_to_str(struct fbgc_object * obj);




#ifdef  __cplusplus
}
#endif

#endif