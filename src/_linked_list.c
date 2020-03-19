#include "fbgc.h"


struct fbgc_ll_base * _new_fbgc_ll_base(fbgc_token token){
    struct fbgc_ll_base *o =  (struct fbgc_ll_base*) fbgc_malloc(sizeof(struct fbgc_ll_base));
    o->type = token;
    //o->next = NULL;  
    return (struct fbgc_ll_base*) o;
}


struct fbgc_ll_base * _new_fbgc_ll_constant(struct fbgc_object * obj){
    struct fbgc_ll_constant * o =  (struct fbgc_ll_constant*) fbgc_malloc(sizeof(struct fbgc_ll_constant));
    o->base.type = CONSTANT;
    o->content = obj;
    //o->next = NULL;  
    return (struct fbgc_ll_base *) o;
}

struct fbgc_ll_base * _new_fbgc_ll_jumper(fbgc_token tok){
    struct fbgc_ll_jumper * o =  (struct fbgc_ll_jumper*) fbgc_malloc(sizeof(struct fbgc_ll_jumper));
    o->base.type = tok;
    return (struct fbgc_ll_base *) o;
}



struct fbgc_ll_base * _new_fbgc_ll_identifier(int loc){
    struct fbgc_ll_identifier * o = (struct fbgc_ll_identifier *) fbgc_malloc(sizeof(struct fbgc_ll_identifier));
    o->base.type = IDENTIFIER;
    o->loc = loc;
    o->flag = ID_FLAG_GLOBAL;
    
    return (struct fbgc_ll_base *) o;
}   


struct fbgc_ll_base * _new_fbgc_ll(){
    struct fbgc_ll * llo =  (struct fbgc_ll*) fbgc_malloc(sizeof(struct fbgc_ll));
    llo->base.type = LINKED_LIST;
    llo->tail = _new_fbgc_ll_base(NIL); // make it NIL object so when we ask type of the TOP we won't see NULL(which causes error), we will just see NIL
    llo->tail->next = (struct fbgc_ll_base *) llo;
    llo->base.next = llo->tail;
    return (struct fbgc_ll_base*) llo;
}


