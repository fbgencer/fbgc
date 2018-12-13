#include <stdio.h>
#include "fbgc_object.h"


int main(){

	struct fbgc_object *db = NULL;
	double nums[] = {5,4,7,2,5};
	for(int i = 0; i<sizeof(nums)/sizeof(double); i++) db = add_fbgc_double_object(db,nums[i]);
	print_fbgc_double_object(db);
	free_fbgc_double_object(db);
	printf("abcdefhijklmnoprstuvyz");

	return 0;
}