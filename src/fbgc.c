#include "fbgc.h"


int main(){
	printf("offf\n");
	struct fbgc_object *db = NULL;
	double nums[] = {5,4,7,2,5};
	for(unsigned int i = 0; i<sizeof(nums)/sizeof(double); i++) 
		db = add_fbgc_double_object(db,nums[i]);
	print_fbgc_double_object(db);
	free_fbgc_double_object(db);
	printf("FBGENCER\n");

	return 0;
}