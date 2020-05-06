#include "fbgc.h"

static size_t map_new_capacity_from_size(size_t size){
	/*
		Below algorithm calculates the capacity for the given size
		Basically capacity is the closest two's power
		0,1 : 1
		2 : 2
		3,4 : 4
		5,6,7,8 : 8
		9,10,11,12,13,14,15 : 16
		and so on

		Take 5 for example, in binary its 0b00101
		take z = 1
		0b0001 <= 0b0101 , shift z left
		0b0010 <= 0b0101 , (ditto)
		0b0100 <= 0b0101 , (ditto)
		0b1000 <= 0b0101 , stop here don't shift, z is 8, the closest two's power for 5
	*/
	size_t z = 1;
	while(z < size)
		z <<= 1;

	return z;
}

struct fbgc_object * new_fbgc_map_object(size_t cap){
	
	//Make sure whateber the entry is we will end up having  2s power of capacity
	cap = map_new_capacity_from_size(cap);

	struct fbgc_map_object * map =  (struct fbgc_map_object*) fbgc_malloc_object(sizeof(struct fbgc_map_object) + sizeof(struct fbgc_map_pair)*cap);
	map->base.type = MAP;
	map->cap = cap;
	map->len = 0;
	map->max_psl = 0;
	while(cap--){
		struct fbgc_map_pair * p = &map->content[cap];
		p->key = NULL; //Now this part is also important, we may end up undefined bytes and cannot understand given index is empty or not
	}
	
	return (struct fbgc_object*) map;
}



static unsigned long fbgc_map_object_hash_function(const char * key){
    unsigned long hash = 5381;
    int c;

    while (c = *key++)
    	hash = ((hash << 5) + hash) + c; // hash * 33 + c
    
    return hash;
}

static size_t fbgc_map_object_index_from_hash(struct fbgc_object * self, unsigned long hash){
	return capacity_fbgc_map_object(self) != 0 ?  hash & (capacity_fbgc_map_object(self) - 1) : 0;	
}

static size_t fbgc_map_object_index(struct fbgc_object * self, struct fbgc_object * key){
	return fbgc_map_object_index_from_hash(self,fbgc_map_object_hash_function(content_fbgc_str_object(key)));
}

static struct fbgc_map_pair * fbgc_map_object_get_pair(struct fbgc_object * self, size_t index){
	return &cast_fbgc_object_as_map(self)->content[index];
}
static struct fbgc_map_pair ** fbgc_map_object_get_pair_address(struct fbgc_object * self, size_t index){
	return NULL;
	//return &cast_fbgc_object_as_map(self)->content[index];
}

static size_t fbgc_map_object_calculate_psl(struct fbgc_object * self,size_t index, size_t index_from_hash){
	if(index >= index_from_hash)
		return index - index_from_hash;
	else
		return capacity_fbgc_map_object(self) - index_from_hash + index;
}

static size_t fbgc_map_object_next_index(struct fbgc_object * self, size_t index){
	return (index + 1) & (capacity_fbgc_map_object(self)-1);
}

static ssize_t fbgc_map_object_get_psl(struct fbgc_object * self ,size_t index){
	struct fbgc_map_pair * p = fbgc_map_object_get_pair(self,index);
	if(p->key == NULL){
		FBGC_LOGV(MAP_OBJECT,"PSL returning:-1\n");
		return -1;
	}
	//FBGC_LOGV(MAP,"index:%ld, ht_index:%ld\n",index,ht_index(ht,p->key) );
	size_t val = fbgc_map_object_calculate_psl(self,index,fbgc_map_object_index(self,p->key));

	if(val > max_psl_fbgc_map_object(self)){
		cast_fbgc_object_as_map(self)->max_psl = val;
	}

	/*if(MAX_ALLOWED_PSL != -1 && max_psl_fbgc_map_object(self) > MAX_ALLOWED_PSL){
		//rehash_at_next_insertion = true;
	}*/

	FBGC_LOGV(MAP_OBJECT,"PSL returning!:%ld\n",val );
	return val;
}


static struct fbgc_map_pair * fbgc_map_object_find(struct fbgc_object * self, struct fbgc_object * key){
	size_t index = fbgc_map_object_index(self,key);
	struct fbgc_map_pair * p  = fbgc_map_object_get_pair(self,index);
	if(p->key != NULL){
		for(uint8_t i = 0; i <= max_psl_fbgc_map_object(self); ++i){
			if(length_fbgc_str_object(p->key) == length_fbgc_str_object(key) && 
				strcmp(content_fbgc_str_object(p->key),content_fbgc_str_object(key)) == 0){
				return p;
			}
			index = fbgc_map_object_next_index(self,index);
			p = fbgc_map_object_get_pair(self,index);
		}
	}
	return NULL;
}



static bool fbgc_map_object_compare_key_at_index(struct fbgc_object * self, size_t index, struct fbgc_object * key){
	struct fbgc_map_pair * p  = fbgc_map_object_get_pair(self,index);
	if(p->key == NULL) return false;

	return !strcmp(content_fbgc_str_object(p->key),content_fbgc_str_object(key));
}


