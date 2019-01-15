#ifndef LINKED_LIST_H
#define LINKED_LIST_H


struct fbgc_ll_object{
    struct fbgc_object base;
    struct fbgc_object * tail;
    int size;
};

#define cast_fbgc_object_as_ll(x)(((struct fbgc_ll_object*) x))

struct fbgc_object * new_fbgc_ll_object();
void free_fbgc_ll_object(struct fbgc_object * );

struct
fbgc_object * push_front_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj);
struct
fbgc_object * push_back_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj);
struct
fbgc_object * pop_front_fbgc_ll_object(struct fbgc_object *head);
struct
fbgc_object * pop_back_fbgc_ll_object(struct fbgc_object *head);

#define top_fbgc_ll_object(head)(head.base->next);
#define back_fbgc_ll_object(head)(cast_fbgc_object_as_ll(head)->tail->next);

void print_fbgc_ll_object(struct fbgc_object * head);

void print_fbgc_object_ll(struct fbgc_object *);
void free_fbgc_object_ll(struct fbgc_object *);

void push_fbgc_object_ll(struct fbgc_object ** head,struct fbgc_object ** tail,struct fbgc_object * obj);




#endif
