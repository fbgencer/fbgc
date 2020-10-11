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
#ifndef FBGC_TUPLE_OBJECTH
#define FBGC_TUPLE_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_tuple_object{
    struct fbgc_object base;
    size_t size;
    struct fbgc_object ** content;
};
extern const struct fbgc_object_property_holder fbgc_tuple_object_property_holder;

#define sizeof_fbgc_tuple_object(x)(sizeof(struct fbgc_tuple_object))

#define cast_fbgc_object_as_tuple(x)(((struct fbgc_tuple_object*) x))
#define size_fbgc_tuple_object(x)(cast_fbgc_object_as_tuple(x)->size)
#define capacity_fbgc_tuple_object(x)(capacity_fbgc_raw_memory(cast_fbgc_object_as_tuple(x)->content,sizeof(struct fbgc_object *) ))
#define content_fbgc_tuple_object(x)(cast_fbgc_object_as_tuple(x)->content)





struct fbgc_object * new_fbgc_tuple_object(size_t size);
struct fbgc_object * new_fbgc_tuple_object_from_tuple_content(struct fbgc_object ** src, size_t len);

struct fbgc_object * set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index);
struct fbgc_object * get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index);
struct fbgc_object ** get_object_address_in_fbgc_tuple_object(struct fbgc_object * self,int index);
struct fbgc_object *  get_back_in_fbgc_tuple_object(struct fbgc_object * self);
struct fbgc_object *  get_front_in_fbgc_tuple_object(struct fbgc_object * self);
struct fbgc_object * push_back_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj);
int index_fbgc_tuple_object(struct fbgc_object * self, struct fbgc_object * obj);





void _push_back_fbgc_tuple_object(struct fbgc_object ** tp,struct fbgc_object * obj);

struct fbgc_object *  __get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index,struct fbgc_object * res);

struct fbgc_object * operator_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

uint8_t print_fbgc_tuple_object(struct fbgc_object *);
void free_fbgc_tuple_object(struct fbgc_object * );


struct fbgc_object * add_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b);


/*! \cond DOCUMENT_EVERYTHING
	Lets not document this part, just log definitions nothing important 
*/
#ifdef LOG_TUPLE_OBJECT
#define TUPLE_OBJECT_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define TUPLE_OBJECT_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _TUPLE_OBJECT_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _TUPLE_OBJECT_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define TUPLE_OBJECT_LOGV(...)
#define TUPLE_OBJECT_LOGD(...)
#define _TUPLE_OBJECT_LOGV(...)
#define _TUPLE_OBJECT_LOGD(...)
#endif

#ifdef  __cplusplus
}
#endif

#endif