static void fbgc_map_object_change_value_at_index(struct fbgc_object * self, size_t index, struct fbgc_object * value){
	struct fbgc_map_pair * p  = fbgc_map_object_get_pair(self,index);
	p->value = value;
}


static void fbgc_map_object_set_pair_at_index(struct fbgc_object * self,size_t index, struct fbgc_map_pair * p){
	struct fbgc_map_pair * at_index  = fbgc_map_object_get_pair(self,index);
	memcpy(at_index,p,sizeof(struct fbgc_map_pair));
}

static void fbgc_map_object_swap_pair_at_index(struct fbgc_object * self,size_t index, struct fbgc_map_pair * p){
	struct fbgc_map_pair * at_index  = fbgc_map_object_get_pair(self,index);

	struct fbgc_map_pair temp = {at_index->key,at_index->value};
	memmove(at_index,p,sizeof(struct fbgc_map_pair));
	memmove(p,&temp,sizeof(struct fbgc_map_pair));

}

static bool fbgc_map_object_is_index_empty(struct fbgc_object * self, size_t index){
	struct fbgc_map_pair * p  = fbgc_map_object_get_pair(self,index);
	return p->key == NULL;
}

static void fbgc_map_object_insert_value_on_rehash(struct fbgc_object * self, size_t index,struct fbgc_map_pair * p){
	ssize_t psl_from_ideal = 0;
	for(;;){
	    if(psl_from_ideal > fbgc_map_object_get_psl(self,index)){
	        if(fbgc_map_object_is_index_empty(self,index)){
	        	FBGC_LOGV(MAP_OBJECT,"ht_insert_value_on_rehash:index %lu empty\n",index);
	        	fbgc_map_object_set_pair_at_index(self,index,p);
	            //m_buckets[ibucket].set_value_of_empty_bucket(dist_from_ideal_bucket, hash, std::move(value));
	            return;
	        }
	        else {
	        	FBGC_LOGV(MAP_OBJECT,"ht_insert_value_on_rehash:swapping pairs at index %lu\n",index);
	        	fbgc_map_object_swap_pair_at_index(self,index,p);
	            //m_buckets[ibucket].swap_with_value_in_bucket(dist_from_ideal_bucket, hash, value);
	        }
	    }
	    
	    ++psl_from_ideal;
	    index = fbgc_map_object_next_index(self,index);
	}
}

static void fbgc_map_object_rehash(struct fbgc_object ** self){

	struct fbgc_map_object * ht = cast_fbgc_object_as_map(*self);

	//if(ht->cap == 0) ht->cap = 1;
	size_t old_cap = ht->cap;

	ht->cap <<= 1; //multiply by two

	struct fbgc_object * new_ht = new_fbgc_map_object(ht->cap);
	
	for(size_t i = 0; i<old_cap; ++i){
		struct fbgc_map_pair * p  = fbgc_map_object_get_pair(*self,i);
		if(p->key == NULL) continue;

		size_t index = fbgc_map_object_index(new_ht,p->key);

		//dprintf("Moving old key '%s':%d|old location:%lu | new location :%lu\n",p->key,p->value,i,index);
		//Do not copy the content just borrow the pointers
		fbgc_map_object_insert_value_on_rehash(new_ht,index,p);
	}
	
	length_fbgc_map_object(new_ht) = ht->len;
	*self = new_ht;	
}




static bool fbgc_map_object_try_rehash(struct fbgc_object ** self_adr){
	struct fbgc_object * self = *self_adr;
	if(length_fbgc_map_object(self) >= (REHASH_LOAD_FACTOR * capacity_fbgc_map_object(self))){
		FBGC_LOGV(MAP_OBJECT,"________rehash!___________\n");
		fbgc_map_object_rehash(self_adr);
		//rehash_at_next_insertion = false;
		return true;
	}

	return false;
}




struct fbgc_object * fbgc_map_object_insert(struct fbgc_object ** self_adr, struct fbgc_object * key,struct fbgc_object * value){

	struct fbgc_object * self = *self_adr;

	FBGC_LOGV(MAP_OBJECT,"%c:%c ==== Insertion",print_fbgc_object(key),print_fbgc_object(value));
	FBGC_LOGV(MAP_OBJECT,"Current table size:%lu | cap:%lu\n",length_fbgc_map_object(self),capacity_fbgc_map_object(self));

	unsigned long hash = fbgc_map_object_hash_function(content_fbgc_str_object(key)); 
	FBGC_LOGV(MAP_OBJECT,"hash :%lu\n",hash);
	
	size_t index = fbgc_map_object_index_from_hash(self,hash);
	FBGC_LOGV(MAP_OBJECT,"index :%lu\n",index);

	ssize_t psl_from_ideal = 0;

