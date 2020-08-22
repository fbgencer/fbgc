#include "fbgc.h"

size_t calculate_new_capacity_from_size(size_t size){
    /*
        Below algorithm calculates the capacity for the given size
        Basically capacity is the closest two's power
        0,1 : 1
        2 : 2
        3,4 : 4
        5,6,7,8 : 8
        9,10,11,12,13,14,15 : 16
        and so on

        Take 5 for example, in binary its 0b00101
        take z = 1
        0b0001 <= 0b0101 , shift z left
        0b0010 <= 0b0101 , (ditto)
        0b0100 <= 0b0101 , (ditto)
        0b1000 <= 0b0101 , stop here don't shift, z is 8, the closest two's power for 5
    */
    size_t z = 1;
    while(z < size)
        z <<= 1;

    #ifdef TUPLE_DEBUG
    cprintf(011,"For the size %d new capacity is calculated as :%d\n",size,z);
    #endif
    return z;
}


struct fbgc_object * new_fbgc_tuple_object(size_t cap){
    //here just allocate a space after to->size, we don't need a pointer we know where we are gonna look.

    cap = calculate_new_capacity_from_size(cap);

    struct fbgc_tuple_object *to =  (struct fbgc_tuple_object*) fbgc_malloc_object(sizeof(struct fbgc_tuple_object));
    to->base.type = TUPLE;
    to->size = 0;
    
    to->content = (struct fbgc_object **) fbgc_malloc(sizeof(struct fbgc_object*)*cap);

    return (struct fbgc_object*) to;
}


struct fbgc_object * new_fbgc_tuple_object_from_tuple_content(struct fbgc_object ** src, size_t len){

    struct fbgc_tuple_object * to = (struct fbgc_tuple_object *)new_fbgc_tuple_object(len);
    to->size = len;

    while(len--){
        to->content[len] = src[len];
    }

    return (struct fbgc_object*) to;
}

int handle_tuple_index(struct fbgc_object * self,int index){

    index = (index < 0) * size_fbgc_tuple_object(self) +  index;
    if( index < 0 || index >= size_fbgc_tuple_object(self) )
        return -1;
    return index;
}


struct fbgc_object * set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index){
    
    //Index can be negative, assume that array rotates.
    //in order to get the contents of the array use macro from tuple_object.h
    if((index = handle_tuple_index(self,index))  != -1){
       return content_fbgc_tuple_object(self)[index] = obj;
    }
    else return NULL;
}
struct fbgc_object *  get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index){
    if((index = handle_tuple_index(self,index)) != -1){
        return (struct fbgc_object *) content_fbgc_tuple_object(self)[index];
    }
    else return NULL;
}

//It seems bizarre to have this function but it should have the same function type as other sequential objects
//This function will only be called by the for loop to iterate inside the object

struct fbgc_object *  __get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index,struct fbgc_object * res){
    return get_object_in_fbgc_tuple_object(self,index);
}

struct fbgc_object **  get_object_address_in_fbgc_tuple_object(struct fbgc_object * self,int index){

    index = (index < 0) * size_fbgc_tuple_object(self) +  index;
    if( index < 0 || index >= size_fbgc_tuple_object(self) ) return NULL;
    return content_fbgc_tuple_object(self)+index;   
}

inline struct fbgc_object *  get_back_in_fbgc_tuple_object(struct fbgc_object * self){
    return get_object_in_fbgc_tuple_object(self,-1);
}

inline struct fbgc_object *  get_front_in_fbgc_tuple_object(struct fbgc_object * self){
    return get_object_in_fbgc_tuple_object(self,0);
}



struct fbgc_object * push_back_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj){

    //Check the capacity, if there is enough space push back the obj

    _FBGC_LOGV(TUPLE_OBJECT,"Tuple size :%lu, capacity %lu",size_fbgc_tuple_object(self) , capacity_fbgc_tuple_object(self));

    size_t old_size = size_fbgc_tuple_object(self);

    if(size_fbgc_tuple_object(self) == capacity_fbgc_tuple_object(self)){

        //Before sending to realloc, request a larger block after requesting change the capacity of the tuple
        
    cast_fbgc_object_as_tuple(self)->content = 
                                (struct fbgc_object **)fbgc_realloc(cast_fbgc_object_as_tuple(self)->content,(capacity_fbgc_tuple_object(self)<<1) * sizeof(struct fbgc_object*));

        FBGC_LOGV(TUPLE_OBJECT,"New memory reallocated!\nAfter realloc Tuple size :%lu, capacity %lu",
            size_fbgc_tuple_object(self) , capacity_fbgc_tuple_object(self));
    }
    
    FBGC_LOGV(TUPLE_OBJECT,"There is enough space to push, pushing the object.");

    content_fbgc_tuple_object(self)[old_size++] = obj;
    cast_fbgc_object_as_tuple(self)->size = old_size;
    
    FBGC_LOGV(TUPLE_OBJECT,"New size tuple %lu\n",size_fbgc_tuple_object(self));
    
    return self;
}

int index_fbgc_tuple_object(struct fbgc_object * self, struct fbgc_object * obj){

    for(size_t i = 0; i<size_fbgc_tuple_object(self); i++){
        if(content_fbgc_tuple_object(self)[i] == obj) return i;
    }
    return -1;
}

struct fbgc_object * copy_fbgc_tuple_object(struct fbgc_object * src){

    size_t cap = calculate_new_capacity_from_size( size_fbgc_tuple_object(src) );
    size_t sz = sizeof(struct fbgc_tuple_object) + sizeof(struct fbgc_object*)*cap;

