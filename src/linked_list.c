#include "fbgc.h"


const char * lltp2str(struct fbgc_ll_base * ll){
    return object_name_array[ll->type];
}

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


struct fbgc_ll_base * _new_fbgc_ll_jumper_with_content(fbgc_token tok,struct fbgc_ll_base * ct ){
    struct fbgc_ll_jumper * o =  (struct fbgc_ll_jumper*) fbgc_malloc(sizeof(struct fbgc_ll_jumper));
    o->base.type = tok;
    o->content = ct;
    return (struct fbgc_ll_base *) o;
}


struct fbgc_ll_base * _new_fbgc_ll_jumper(fbgc_token tok){
    return _new_fbgc_ll_jumper_with_content(tok,NULL);
}


struct fbgc_ll_base * _new_fbgc_ll_identifier(int loc){
    struct fbgc_ll_identifier * o = (struct fbgc_ll_identifier *) fbgc_malloc(sizeof(struct fbgc_ll_identifier));
    o->base.type = IDENTIFIER;
    o->loc = loc;
    o->flag = ID_FLAG_GLOBAL;
    
    return (struct fbgc_ll_base *) o;
}   

struct fbgc_ll_base * _new_fbgc_ll_opcode_int(fbgc_token t, int x){
    struct fbgc_ll_opcode_int * o = (struct fbgc_ll_opcode_int *) fbgc_malloc(sizeof(struct fbgc_ll_opcode_int));
    o->base.type = t;
    o->content = x;
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

struct fbgc_ll_base  * _insert_next_fbgc_ll(struct fbgc_ll_base * prev,struct fbgc_ll_base * obj){
    //If you are sure about that obj is inside the ist use this function
    obj->next = prev->next;
    prev->next = obj;
    return obj; //Now return current prev object if iterator wants to continue from the list
}

struct fbgc_ll_base  * _insert_fbgc_ll(struct fbgc_ll_base * prev,struct fbgc_ll_base * current,struct fbgc_ll_base * obj){
    //Notice that if prev->next is current we lost this object
    obj->next = current->next;
    prev->next = obj;
    return obj; //Now return current prev object if iterator wants to continue from the list
}


void _pop_front_fbgc_ll(struct fbgc_ll_base *head){
    //cast head as ll so we can change its content as our list size
    _cast_llbase_as_ll(head)->base.next = _cast_llbase_as_ll(head)->base.next->next;
}


struct fbgc_ll_base * _top_and_pop_front_fbgc_ll(struct fbgc_ll_base * head){
    //cast head as ll so we can change its content as our list size
    struct fbgc_ll_base * top = head->next;
    head->next = head->next->next;
    return top;
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

void _print_fbgc_ll_base(struct fbgc_ll_base * lb){
    if(lb->type == CONSTANT){
        struct fbgc_object * obj = _cast_llbase_as_llconstant(lb)->content;
        print_fbgc_object(obj);
    }
    else cprintf(111,"%s",lltp2str(lb));
}


uint8_t _print_fbgc_ll(struct fbgc_ll_base * head,const char *s1){

    assert(head != NULL);
    struct fbgc_ll * head_ll = _cast_llbase_as_ll(head);
     cprintf(101,"[%s]->",s1);
    _print_fbgc_ll_code(head_ll->base.next,head_ll->tail);
    cprintf(101,"<->[]\n");
    return 1;
}



uint8_t _print_fbgc_ll_code(struct fbgc_ll_base * head_next,struct fbgc_ll_base * tail){

    struct fbgc_ll_base * iter = head_next;
    int i = 0;
    while(iter!=tail){
        
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
            
            else if(obj->type == STRING) cprintf(011,"{'%s'}",content_fbgc_str_object(obj));
            else if(obj->type == LOGIC){
                cprintf(011, "{%s}", (cast_fbgc_object_as_logic(obj)->content) ? "true" :"false");
            }
            else if(obj->type == TUPLE) {
              cprintf(011,"{TUPLE}:{");
              print_fbgc_tuple_object(obj);
              cprintf(011,"}");
            }
            else if(obj->type == FUN){
                cprintf(011,"{FUN}:{");
                print_fbgc_fun_object(obj);
                 cprintf(011,"}");   
            }
            //Do not print CLASS types because they have themselves in their code so it causes infinite loop
            else cprintf(011,"{%s}",object_name_array[obj->type]);  
        }
        else if(iter->type == IDENTIFIER){
            if(is_id_flag_GLOBAL(iter) ) cprintf(011,"%s{G<%d>}","ID",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_LOCAL(iter) ) cprintf(011,"%s{L<%d>}","ID",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_MEMBER(iter) ) cprintf(011,"%s{M<%d>}","ID",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_CLASS(iter) ) cprintf(011,"%s{C<%d>}","ID",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else{
                cprintf(111,"Undefined ID! %d\n"); 
            }           
        }
        else if(iter->type != ASSIGN_SUBSCRIPT && is_fbgc_ASSIGNMENT_OPERATOR(iter->type)){
            cprintf(011,"{(%s)",get_token_as_str(iter->type));
            if(is_id_flag_GLOBAL(iter) ) cprintf(011,"G<%d>}",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_LOCAL(iter) ) cprintf(011,"L<%d>}",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_MEMBER(iter) ) cprintf(011,"M<%d>}",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else if(is_id_flag_CLASS(iter) ) cprintf(011,"C<%d>}",_cast_fbgc_object_as_llidentifier(iter)->loc);
            else{
                cprintf(111,"Undefined ID!\n"); 
            }           
        }        

        else if(iter->type == IF){
          cprintf(011,"{IF:");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) _print_fbgc_ll_base(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == ELIF){
        cprintf(011,"{ELIF:");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) _print_fbgc_ll_base(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }    
        else if(iter->type == JUMP){
        cprintf(011,"{JUMP -> ");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) _print_fbgc_ll_base(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }
        else if(iter->type == WHILE){
            
            cprintf(011,"{WHILE:");
            if(_cast_llbase_as_lljumper(iter)->content != NULL) _print_fbgc_ll_base(_cast_llbase_as_lljumper(iter)->content->next);
            cprintf(011,"}");
        }
        else if(iter->type == FOR_BEGIN){
        cprintf(011,"{FOR_BEGIN}");
          //print_fbgc_object(_cast_llbase_as_lljumper(iter)->content->next);
          //cprintf(011,"}");
        }        
        else if(iter->type == BREAK){
        cprintf(011,"{BREAK -> ");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) _print_fbgc_ll_base(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        } 
        else if(iter->type == CONT){
        cprintf(011,"{CONT -> ");
          if(_cast_llbase_as_lljumper(iter)->content != NULL) _print_fbgc_ll_base(_cast_llbase_as_lljumper(iter)->content->next);
          cprintf(011,"}");
        }                  

        else if(iter->type == BUILD_TUPLE){
            cprintf(011,"{BUILD_TUPLE(%d)}",_cast_llbase_as_llopcode_int(iter)->content);
        }
        else if(iter->type == BUILD_MATRIX){
            cprintf(011,"{BUILD_MATRIX(%d)}",_cast_llbase_as_llopcode_int(iter)->content);
        }
        else if(iter->type == BUILD_MAP){
            cprintf(011,"{BUILD_MAP(%d)}",_cast_llbase_as_llopcode_int(iter)->content);
        }        
        else if(iter->type == FUN_CALL){
            cprintf(011,"{FUN_CALL(%d)}",_cast_llbase_as_llopcode_int(iter)->content);
        }
        else if(iter->type == CLASS_CALL){
            cprintf(011,"{CLASS_CALL(%d)}",_cast_llbase_as_llopcode_int(iter)->content);
        }        
        else if(iter->type == LOAD_SUBSCRIPT){
            cprintf(011,"{LOAD_SUBSCRIPT(%d)}",_cast_llbase_as_llopcode_int(iter)->content);   
        }
        else if(iter->type == ROW){
            cprintf(011,"{ROW(%d)}",_cast_llbase_as_llopcode_int(iter)->content);
        }                                    
        else if(is_fbgc_OPERATOR(iter->type)) cprintf(011,"{%s}",get_token_as_str(iter->type));

        else cprintf(011,"{%s}",lltp2str(iter));     
            iter = iter->next;
    }
    
    return 1;
}

