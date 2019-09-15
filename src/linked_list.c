#include "fbgc.h"

 
struct
fbgc_object * new_fbgc_ll_object(){
    struct fbgc_ll_object *llo =  (struct fbgc_ll_object*) fbgc_malloc(sizeof(struct fbgc_ll_object));
    llo->base.type = LINKED_LIST;
    llo->tail = new_fbgc_object(UNKNOWN); // make it new ll object but we can't iterate from back so its just a null object
    llo->tail->next = (struct fbgc_object *) llo;
    llo->base.next = llo->tail;
    return (struct fbgc_object*) llo;
}


void push_front_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj){
     //cast head as ll so we can change its content as our list size
    #define head_ll cast_fbgc_object_as_ll(head)
    
    //connect the list
    //[H] <-> [T] 
    obj->next = head_ll->base.next;// [obj]->[T] and 
    //[H]->[T] head next and obj next shows previous top object
    head_ll->base.next = obj; // Now [H]->[obj]->[T]
    
    //This gives the opportunity that we can connect linked list with O(1) complexity from back
    //See the pushback function
    if(head_ll->tail != NULL) 
        head_ll->tail->next = obj; // Now [H]->[obj]<->[T] connect the tail just for one time

    #undef head_ll
}


void push_back_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj){
    //cast head as ll so we can change its content as our list size

    #define head_ll cast_fbgc_object_as_ll(head)
    assert(head_ll->tail != NULL);
    //[H] -> [O1] <-> [T] 
    head_ll->tail->next->next = obj; //[O1]->[obj] 
    obj->next = head_ll->tail;//[O1]->[obj]->[T]
    head_ll->tail->next = obj;//[O1]->[obj]<->[T]

    #undef head_ll
}

struct
fbgc_object * insert_next_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * prev,struct fbgc_object * obj){
    //If you are sure about that obj is inside head list use this function
    //but it won't check anything, it could cause memory problems!
    #define head_ll cast_fbgc_object_as_ll(head)
    
    obj->next = prev->next;
    prev->next = obj;
    return (struct fbgc_object *) head_ll;
    #undef head_ll
}

void pop_front_fbgc_ll_object(struct fbgc_object *head){
    //cast head as ll so we can change its content as our list size
    #define head_ll cast_fbgc_object_as_ll(head)
    
    head_ll->base.next = head_ll->base.next->next;
    
    #undef head_ll
}

void delete_front_fbgc_ll_object(struct fbgc_object *head){
    //cast head as ll so we can change its content as our list size
    //xXxxXxXxXXXXXXXXXXX
    //Change this function, it seems unnecessary
    //XXXXXXXXXXXXXXXXXXX
    #define head_ll cast_fbgc_object_as_ll(head)
    
    //struct fbgc_object * temp = head_ll->base.next;
    head_ll->base.next =  head_ll->base.next->next;
    #undef head_ll

}


void pop_back_fbgc_ll_object(struct fbgc_object *head){
    //cast head as ll so we can change its content as our list size
    #define head_ll cast_fbgc_object_as_ll(head)
    
    assert(head_ll->tail != NULL);

    struct fbgc_object * iter = head_ll->base.next;
    while(iter->next != head_ll->tail->next) iter = iter->next;
    iter->next = head_ll->tail;
    head_ll->tail->next = iter;
    
    #undef head_ll 
}

struct fbgc_object * top_fbgc_ll_object(struct fbgc_object * head){
    return head->next;
}