	while(psl_from_ideal <= fbgc_map_object_get_psl(self,index) ){
		
		if(fbgc_map_object_compare_key_at_index(self,index,key)){
			//Change the value
			FBGC_LOGV(MAP_OBJECT,"This is our old content, changing the value!\n");
			fbgc_map_object_change_value_at_index(self,index,value);
			return *self_adr;
		}

		index = fbgc_map_object_next_index(self,index);
		++psl_from_ideal;
	}

	if(fbgc_map_object_try_rehash(self_adr)){
		self = *self_adr;
		FBGC_LOGV(MAP_OBJECT,"Updated table size:%lu | cap:%lu\n",length_fbgc_map_object(self),capacity_fbgc_map_object(self));
		index = fbgc_map_object_index_from_hash(self,hash); 
		psl_from_ideal = 0;
		while(psl_from_ideal <= fbgc_map_object_get_psl(self,index)){
			index = fbgc_map_object_next_index(self,index);
			++psl_from_ideal;
		}
	}

	
	FBGC_LOGV(MAP_OBJECT,"After everything index is :%lu\n",index);

	if(fbgc_map_object_is_index_empty(self,index)){
		FBGC_LOGV(MAP_OBJECT,"Empty bucket found at %lu\n",index);
		struct fbgc_map_pair p = {key,value};
		fbgc_map_object_set_pair_at_index(self,index,&p);
	}
	else{
		FBGC_LOGV(MAP_OBJECT,"Inserting value into %lu\n",index);
		
		struct fbgc_map_pair p = {key,value};
		psl_from_ideal = fbgc_map_object_get_psl(self,index)+1;
		fbgc_map_object_swap_pair_at_index(self,index,&p);
		//dprintf("Now pair is '%s':%d\n",p.key,p.value );
		index = fbgc_map_object_next_index(self,index);

		size_t count = 0;
		
		while(p.key != NULL){
			//dprintf("psl now :%ld\n",psl_from_ideal);
			if(psl_from_ideal > fbgc_map_object_get_psl(self,index)){
				//if(psl_from_ideal > 5) assert(0);

				//dprintf("Swapping at index :%lu \n",index);
				psl_from_ideal = fbgc_map_object_get_psl(self,index);
				fbgc_map_object_swap_pair_at_index(self,index,&p);
				//psl_from_ideal = ht_get_psl(ht,index)+1;
			}
			//dprintf("p = {'%s':%d}\n",p.key,p.value);
			++psl_from_ideal;
			index = fbgc_map_object_next_index(self,index);
			++count;
			//if(MAX_ALLOWED_SWAP != -1 && count > MAX_ALLOWED_SWAP){
				//printf("MAX_ALLOWED_SWAP:%ld exceeded\n",count );
				//ht_rehash(ht);
				//--count;
				//rehash_at_next_insertion = true;
				//assert(0);
			//}
		}	
	}

	cast_fbgc_object_as_map(*self_adr)->len++;
	FBGC_LOGV(MAP_OBJECT,"Length is increased:%lu\n",length_fbgc_map_object(self));
	return *self_adr;
	
}

struct fbgc_object * fbgc_map_object_lookup(struct fbgc_object * self, struct fbgc_object * key){
	struct fbgc_map_pair * p  = fbgc_map_object_find(self,key);
	if(p != NULL){
		FBGC_LOGV(MAP_OBJECT,"%c:Lookup given key =>",print_fbgc_object(key));
		FBGC_LOGV(MAP_OBJECT,"%c:%c",print_fbgc_object(p->key),print_fbgc_object(p->value));
		return p->value;
	}
	else{
		FBGC_LOGE("Key[%s] is not in the map\n",content_fbgc_str_object(key));
		assert(0);
	}
}
struct fbgc_object * fbgc_map_object_remove(struct fbgc_object ** self, struct fbgc_object * key){
	struct fbgc_map_pair * p  = fbgc_map_object_find(*self,key);
	if(p != NULL){
		cast_fbgc_object_as_map(*self)->len--;
		p->key = NULL;
		if(length_fbgc_map_object(*self) <= (REHASH_LOAD_FACTOR * capacity_fbgc_map_object(*self)) ){
			//XXXXXXXXXX
			//ht_shrink(ht);
		}
		return p->value;
	}
	else{
		FBGC_LOGE("%c is not in the map\n",print_fbgc_object(key));
		assert(0);
	}
}


uint8_t print_fbgc_map_object(struct fbgc_object * obj){
	printf("{");
	for(size_t i = 0; i<capacity_fbgc_map_object(obj); i++){
		struct fbgc_map_pair * p  = fbgc_map_object_get_pair(obj,i);
		if(p->key != NULL){
			print_fbgc_object(p->key); printf(":"); print_fbgc_object(p->value);
			printf(", ");
		}
	}
	return printf("}");
}


const struct fbgc_object_property_holder fbgc_map_object_property_holder = {
    .bits = 
    _BIT_PRINT 
    ,
    .properties ={
        {.print = &print_fbgc_map_object},
    }
};
