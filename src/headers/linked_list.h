#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_ll_base{
    uint8_t type;
    struct fbgc_ll_base * next;
};

struct fbgc_ll{
    struct fbgc_ll_base base;
    struct fbgc_ll_base * tail;
};

struct fbgc_ll_constant{
	struct fbgc_ll_base base;
	struct fbgc_object * content;
};

struct fbgc_ll_jumper{
	struct fbgc_ll_base base;
	struct fbgc_ll_base * content;	
};

struct fbgc_ll_identifier{
	struct fbgc_ll_base base;
    int loc;
    uint8_t flag;
};
#define sizeof_fbgc_ll_identifier(x)(sizeof(struct fbgc_ll_identifier))

#define ID_FLAG_GLOBAL 0x01 //0b00000001
#define ID_FLAG_LOCAL 0x02 //0b00000010
#define ID_FLAG_MEMBER 0x04 //0b00000100
#define ID_FLAG_PUSH_ITSELF 0x10 // x=y=10 like enterance can be solved with this but it could be a problem..

//This flags will be used by the assignment operator!, It uses MSB 4-bit of the flag
#define _cast_fbgc_object_as_llidentifier(x)((struct fbgc_ll_identifier *) x)
#define get_ll_identifier_loc(x)(_cast_fbgc_object_as_llidentifier(x)->loc)
#define set_ll_identifier_loc(x,y)(_cast_fbgc_object_as_llidentifier(x)->loc = y)


#define set_id_flag_GLOBAL(x)(_cast_fbgc_object_as_llidentifier(x)->flag = ID_FLAG_GLOBAL )
#define set_id_flag_LOCAL(x)(_cast_fbgc_object_as_llidentifier(x)->flag = ID_FLAG_LOCAL )
#define set_id_flag_MEMBER(x)(_cast_fbgc_object_as_llidentifier(x)->flag = ID_FLAG_MEMBER )
#define set_id_flag_PUSH_ITSELF(x)(_cast_fbgc_object_as_llidentifier(x)->flag |= ID_FLAG_PUSH_ITSELF )

#define is_id_flag_GLOBAL(x)(_cast_fbgc_object_as_llidentifier(x)->flag & ID_FLAG_GLOBAL )
#define is_id_flag_LOCAL(x)(_cast_fbgc_object_as_llidentifier(x)->flag & ID_FLAG_LOCAL )
#define is_id_flag_MEMBER(x)(_cast_fbgc_object_as_llidentifier(x)->flag & ID_FLAG_MEMBER )
#define is_id_flag_PUSH_ITSELF(x)(_cast_fbgc_object_as_llidentifier(x)->flag & ID_FLAG_PUSH_ITSELF )

#define get_id_flag(x)(_cast_fbgc_object_as_llidentifier(x)->flag)

struct fbgc_ll_base * _new_fbgc_ll_base(fbgc_token token);
struct fbgc_ll_base * _new_fbgc_ll_constant(struct fbgc_object * obj);
struct fbgc_ll_base * _new_fbgc_ll_jumper(fbgc_token tok);
struct fbgc_ll_base * _new_fbgc_ll_identifier(int loc);


#define _cast_llbase_as_llconstant(x)( ((struct fbgc_ll_constant*) x) )
#define _cast_llbase_as_lljumper(x)( ((struct fbgc_ll_jumper*) x) )


#define _cast_llbase_as_ll(x)(((struct fbgc_ll*) x))

struct fbgc_ll_base * _new_fbgc_ll();
void _push_front_fbgc_ll(struct fbgc_ll_base * head,struct fbgc_ll_base * obj);
void _push_back_fbgc_ll(struct fbgc_ll_base * head,struct fbgc_ll_base * obj);
void _insert_next_fbgc_ll(struct fbgc_ll_base * prev,struct fbgc_ll_base * obj);
void _pop_front_fbgc_ll(struct fbgc_ll_base *head);
void _pop_back_fbgc_ll(struct fbgc_ll_base *head);
struct fbgc_ll_base * _top_fbgc_ll(struct fbgc_ll_base * head);

//FAST LIST ACCES DEFINITION IN ORDER TO INCREASE READABILITY
#define POP_LL(head)(head->next = head->next->next)
#define POP_BACK_LL(head)() //do not use it
#define TOP_LL(head)(head->next)
#define BACK_LL(head)(_cast_llbase_as_ll(head)->tail->next)
#define is_empty_fbgc_ll(head)(_cast_llbase_as_ll(head)->tail == head->next)
#define is_not_empty_fbgc_ll(head)(_cast_llbase_as_ll(head)->tail != head->next)

void _print_fbgc_ll(struct fbgc_ll_base * head,const char *s1);


#ifdef  __cplusplus
}
#endif

#endif