void _push_front_fbgc_ll(struct fbgc_ll_base * head,struct fbgc_ll_base * obj){
     //cast head as ll so we can change its content as our list size
    #define head_ll _cast_llbase_as_ll(head)
    
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


void _push_back_fbgc_ll(struct fbgc_ll_base * head,struct fbgc_ll_base * obj){
    //cast head as ll so we can change its content as our list size

    #define head_ll _cast_llbase_as_ll(head)
    assert(head_ll->tail != NULL);
    //[H] -> [O1] <-> [T] 
    head_ll->tail->next->next = obj; //[O1]->[obj] 
    obj->next = head_ll->tail;//[O1]->[obj]->[T]
    head_ll->tail->next = obj;//[O1]->[obj]<->[T]
    #undef head_ll
}

void _insert_next_fbgc_ll(struct fbgc_ll_base * prev,struct fbgc_ll_base * obj){
    //If you are sure about that obj is inside head list use this function
    //but it won't check anything, it could cause memory problems!
    obj->next = prev->next;
    prev->next = obj;
}

void _pop_front_fbgc_ll(struct fbgc_ll_base *head){
    //cast head as ll so we can change its content as our list size
    _cast_llbase_as_ll(head)->base.next = _cast_llbase_as_ll(head)->base.next->next;
}


void _pop_back_fbgc_ll(struct fbgc_ll_base *head){
    //cast head as ll so we can change its content as our list size
    #define head_ll _cast_llbase_as_ll(head)
    
    assert(head_ll->tail != NULL);
    struct fbgc_ll_base * iter = head_ll->base.next;
    while(iter->next != head_ll->tail->next) iter = iter->next;
    iter->next = head_ll->tail;
    head_ll->tail->next = iter;
    #undef head_ll 
}

struct fbgc_ll_base * _top_fbgc_ll(struct fbgc_ll_base * head){
    return head->next;
}


void _print_fbgc_ll(struct fbgc_ll_base * head,const char *s1){

    assert(head != NULL);
    cprintf(111,"{NEW_LL}=");

    struct fbgc_ll * head_ll = _cast_llbase_as_ll(head);
    struct fbgc_ll_base * iter = head_ll->base.next;

    cprintf(101,"[%s]->",s1);
    int i = 0;
    while(iter!=head_ll->tail){
        
        if(iter == NULL){
            cprintf(111,"\n\n\n\nProblem at %d\n\n\n\n",i);
            break;
        }
        i++;

        if(iter->type == CONSTANT){
            struct fbgc_object * obj = _cast_llbase_as_llconstant(iter)->content;
            if(obj->type == INT) cprintf(011,"{%d}",cast_fbgc_object_as_int(obj)->content);   
            else if(obj->type == DOUBLE) cprintf(011,"{%g}",cast_fbgc_object_as_double(obj)->content); 
            else if(obj->type == COMPLEX)
                cprintf(011,"{%g%+gj}",cast_fbgc_object_as_complex(obj)->z.real,cast_fbgc_object_as_complex(obj)->z.imag);
            
            //CHANGE THIS LINE, STRING CASTING MUST BE CHANGED | THIS WAY(&cast_fbgc_object_as_str(iter)->content) IS NOT SAFE!
            else if(obj->type == STRING) cprintf(011,"{'%s'}",&cast_fbgc_object_as_str(obj)->content,object_name_array[obj->type]);
            else if(obj->type == CSTRING){
              cprintf(011,"{");
              print_fbgc_cstr_object(obj);
              cprintf(011,"}");
            }
            else if(obj->type == LOGIC){
                cprintf(011, "{%s}", (cast_fbgc_object_as_logic(obj)->content) ? "true" :"false");
            }
            else if(obj->type == TUPLE) {
              cprintf(011,"{TUPLE}:{");
              print_fbgc_tuple_object(iter);
              cprintf(011,"}");
            }
        }
        else if(iter->type == IDENTIFIER){
            if(is_id_flag_GLOBAL(iter) ) cprintf(011,"%s{G<%d>}","ID",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_LOCAL(iter) ) cprintf(011,"%s{L<%d>}","ID",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_MEMBER(iter) ) cprintf(011,"%s{M<%d>}","ID",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else{
                cprintf(111,"Undefined ID!\n"); 
            }           
        }
        else if(iter->type != ASSIGN_SUBSCRIPT && is_fbgc_ASSIGNMENT_OPERATOR(iter->type)){
            cprintf(011,"{(%s)",get_token_as_str(iter->type));
            if(is_id_flag_GLOBAL(iter) ) cprintf(011,"G<%d>}",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_LOCAL(iter) ) cprintf(011,"L<%d>}",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_MEMBER(iter) ) cprintf(011,"M<%d>}",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else{
                cprintf(111,"Undefined ID!\n"); 
            }           
        }        

        else if(iter->type == IF){
          cprintf(011,"{IF:");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == ELIF){
        cprintf(011,"{ELIF:");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }    
        else if(iter->type == JUMP){
        cprintf(011,"{JUMP -> ");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == WHILE){
        cprintf(011,"{WHILE:");
        if(_cast_llbase_as_lljumper(iter)->content != NULL) print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == FOR_BEGIN){
        cprintf(011,"{FOR_BEGIN}");
          //print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
          //cprintf(011,"}");
        }        
        else if(iter->type == BREAK){
        cprintf(011,"{BREAK -> ");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        } 
        else if(iter->type == CONT){
        cprintf(011,"{CONT -> ");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
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
        else if(iter->type == FUN_CALL){
            cprintf(011,"{FUN_CALL}");
               // cast_fbgc_object_as_int(iter)->content);
        }         
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

