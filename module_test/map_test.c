#include "fbgc.h"
void map_example1();

void map_test(){
	map_example1();
}


void map_example1(){
	printf("sz:%lu\n",sizeof(struct fbgc_map_object));


	struct fbgc_object * m = new_fbgc_map_object(2);

	fbgc_map_object_insert(&m,new_fbgc_str_object("abi"),new_fbgc_int_object(10));
	fbgc_map_object_insert(&m,new_fbgc_str_object("basar"),new_fbgc_int_object(20));
	fbgc_map_object_insert(&m,new_fbgc_str_object("ceviz"),new_fbgc_int_object(30));
	
	fbgc_map_object_insert(&m,new_fbgc_str_object("denizli"),new_fbgc_int_object(40));
	fbgc_map_object_insert(&m,new_fbgc_str_object("edirne"),new_fbgc_int_object(50));
	
	fbgc_map_object_insert(&m,new_fbgc_str_object("fikretbasargencer"),new_fbgc_int_object(60));
	fbgc_map_object_insert(&m,new_fbgc_str_object("genlikbirimi"),new_fbgc_int_object(70));
	fbgc_map_object_remove(&m,new_fbgc_str_object("genlikbirimi"));

	printf_fbgc_object(m);

	printf("\n");

}