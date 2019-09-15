#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_ll_object{
    struct fbgc_object base;
    struct fbgc_object * tail;
};

#define cast_fbgc_object_as_ll(x)(((struct fbgc_ll_object*) x))

struct fbgc_object * new_fbgc_ll_object();


void push_front_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj);
void push_back_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj);
struct fbgc_object * insert_next_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * prev,struct fbgc_object * obj);


void pop_front_fbgc_ll_object(struct fbgc_object *head);
void pop_back_fbgc_ll_object(struct fbgc_object *head);
void delete_front_fbgc_ll_object(struct fbgc_object *head);
struct fbgc_object * top_fbgc_ll_object(struct fbgc_object * head);

//FAST LIST ACCES DEFINITION IN ORDER TO INCREASE READABILITY
#define POP_LL(head)(head->next = head->next->next)
#define POP_BACK_LL(head)() //do not use it
#define TOP_LL(head)(head->next)
#define BACK_LL(head)(cast_fbgc_object_as_ll(head)->tail->next)


#define is_empty_fbgc_ll_object(head)(cast_fbgc_object_as_ll(head)->tail == head->next)

void print_fbgc_ll_object(struct fbgc_object * head,const char *s1);


#ifdef  __cplusplus
}
#endif

#endif
