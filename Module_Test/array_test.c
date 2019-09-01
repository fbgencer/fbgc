#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_math.h"


struct keypair{
    struct fbgc_object * key;
    struct fbgc_object * value;    
};

int main(){

    initialize_fbgc_memory_block();
    
    struct keypair kp;
    struct fbgc_object * k1, * v1;

    struct fbgc_array_object * ao = new_fbgc_array_object(0,sizeof(struct keypair));

    k1 = new_fbgc_cstr_object("fbgencer");
    v1 = new_fbgc_int_object(314151619);
    kp.key = k1; kp.value = v1;
    ao = push_back_fbgc_array_object(ao,&kp);
   

    k1 = new_fbgc_cstr_object("__fbgc__");
    v1 = new_fbgc_double_object(3.14151619);
    kp.key = k1; kp.value = v1;
    ao = push_back_fbgc_array_object(ao,&kp);

    k1 = new_fbgc_cstr_object("xXxX:)");
    v1 = new_fbgc_object(TRUE);
    kp.key = k1; kp.value = v1;
    ao = push_back_fbgc_array_object(ao,&kp);


    k1 = new_fbgc_cstr_object("0123456");
    v1 = new_fbgc_double_object(-273.15);
    kp.key = k1; kp.value = v1;
    ao = push_back_fbgc_array_object(ao,&kp); 

    struct keypair * kk = (get_top_in_fbgc_array_object(ao));

    print_fbgc_array_object(ao);
    cprintf(100,"\nKey : "); print_fbgc_object(kk->key);
    cprintf(100,"\nValue : "); print_fbgc_object(kk->value);
    cprintf(100,"\n");
    
    cprintf(111,"Searchin for a specific object...");
    struct keypair  * sec = get_address_in_fbgc_array_object(ao,0);
    int where = index_fbgc_array_object(ao,sec);
    cprintf(111,"Found at %d\n",where);

    free_fbgc_memory_block();    
    
	return 0;
}


