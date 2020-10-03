// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"


struct fbgc_object * new_fbgc_cstruct_object(size_t bsz, const struct fbgc_object_property_holder * prop){
	struct fbgc_cstruct_object * so =  (struct fbgc_cstruct_object*) fbgc_malloc_object(sizeof(struct fbgc_cstruct_object)+bsz);
    so->base.type = CSTRUCT;
    so->properties = prop;
    so->cstruct = fbgc_malloc(bsz);
    return (struct fbgc_object*) so;
}

struct fbgc_object * get_set_fbgc_cstruct_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * rhs){
	/*struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(o);
	const struct fbgc_cmodule * cm = so->parent;
	for (int i = 0; ; ++i){
		const struct fbgc_cfunction * cc = cm->functions[i];
		if(cc == NULL) break;
		
		if(!my_strcmp(str,cc->name)){
			return new_fbgc_cfun_object(cc->function);
		} 
		
	}*/
	return NULL;
}
