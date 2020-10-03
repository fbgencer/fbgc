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
#ifndef FBGC_CSTRUCT_OBJECTH
#define FBGC_CSTRUCT_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_cstruct_object{
    struct fbgc_object base;
    const struct fbgc_object_property_holder * properties;
    void * cstruct; // defined in C 
};



#define cast_fbgc_object_as_cstruct(x)( (struct fbgc_cstruct_object *) x)

#define sizeof_fbgc_cstruct_object(x) (sizeof(struct fbgc_cstruct_object) )



struct fbgc_object * new_fbgc_cstruct_object(size_t bsz,const struct fbgc_object_property_holder * );

struct fbgc_object * get_set_fbgc_cstruct_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * rhs);


#ifdef  __cplusplus
}
#endif

#endif
