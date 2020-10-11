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
#ifndef CMODULE_OBJECT_H
#define CMODULE_OBJECT_H

/*
	module name : math
	module functions : sin,cos,tan..
	module constants : pi, e,
*/

#ifdef __cplusplus
extern "C" {
#endif



//C module holds CONSTANT array of c functions, this array is (pre)determined during the compilation time


struct fbgc_cmodule{
    const struct fbgc_cfunction initializer;
    const struct fbgc_cfunction functions[];
};

//This is where c functions and modules interact with each other
//This is our object to hold in fields
struct fbgc_cmodule_object{
    struct fbgc_object base;
    const struct fbgc_object_property_holder * properties;
};

#define sizeof_fbgc_cmodule_object()(sizeof(struct fbgc_cmodule_object))

struct fbgc_cfun_object{
	struct fbgc_object base;
	struct fbgc_object * (* function)(struct fbgc_cfun_arg * );
};

extern const struct fbgc_object_property_holder fbgc_cfun_object_property_holder;

#define sizeof_fbgc_cfun_object()(sizeof(struct fbgc_cfun_object))


#define cast_fbgc_object_as_cfun(x)(((struct fbgc_cfun_object*) x))
#define cast_fbgc_object_as_cmodule(x)(((struct fbgc_cmodule_object*) x))



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_cfun_arg * ) );
struct fbgc_object * new_fbgc_cmodule_object(const struct fbgc_object_property_holder * p);
void print_fbgc_cmodule(const struct fbgc_cmodule * );
void free_fbgc_cfun_object(struct fbgc_object * obj);
struct fbgc_object * get_set_fbgc_cmodule_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * rhs);


struct fbgc_object * new_cfun_object_from_str(struct fbgc_object * field_obj,const char * str);


#ifdef  __cplusplus
}
#endif

#endif