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
#ifndef FBGC_INT_OBJECTH
#define FBGC_INT_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif
/*! fbgc int object derived from base class and hold one int variable */

struct fbgc_int_object{
    struct fbgc_object base; /**< fbgc base structure */
    int content; /**< fbgc_int object holds integer value in this variable */
};

extern const struct fbgc_object_property_holder fbgc_int_object_property_holder;


#define cast_fbgc_object_as_int(x)(((struct fbgc_int_object*) x))
#define sizeof_fbgc_int_object(x)(sizeof(struct fbgc_int_object))


struct fbgc_object * new_fbgc_int_object(int x);
struct fbgc_object * new_fbgc_int_object_from_str(const char *);
struct fbgc_object * new_fbgc_int_object_from_substr(const char *,const char *,fbgc_token );


uint8_t print_fbgc_int_object(struct fbgc_object *);
struct fbgc_object * fbgc_int_object_to_str(struct fbgc_object * obj);


void free_fbgc_int_object(struct fbgc_object * obj);


#ifdef  __cplusplus
}
#endif

#endif