#include "fbgc.h"

//below bit operations are defined for 1byte values
#define get_bit_at(num,bit_no) ( (num>>bit_no)&((uint8_t)1) )
#define set_bit_at(num,bit_no) ( num |= (1<<bit_no) )
#define clear_bit_at(num,bit_no) ( num &= ~(1<<bit_no) )
#define set_value_bit_at(num,bit_no,value) (num ^= (-value ^ num) & ((uint8_t)1 << bit_no) )
#define toggle_bit_at(num,bit_no) (num ^= (uint8_t)1 << bit_no)

void init_static_fbgc_bool_vector(struct fbgc_bool_vector * bv,size_t cap){
	assert(bv);
	//Find the closes power of 8 
	bv->content = (uint8_t*)fbgc_malloc(ceil(cap/8.0) + (cap == 0) );
	bv->size = 0;
}

struct fbgc_bool_vector * new_fbgc_bool_vector(size_t cap){
	struct fbgc_bool_vector * bv = (struct fbgc_bool_vector *) fbgc_malloc(sizeof(struct fbgc_bool_vector));
	init_static_fbgc_bool_vector(bv,cap);
	return bv;
}

inline size_t size_fbgc_bool_vector(const struct fbgc_bool_vector * bv){
	return bv->size;
}

inline static size_t byte_capacity_fbgc_bool_vector(const struct fbgc_bool_vector * bv){
	//So multiply by 8 to get bit number
	return capacity_fbgc_raw_memory(bv->content,1);
}
inline size_t capacity_fbgc_bool_vector(const struct fbgc_bool_vector * bv){
	//So multiply by 8 to get bit number
	return capacity_fbgc_raw_memory(bv->content,1)*8;
}
inline bool is_full_fbgc_bool_vector(const struct fbgc_bool_vector * bv){
	return bv->size == capacity_fbgc_bool_vector(bv);
}
inline bool is_empty_fbgc_bool_vector(const struct fbgc_bool_vector * bv){
	return bv->size == 0;
}

static uint8_t * from_bit_index_get_byte_address(const struct fbgc_bool_vector * bv, size_t * index){
	//first find which byte holds this index return its address and also update incoming index
	size_t array_index =  (*index != 0) ? ceil(*index/8.0)-1 : 0;
	assert(array_index <= (capacity_fbgc_bool_vector(bv)/8) );
	//cprintf(100,"Inc index %ld, array_index %ld, ",*index,array_index);
	*index %= 8;
	//cprintf(110,"bit index %ld\n", *index);
	return bv->content+array_index;
}

bool at_fbgc_bool_vector(const struct fbgc_bool_vector * bv, size_t index){
	uint8_t * byte_addr = from_bit_index_get_byte_address(bv,&index);
	return get_bit_at(*byte_addr,index);
}

static uint8_t * from_negative_index_to_positive_index_byte_array(const struct fbgc_bool_vector * bv, int * index){
	*index = (*index < 0) * bv->size +  *index;
	assert( *index < bv->size && *index >= 0 );
	size_t pos_index = *index;
	return from_bit_index_get_byte_address(bv,&pos_index);
}

bool get_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index){
	uint8_t * byte_addr = from_negative_index_to_positive_index_byte_array(bv,&index);
	return get_bit_at(*byte_addr,index);
}
void set_bit_value_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index, bool value){
	uint8_t * byte_addr = from_negative_index_to_positive_index_byte_array(bv,&index);
	set_value_bit_at(*byte_addr,index,value);
}
void set_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index){
	uint8_t * byte_addr = from_negative_index_to_positive_index_byte_array(bv,&index);
	set_value_bit_at(*byte_addr,index,1);
}
void clear_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index){
	uint8_t * byte_addr = from_negative_index_to_positive_index_byte_array(bv,&index);
	set_value_bit_at(*byte_addr,index,0);
}

void toggle_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index){
	uint8_t * byte_addr = from_negative_index_to_positive_index_byte_array(bv,&index);
	toggle_bit_at(*byte_addr,index);
}

bool front_fbgc_bool_vector(const struct fbgc_bool_vector * bv){
	//How can we handle this errors?
	assert(bv->size);
	return at_fbgc_bool_vector(bv,0);
}
bool back_fbgc_bool_vector(const struct fbgc_bool_vector * bv){
	//How can we handle this errors?
	assert(bv->size);
	return at_fbgc_bool_vector(bv,bv->size-1);	
}

void set_back_fbgc_bool_vector(const struct fbgc_bool_vector * bv, bool value){
	assert(bv->size);
	set_bit_value_fbgc_bool_vector(bv,bv->size-1,value);
}


void push_back_fbgc_bool_vector(struct fbgc_bool_vector * bv, bool value){
	//	Check the capacity, if there is enough space push back the obj	
	if(is_full_fbgc_bool_vector(bv)){
		//request change for the capacity of the vector
		bv->content = fbgc_realloc(bv->content,((byte_capacity_fbgc_bool_vector(bv))+1) );
	}
	size_t index = bv->size;
	uint8_t * byte_addr = from_bit_index_get_byte_address(bv,&index);

	set_value_bit_at(*byte_addr,index,value);
	
	++bv->size;
}
void pop_back_fbgc_bool_vector(struct fbgc_bool_vector * bv){
	--bv->size;
	shrink_fbgc_bool_vector(bv,bv->size);
}

void shrink_fbgc_bool_vector(struct fbgc_bool_vector * bv, size_t cap){
	//Find the closes power of 8 
	cap = ceil(cap/8.0) + (cap == 0);
	if(cap < byte_capacity_fbgc_bool_vector(bv)){
		bv->content = fbgc_realloc(bv->content,cap);	
	}
	
}




void insert_fbgc_bool_vector(struct fbgc_bool_vector * bv, const void * item, size_t index_start, size_t item_len);



void test_fbgc_bool_vector(){
	struct fbgc_bool_vector * bv = new_fbgc_bool_vector(7);
	//printf("Bv size %ld\n",size_fbgc_bool_vector(bv));
	//printf("Bv cap  %ld\n",capacity_fbgc_bool_vector(bv));

	for(int i = 0; i<10; ++i)
		push_back_fbgc_bool_vector(bv,0);
	printf("Bv size %ld\n",size_fbgc_bool_vector(bv));

	// set_bit_fbgc_bool_vector(bv,3,1);
	// set_bit_fbgc_bool_vector(bv,4,1);
	// set_bit_fbgc_bool_vector(bv,5,1);
	
	pop_back_fbgc_bool_vector(bv);
	pop_back_fbgc_bool_vector(bv);
	pop_back_fbgc_bool_vector(bv);
	
	set_bit_fbgc_bool_vector(bv,-1);
	set_bit_fbgc_bool_vector(bv,-2);
	set_bit_fbgc_bool_vector(bv,-5);

	printf("Bv size %ld | cap %ld\n",size_fbgc_bool_vector(bv), capacity_fbgc_bool_vector(bv));
	for (int i = 0; i < size_fbgc_bool_vector(bv); ++i)
	{
		printf("Bv content[%d]:%d\n",i,get_bit_fbgc_bool_vector(bv,i));
	}
	//set_bit_fbgc_bool_vector(bv,1,3);
}