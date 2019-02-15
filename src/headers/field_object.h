#ifndef FIELD_OBJECT_H
#define FIELD_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_field_object{
	struct fbgc_object base;
	struct fbgc_object * head; // main tree list, it's a linked list object
	struct fbgc_object * global_table; // field global symbol table 
	struct fbgc_object * modules;
	//later functions, classes etc will be added.
};

#define cast_fbgc_object_as_field(x)(((struct fbgc_field_object*) x))


struct fbgc_object * new_fbgc_field_object(void);

void load_module_in_field_object(struct fbgc_object * field_obj,const struct fbgc_cmodule * module);
void free_fbgc_field_object(struct fbgc_object * field_obj);


#ifdef  __cplusplus
}
#endif

#endif