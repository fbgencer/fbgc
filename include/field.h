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
#ifndef FIELD_H
#define FIELD_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_object * new_fbgc_field(uint8_t module_no, ...);
struct fbgc_ll_base * add_variable_in_field(struct fbgc_object * fobj,const char * var_name, struct fbgc_object * rhs);
uint8_t print_field_locals(struct fbgc_object * fobj);
struct fbgc_object * get_fbgc_field_cmodules(struct fbgc_object * o);

#define cast_fbgc_object_as_field(x)(((struct fbgc_class_object*) x))


#ifdef  __cplusplus
}
#endif

#endif