    struct fbgc_tuple_object * dest =  (struct fbgc_tuple_object*) fbgc_malloc_object(sz);
    
    memcpy(dest,src,sz);    

    return (struct fbgc_object *) dest;
}


struct fbgc_object * operator_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
 
    if(b == NULL) return NULL;

switch(op)
{
    case STARSTAR:
    {
        return NULL;
    }
    case SLASHSLASH:
    {
        return NULL;
    }
    case LOEQ:
    {
        return NULL;
    }
    case GREQ:
    {
        return NULL;
    }
    case EQEQ:
    {
        return NULL;
    }
    case NOTEQ:
    {
        return NULL;
    }

    case RSHIFT:
    {   
        //a>>b, a can be anything, b must be tuple.
        if(b->type == TUPLE){
            ;
        }
        return NULL;        
    }
    case LSHIFT:
    {   
        //a<<b, b can be anything, a must be tuple.
        if(a->type == TUPLE){
            struct fbgc_object * t = copy_fbgc_tuple_object(a);
            return push_back_fbgc_tuple_object(t,b);
        }
        return NULL;
    }
    case CARET:
    {
        return NULL;
    }
    case PERCENT:
    {
      //  if(a->type == STRING) return binary_op_fbgc_str_object(a,b,PERCENT);
    }
    case LOWER:
    {
        return NULL;
    }
    case GREATER:
    {
        return NULL;
    }
    case PIPE:
    {
        return NULL;
    }
    case AMPERSAND:
    {
        return NULL;
    }
    case SLASH:
    {
        return NULL;
    }
    case STAR:
    {
        return NULL;
    }
    case MINUS:
    {
        return NULL;
    }
    case PLUS:
    {
        return NULL;
    }
}

    return NULL;
}


uint8_t print_fbgc_tuple_object(struct fbgc_object * obj){
    printf("(");
    for(size_t i = 0; i<size_fbgc_tuple_object(obj); i++){
        print_fbgc_object(content_fbgc_tuple_object(obj)[i]);
        printf(", ");
    }
    return printf(")");
}

struct fbgc_object * subscript_operator_fbgc_tuple_object(struct fbgc_object * iterable,struct fbgc_object * index_obj){
    if(index_obj->type != INT){
        FBGC_LOGE("Index value must be integer");
        return NULL;
    }
    iterable = get_object_in_fbgc_tuple_object(iterable,cast_fbgc_object_as_int(index_obj)->content);

    if(iterable == NULL){
        FBGC_LOGE("Index value out of range");
    }
    
    return iterable;    
}

struct fbgc_object * subscript_assign_operator_fbgc_tuple_object(struct fbgc_object * iterable,struct fbgc_object * index_obj, struct fbgc_object * rhs){
    if(index_obj->type != INT){
        FBGC_LOGE("Index value must be integer");
        return NULL;
    }
    iterable = set_object_in_fbgc_tuple_object(iterable,rhs,cast_fbgc_object_as_int(index_obj)->content);
    if(iterable == NULL){
        FBGC_LOGE("Index value out of range");
        return NULL;    
    }
    return iterable;  
}

struct fbgc_object * abs_operator_fbgc_tuple_object(struct fbgc_object * self){
    return new_fbgc_int_object(size_fbgc_tuple_object(self));
}

struct fbgc_object * find_fbgc_tuple_object(struct fbgc_cfun_arg * arg){
    struct fbgc_tuple_object * self;
    struct fbgc_object * o;
    if(parse_tuple_content(arg,"to",&self,&o) == -1)
        return NULL;

    int sz = self->size;

    while(sz--){
        struct fbgc_object * current_obj = self->content[sz];
        fbgc_token main_tok = MAX(current_obj->type,o->type);
        struct fbgc_object * res = call_fbgc_operator(main_tok,o,current_obj,EQEQ);
        if(res == NULL) continue;
        if(res->type == LOGIC && cast_fbgc_object_as_logic(res)->content)
            return new_fbgc_int_object(sz);
    }


    return NULL;
};


static struct fbgc_object_method _tuple_methods = {
    .len = 1,
    .method = {
        {.name = "find", .function = &find_fbgc_tuple_object},
    }
};


static size_t size_of_fbgc_tuple_object(struct fbgc_object * self){
    return sizeof(struct fbgc_tuple_object);
}

static void gc_mark_fbgc_tuple_object(struct fbgc_object * self){
    fbgc_gc_mark_pointer((cast_fbgc_object_as_tuple(self)->content),capacity_fbgc_tuple_object(self),sizeof(struct fbgc_object *),0);
}


const struct fbgc_object_property_holder fbgc_tuple_object_property_holder = {
    .bits = 
    _BIT_PRINT |
    _BIT_BINARY_OPERATOR | 
    _BIT_SUBSCRIPT_OPERATOR |
    _BIT_SUBSCRIPT_ASSIGN_OPERATOR |
    _BIT_ABS_OPERATOR | 
    _BIT_METHODS | 
    _BIT_SIZE_OF |
    _BIT_GC_MARK
    ,
    .properties ={
        {.print = &print_fbgc_tuple_object},
        {.methods = &_tuple_methods},
        {.binary_operator = &operator_fbgc_tuple_object},
        {.subscript_operator = &subscript_operator_fbgc_tuple_object},
        {.subscript_assign_operator = &subscript_assign_operator_fbgc_tuple_object},
        {.abs_operator = &abs_operator_fbgc_tuple_object},
        {.gc_mark = &gc_mark_fbgc_tuple_object},
        {.size_of = &size_of_fbgc_tuple_object},
    }
};