void print_fbgc_ll_object(struct fbgc_object * head,const char *s1){

    assert(head != NULL);

    struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
    struct fbgc_object * iter = head_ll->base.next;

    cprintf(101,"[%s]->",s1);
    int i = 0;
    while(iter!=head_ll->tail){
        
        if(iter == NULL){
            cprintf(111,"\n\n\n\nProblem at %d\n\n\n\n",i);
            break;
        }
        i++;



        if(iter->type == INT) cprintf(011,"{%d}",cast_fbgc_object_as_int(iter)->content);   
        else if(iter->type == DOUBLE) cprintf(011,"{%f}",cast_fbgc_object_as_double(iter)->content); 
        else if(iter->type == COMPLEX)
            cprintf(011,"{%f %+fj}",cast_fbgc_object_as_complex(iter)->z.real,cast_fbgc_object_as_complex(iter)->z.imag);
        
        //CHANGE THIS LINE, STRING CASTING MUST BE CHANGED | THIS WAY(&cast_fbgc_object_as_str(iter)->content) IS NOT SAFE!
        else if(iter->type == STRING) cprintf(011,"{'%s'}",&cast_fbgc_object_as_str(iter)->content,object_name_array[iter->type]);
        else if(iter->type == CSTRING){
          cprintf(011,"{");
          print_fbgc_cstr_object(iter);
          cprintf(011,"}");
        }
        else if(iter->type == IDENTIFIER){
            if(is_id_flag_GLOBAL(iter) ) cprintf(011,"%s{G<%d>}","ID",cast_fbgc_object_as_id_opcode(iter)->loc);
            else if(is_id_flag_LOCAL(iter) ) cprintf(011,"%s{L<%d>}","ID",cast_fbgc_object_as_id_opcode(iter)->loc);
            else if(is_id_flag_SUBSCRIPT(iter) ) cprintf(011,"%s{S<%d>}","ID",cast_fbgc_object_as_id_opcode(iter)->loc);
            else{
                cprintf(111,"Undefined ID!\n"); 
            }           
        }
        else if(iter->type == TUPLE) {
          cprintf(011,"{TUPLE}:{");
          print_fbgc_tuple_object(iter);
          cprintf(011,"}");
        }
        else if(iter->type == IF_BEGIN){
        cprintf(011,"{IF_BEGIN -> ");
          print_fbgc_object(cast_fbgc_object_as_jumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == ELIF_BEGIN){
        cprintf(011,"{ELIF_BEGIN -> ");
          print_fbgc_object(cast_fbgc_object_as_jumper(iter)->content->next);
          cprintf(011,"}");
        }    
        else if(iter->type == JUMP){
        cprintf(011,"{JUMP -> ");
          print_fbgc_object(cast_fbgc_object_as_jumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == WHILE_BEGIN){
        cprintf(011,"{WHILE_BEGIN -> ");
          print_fbgc_object(cast_fbgc_object_as_jumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == FOR_BEGIN){
        cprintf(011,"{FOR_BEGIN -> ");
          print_fbgc_object(cast_fbgc_object_as_jumper(iter)->content->next);
          cprintf(011,"}");
        }        
        else if(iter->type == BREAK){
        cprintf(011,"{BREAK -> ");
          print_fbgc_object(cast_fbgc_object_as_jumper(iter)->content);
          cprintf(011,"}");
        } 
        else if(iter->type == CONT){
        cprintf(011,"{CONT -> ");
          print_fbgc_object(cast_fbgc_object_as_jumper(iter)->content);
          cprintf(011,"}");
        }                  
        else if(iter->type == FUN){
            cprintf(011,"FUN:[");
            print_fbgc_object(iter);
            cprintf(011,"]");
        }

        else if(iter->type == BUILD_TUPLE){
            cprintf(011,"{BUILD_TUPLE(%d)}",
                cast_fbgc_object_as_int(iter)->content);
        }
        else if(iter->type == BUILD_MATRIX){
            cprintf(011,"{BUILD_MATRIX(%d)}",
                cast_fbgc_object_as_int(iter)->content);
        }
       /* else if(iter->type == FUN_CALL){
            cprintf(011,"{FUN_CALL(%d)}",
                cast_fbgc_object_as_int(iter)->content);
        }         */
        else if(iter->type == ROW){
            cprintf(011,"{ROW(%d)}",
                cast_fbgc_object_as_int(iter)->content);
        }                                    
        else if(is_fbgc_OPERATOR(iter->type)) cprintf(011,"{%s}",get_token_as_str(iter->type));

        else cprintf(011,"{%s}",object_name_array[iter->type]);     
            iter = iter->next;
    }
    if(head_ll->base.type == LINKED_LIST) cprintf(101,"<->[T]\n");
    else cprintf(101,"->NULL\n");

}

