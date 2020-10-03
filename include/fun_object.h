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
#ifndef FUN_OBJECT_H
#define FUN_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_fun_object{ 
	struct fbgc_object base;
	struct fbgc_ll_base * code; //function linked list code
	uint8_t no_locals;
	uint8_t no_arg : 5;
	uint8_t flag_variadic : 1;
	uint8_t flag_default_args : 1;
	uint8_t flag_kwargs : 1;
};
//We give 6bits for no_arg so max number is 
#define is_fbgc_fun_object_defined(x)(cast_fbgc_object_as_fun(x)->no_arg != 0b00011111)

extern const struct fbgc_object_property_holder fbgc_fun_object_property_holder;

#define cast_fbgc_object_as_fun(x)(((struct fbgc_fun_object*) x))
#define sizeof_fbgc_fun_object()(sizeof(struct fbgc_fun_object)) 



uint8_t print_fbgc_fun_object(struct fbgc_object *);
void fprintf_fbgc_fun_object(struct fbgc_object *);

struct fbgc_object * new_fbgc_fun_object();


#ifdef  __cplusplus
}
#endif

#endif