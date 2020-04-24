#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_object{
    fbgc_token type;
};




#define is_object_null(obj)(obj == NULL)
#define get_fbgc_object_type(obj)( (obj->type & 0x7F))

#define fbgc_object_max(o1,o2)(get_fbgc_object_type(o1)>get_fbgc_object_type(o2) ? get_fbgc_object_type(o1):\
get_fbgc_object_type(o2))

#define MAX(a,b)(a>b?a:b)
#define MIN(a,b)(a<b?a:b)


//This only for debug case!
#define object_type_as_str(obj)(obj != NULL ? object_name_array[get_fbgc_object_type(obj)] : "NULL")

#define sizeof_fbgc_object()(sizeof(struct fbgc_object))

struct fbgc_object * new_fbgc_object(fbgc_token);
size_t get_fbgc_object_size(struct fbgc_object * );



char convert_fbgc_object_to_logic(struct fbgc_object * );
int convert_fbgc_object_to_int(struct fbgc_object * );
double convert_fbgc_object_to_double(struct fbgc_object * );
struct raw_complex convert_fbgc_object_to_complex(struct fbgc_object * obj);

struct fbgc_object * get_set_fbgc_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm);
struct fbgc_object ** get_address_fbgc_object_member(struct fbgc_object * o, const char * str);

struct fbgc_object * subscript_operator_fbgc_object(struct fbgc_object * iterable,struct fbgc_object * index_obj);
struct fbgc_object * subscript_assign_operator_fbgc_object(struct fbgc_object * iterable,struct fbgc_object * index_obj,struct fbgc_object * rhs);


struct fbgc_object * abs_operator_fbgc_object(struct fbgc_object * self);

uint8_t print_fbgc_object(struct fbgc_object *);
uint8_t myprint_fbgc_object(struct fbgc_object *);
void printf_fbgc_object(struct fbgc_object * self);
void free_fbgc_object(struct fbgc_object *);



uint8_t _is_property(uint32_t bit, uint8_t where);
int8_t _find_property(uint32_t bit, uint32_t bit_fonc);
const struct fbgc_object_property_holder * get_fbgc_object_property_holder(struct fbgc_object * o);

const char * objtp2str(struct fbgc_object * );


enum{
	_LOC_PRINT = 0,
	_LOC_GET_SET_MEMBER,
	_LOC_BINARY_OPERATOR ,
	_LOC_UNARY_OPERATOR ,
	_LOC_SUBSCRIPT_OPERATOR ,
	_LOC_SUBSCRIPT_ASSIGN_OPERATOR ,
	_LOC_ABS_OPERATOR,
	_LOC_SIZE_OF,
	_LOC_TO_LOGIC ,
	_LOC_TO_INT ,
	_LOC_TO_DOUBLE ,
	_LOC_TO_STR ,
	_LOC_TO_TUPLE ,
	_LOC_DESTRUCTOR,
};
	

#define _BIT_PRINT (0x0001 << _LOC_PRINT)
#define _BIT_GET_SET_MEMBER (0x0001  << _LOC_GET_SET_MEMBER)
#define _BIT_BINARY_OPERATOR (0x0001 << _LOC_BINARY_OPERATOR)
#define _BIT_UNARY_OPERATOR	(0x0001 << _LOC_UNARY_OPERATOR)
#define _BIT_SUBSCRIPT_OPERATOR 		(0x0001 << _LOC_SUBSCRIPT_OPERATOR)
#define _BIT_SUBSCRIPT_ASSIGN_OPERATOR  (0x0001 << _LOC_SUBSCRIPT_ASSIGN_OPERATOR)
#define _BIT_ABS_OPERATOR (0x0001 << _LOC_ABS_OPERATOR)
#define _BIT_SIZE_OF (0x0001 << _LOC_SIZE_OF)
#define _BIT_TO_LOGIC (0x0001 << _LOC_TO_LOGIC)
#define _BIT_TO_INT (0x0001 << _LOC_TO_INT)
#define _BIT_TO_DOUBLE (0x0001 << _LOC_TO_DOUBLE)
#define _BIT_TO_STR (0x0001 << _LOC_TO_STR)
#define _BIT_TO_TUPLE (0x0001 << _LOC_TO_TUPLE)
#define _BIT_DESTRUCTOR (0x0001 << _LOC_DESTRUCTOR)


union _properties{
	struct fbgc_object * ( * const binary_operator)(struct fbgc_object *,struct fbgc_object * ,fbgc_token op);
	struct fbgc_object * ( * const unary_operator)(struct fbgc_object *,fbgc_token op);
	struct fbgc_object * ( * const subscript_operator)(struct fbgc_object *,struct fbgc_object *);
	struct fbgc_object * ( * const subscript_assign_operator)(struct fbgc_object *,struct fbgc_object *,struct fbgc_object *);
	struct fbgc_object * ( * const abs_operator)(struct fbgc_object *);
	struct fbgc_object * ( * const to_logic)(struct fbgc_object *);
	struct fbgc_object * ( * const to_int)(struct fbgc_object *);
	struct fbgc_object * ( * const to_double)(struct fbgc_object *);
	struct fbgc_object * ( * const to_str)(struct fbgc_object *);
	struct fbgc_object * ( * const to_tuple)(struct fbgc_object *);
	struct fbgc_object * ( * const to_matrix)(struct fbgc_object *);
	struct fbgc_object * ( * const to_range)(struct fbgc_object *);
	size_t (* const size_of)(struct fbgc_object * );

	struct fbgc_object * ( * const get_set_member)(struct fbgc_object *, const char *, struct fbgc_object *);
	
	uint8_t (* const print)(struct fbgc_object *);
	void (* const destructor)(struct fbgc_object *);
};

struct fbgc_object_property_holder{
	const uint32_t bits;
	const union _properties properties[];
};







#ifdef  __cplusplus
}
#endif

#endif