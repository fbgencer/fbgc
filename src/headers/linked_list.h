#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_ll_object{
    struct fbgc_object base;
    struct fbgc_object * tail;
    size_t size;
};

#define cast_fbgc_object_as_ll(x)(((struct fbgc_ll_object*) x))

struct fbgc_object * new_fbgc_ll_object(fbgc_token type);
void free_fbgc_ll_object(struct fbgc_object * );

struct fbgc_object * push_front_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj);
struct fbgc_object * push_back_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj);
struct fbgc_object * insert_next_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * prev,struct fbgc_object * obj);


struct fbgc_object * pop_front_fbgc_ll_object(struct fbgc_object *head);
struct fbgc_object * pop_back_fbgc_ll_object(struct fbgc_object *head);
#define top_fbgc_ll_object(head)(head->next)
#define back_fbgc_ll_object(head)(cast_fbgc_object_as_ll(head)->tail->next)
struct fbgc_object * delete_front_fbgc_ll_object(struct fbgc_object *head);
#define is_empty_fbgc_ll_object(head)(cast_fbgc_object_as_ll(head)->tail == head->next)

void print_fbgc_ll_object(struct fbgc_object * head,const char *s1);


#ifdef  __cplusplus
}
#endif

#endif
