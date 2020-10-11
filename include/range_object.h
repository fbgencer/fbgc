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
#ifndef RANGE_OBJECT_H
#define RANGE_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_range_object{ 
	struct fbgc_object base;
	struct fbgc_object * start;
	struct fbgc_object * end;
	struct fbgc_object * step;
};

extern const struct fbgc_object_property_holder fbgc_range_object_property_holder;

#define cast_fbgc_object_as_range(x)(((struct fbgc_range_object*) x))
#define sizeof_fbgc_range_object() (sizeof(struct fbgc_range_object)) 



struct fbgc_object * new_fbgc_range_object(struct fbgc_object * s,struct fbgc_object * e);
struct fbgc_object * get_element_in_fbgc_range_object(struct fbgc_object * robj,int index,struct fbgc_object * result);
struct fbgc_object * get_int_element_in_fbgc_range_object(struct fbgc_object * robj,int index,struct fbgc_object * result);
struct fbgc_object * get_double_element_in_fbgc_range_object(struct fbgc_object * robj,int index,struct fbgc_object * result);

#define get_fbgc_range_object_iter_type(x)\
(MAX(MAX(cast_fbgc_object_as_range(x)->start->type,cast_fbgc_object_as_range(x)->step->type),cast_fbgc_object_as_range(x)->end->type)) 

uint8_t print_fbgc_range_object(struct fbgc_object * obj);

#ifdef  __cplusplus
}
#endif

#endif