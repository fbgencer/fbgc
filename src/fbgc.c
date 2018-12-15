#include "fbgc.h"


int main(){
	

	struct fbgc_object *db1 = new_fbgc_double_object(3.123);
	struct fbgc_object *db2 = new_fbgc_double_object_from_str("1.123e3");
	
	connect_fbgc_objects(db1,db2);
	print_fbgc_object_ll(db1);

	free_fbgc_double_object(db1);
	free_fbgc_double_object(db2);

	return 0;
}