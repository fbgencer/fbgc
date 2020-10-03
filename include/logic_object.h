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
#ifndef FBGC_LOGIC_OBJECTH
#define FBGC_LOGIC_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_logic_object{
    struct fbgc_object base;
    uint8_t content:1;
};

extern struct fbgc_logic_object fbgc_logic_true;
extern struct fbgc_logic_object fbgc_logic_false;


extern const struct fbgc_object_property_holder fbgc_logic_object_property_holder;

#define cast_fbgc_object_as_logic(x)(((struct fbgc_logic_object*) x))
#define sizeof_fbgc_logic_object(x)(sizeof(struct fbgc_logic_object))


struct fbgc_object * new_fbgc_logic_object(int);

struct fbgc_object * operator_fbgc_logic_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

uint8_t print_fbgc_logic_object(struct fbgc_object * obj);
void free_fbgc_logic_object(struct fbgc_object * );
struct fbgc_object * fbgc_logic_object_to_str(struct fbgc_object * obj);




#ifdef  __cplusplus
}
#endif

#endif