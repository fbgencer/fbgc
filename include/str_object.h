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
#ifndef FBGC_STR_OBJECTH
#define FBGC_STR_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_str_object{
    struct fbgc_object base;
    size_t len;
    char content[0];
};
extern const struct fbgc_object_property_holder fbgc_str_object_property_holder;


#define cast_fbgc_object_as_str(x)(((struct fbgc_str_object*) x))


#define length_fbgc_str_object(x)(cast_fbgc_object_as_str(x)->len)
#define content_fbgc_str_object(x)(cast_fbgc_object_as_str(x)->content)

struct fbgc_object * new_fbgc_str_object(const char *str);
struct fbgc_object * new_fbgc_str_object_from_substr(const char *str1,const char *str2);
struct fbgc_object * new_fbgc_str_object_empty(size_t len);
struct fbgc_object * new_fbgc_str_object_from_object(struct fbgc_object * obj);


struct fbgc_object * operator_fbgc_str_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

struct fbgc_object * get_char_from_fbgc_str_object(struct fbgc_object * so,int i1,struct fbgc_object * res);
struct fbgc_object * get_object_in_fbgc_str_object(struct fbgc_object * obj,int i1, int i2);
struct fbgc_object * set_object_in_fbgc_str_object(struct fbgc_object * obj,int i1, int i2,struct fbgc_object *o);

uint8_t print_fbgc_str_object(struct fbgc_object *);
void free_fbgc_str_object(struct fbgc_object * );




uint8_t my_strncmp(const char *s1, const char *s2, register size_t n);
uint8_t my_strcmp(const char *p1, const char *p2);



#ifdef  __cplusplus
}
#endif

#endif
