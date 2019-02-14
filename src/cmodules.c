#include "fbgc.h"


struct fbgc_object * get_cfun_obj_from_substr(struct fbgc_object * field_obj,const char *str1,const char *str2){
	
	for(int i = 0; i<2; i++){
		if(!memcmp(str1,fbgc_io_module.functions[i]->name,str2-str1) ) 
			return new_fbgc_cfun_object(fbgc_io_module.functions[i]->function);
	}
	return NULL;	

}