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
#include "fbgc.h"

static struct fbgc_object * _new_fbgc_map_object(size_t cap, uint8_t rlf){
	
	struct fbgc_map_object * map =  (struct fbgc_map_object*) fbgc_malloc_object(sizeof(struct fbgc_map_object));
	map->base.type = MAP;
	map->size = 0;
	map->max_psl = 0;
	map->rehash_load_factor = rlf;
	//map->max_allowed_psl = 5;
	map->rehash_at_next_insertion = false;
	map->content = (struct fbgc_map_pair *) fbgc_malloc(cap * sizeof(struct fbgc_map_pair) );
	
	return (struct fbgc_object*) map;
}

struct fbgc_object * new_fbgc_map_object(size_t cap, uint8_t rlf){
	return _new_fbgc_map_object(fbgc_round_to_closest_power_of_two(cap), rlf);
}

struct fbgc_object * new_fbgc_map_object_exact(size_t cap, uint8_t rlf){
	return _new_fbgc_map_object(cap, rlf);
}


void fbgc_map_object_set_rehash_load_factor(struct fbgc_object *self, uint8_t rlf){
	cast_fbgc_object_as_map(self)->rehash_load_factor = rlf;
}


static unsigned long map_hash_function(const char * key,size_t len){
	unsigned long hash = 5381;
	int c;
	int count = 0;
	while(c = *key++ && --len){
		++count;
		hash = ((hash << 5) + hash) + c; // hash * 33 + c
	}
	return hash;
}

static size_t map_index_from_hash(struct fbgc_object * self, unsigned long hash){
	return capacity_fbgc_map_object(self) != 0 ?  hash & (capacity_fbgc_map_object(self) - 1) : 0;	
}

static size_t map_index(struct fbgc_object * self, struct fbgc_object * key){
	return map_index_from_hash(self,map_hash_function(content_fbgc_str_object(key),length_fbgc_str_object(key)));
}

static struct fbgc_map_pair * map_get_pair(struct fbgc_object * self, size_t index){
	return &cast_fbgc_object_as_map(self)->content[index];
}

static size_t map_calculate_psl(struct fbgc_object * self,size_t index, size_t index_from_hash){

	return capacity_fbgc_map_object(self)*((index < index_from_hash)) + index - index_from_hash;

	//if(index >= index_from_hash)
	//	return index - index_from_hash;
	//else
	//	return capacity_fbgc_map_object(self) - index_from_hash + index;
}

static size_t map_next_index(struct fbgc_object * self, size_t index){
	
	return (index + 1) % (capacity_fbgc_map_object(self));
	//return (index + 1) & (capacity_fbgc_map_object(self)-1);
}

static ssize_t map_get_psl(struct fbgc_object * self ,size_t index){
	struct fbgc_map_pair * p = map_get_pair(self,index);
	if(p->key == NULL){
		FBGC_LOGV(MAP_OBJECT,"PSL returning:-1\n");
		return -1;
	}
	//FBGC_LOGV(MAP,"index:%ld, ht_index:%ld\n",index,ht_index(ht,p->key) );
	size_t val = map_calculate_psl(self,index,map_index(self,p->key));

	if(val > max_psl_fbgc_map_object(self)){
		cast_fbgc_object_as_map(self)->max_psl = val;
	}

	if(max_psl_fbgc_map_object(self) > MAX_ALLOWED_PSL){
		cast_fbgc_object_as_map(self)->rehash_at_next_insertion = true;
	}

	FBGC_LOGV(MAP_OBJECT,"PSL returning!:%ld\n",val );
	return val;
}


static struct fbgc_map_pair * map_find_str(struct fbgc_object * self, const char * key, size_t len){
	size_t index = map_index_from_hash(self,map_hash_function(key,len));
	struct fbgc_map_pair * p  = map_get_pair(self,index);
	FBGC_LOGD(MAP_OBJECT,"Trying to find key '%s'\n",key);

	for(uint8_t i = 0; p->key != NULL && i <= max_psl_fbgc_map_object(self); ++i){
		FBGC_LOGD(MAP_OBJECT,"%c%c<= key:pair\n",print_fbgc_object(p->key),print_fbgc_object(p->value));
		//FBGC_LOGE("pkey:%lu,len:%lu| comparison:%d\n",length_fbgc_str_object(p->key),len,strcmp(content_fbgc_str_object(p->key),key));
		if(length_fbgc_str_object(p->key) == len && 
			strcmp(content_fbgc_str_object(p->key),key) == 0){
			return p;
		}
		
		index = map_next_index(self,index);
		p = map_get_pair(self,index);
	}
	return NULL;
}


static struct fbgc_map_pair * map_find(struct fbgc_object * self, struct fbgc_object * key){
	return map_find_str(self,content_fbgc_str_object(key),length_fbgc_str_object(key));
}



static bool map_compare_key_at_index(struct fbgc_object * self, size_t index, struct fbgc_object * key){
	struct fbgc_map_pair * p  = map_get_pair(self,index);
	if(p->key == NULL) return false;

	return !strcmp(content_fbgc_str_object(p->key),content_fbgc_str_object(key));
}


static void map_change_value_at_index(struct fbgc_object * self, size_t index, struct fbgc_object * value){
	struct fbgc_map_pair * p  = map_get_pair(self,index);
	p->value = value;
}


static void map_set_pair_at_index(struct fbgc_object * self,size_t index, struct fbgc_map_pair * p){
	struct fbgc_map_pair * at_index  = map_get_pair(self,index);
	memcpy(at_index,p,sizeof(struct fbgc_map_pair));
}

static void map_swap_pair_at_index(struct fbgc_object * self,size_t index, struct fbgc_map_pair * p){
	struct fbgc_map_pair * at_index  = map_get_pair(self,index);

	struct fbgc_map_pair temp = {at_index->key,at_index->value};
	memmove(at_index,p,sizeof(struct fbgc_map_pair));
	p->key = temp.key;
	p->value = temp.value;
	//memmove(p,&temp,sizeof(struct fbgc_map_pair));

}

static bool map_is_index_empty(struct fbgc_object * self, size_t index){
	struct fbgc_map_pair * p  = map_get_pair(self,index);
	return p->key == NULL;
}

static void map_insert_value_on_rehash(struct fbgc_object * self, size_t index,struct fbgc_map_pair * p){
	ssize_t psl_from_ideal = 0;
	for(;;){
		if(psl_from_ideal > map_get_psl(self,index)){
			//cprintf(100,"psl_from_ideal : %d\n",psl_from_ideal);
			if(psl_from_ideal > max_psl_fbgc_map_object(self)){
				cast_fbgc_object_as_map(self)->max_psl = psl_from_ideal;
			}

			if(map_is_index_empty(self,index)){
				FBGC_LOGV(MAP_OBJECT,"ht_insert_value_on_rehash:index %lu empty\n",index);
				map_set_pair_at_index(self,index,p);
				_FBGC_LOGV(MAP_OBJECT,"returning");
				return;
			}
			else {
				FBGC_LOGV(MAP_OBJECT,"ht_insert_value_on_rehash:swapping pairs at index %lu\n",index);
				map_swap_pair_at_index(self,index,p);
			}
		}
		++psl_from_ideal;
		index = map_next_index(self,index);
		
		
	}
}

static void map_rehash(struct fbgc_object * self, size_t new_cap){

	struct fbgc_object * new_ht = new_fbgc_map_object_exact(new_cap, cast_fbgc_object_as_map(self)->rehash_load_factor );
	FBGC_LOGD(MAP_OBJECT,"New capacity : %lu\n",capacity_fbgc_map_object(new_ht));
	for(size_t i = 0; i<capacity_fbgc_map_object(self); ++i){
		struct fbgc_map_pair * p  = map_get_pair(self,i);
		if(p->key == NULL) continue;

		size_t index = map_index(new_ht,p->key);

		FBGC_LOGD(MAP_OBJECT,"Moving old key '%s'|old location:%lu | new location :%lu\n",content_fbgc_str_object(p->key),i,index);
		//Do not copy the content just borrow the pointers
		map_insert_value_on_rehash(new_ht,index,p);
		

	}
	cast_fbgc_object_as_map(self)->max_psl = cast_fbgc_object_as_map(new_ht)->max_psl;
	cast_fbgc_object_as_map(self)->content = cast_fbgc_object_as_map(new_ht)->content;

	FBGC_LOGD(MAP_OBJECT,"New capacity self: %lu\n",capacity_fbgc_map_object(self));
	//print_fbgc_map_object(self);
	FBGC_LOGV(MAP_OBJECT,"-----------------------------");
}




static bool map_try_rehash(struct fbgc_object * self){
	if(size_fbgc_map_object(self) >= ((cast_fbgc_object_as_map(self)->rehash_load_factor/100.0) * capacity_fbgc_map_object(self))){
		FBGC_LOGV(MAP_OBJECT,"________rehash!___________\n");
		map_rehash(self,capacity_fbgc_map_object(self)<<1);
		cast_fbgc_object_as_map(self)->rehash_at_next_insertion = false;
		return true;
	}

	return false;
}




struct fbgc_object * fbgc_map_object_insert(struct fbgc_object * self, struct fbgc_object * key,struct fbgc_object * value){


	FBGC_LOGD(MAP_OBJECT,"=====================Insertion===================\n");
	FBGC_LOGV(MAP_OBJECT,"%c%c\n",print_fbgc_object(key),print_fbgc_object(value));
	FBGC_LOGV(MAP_OBJECT,"Current table size:%lu | cap:%lu\n",size_fbgc_map_object(self),capacity_fbgc_map_object(self));

	unsigned long hash = map_hash_function(content_fbgc_str_object(key),length_fbgc_str_object(key)); 
	FBGC_LOGV(MAP_OBJECT,"hash :%lu\n",hash);
	
	size_t index = map_index_from_hash(self,hash);
	FBGC_LOGV(MAP_OBJECT,"index :%lu\n",index);

	ssize_t psl_from_ideal = 0;

	while(psl_from_ideal <= map_get_psl(self,index) ){
		
		if(map_compare_key_at_index(self,index,key)){
			//Change the value
			FBGC_LOGV(MAP_OBJECT,"This is our old content, changing the value!\n");
			map_change_value_at_index(self,index,value);
			return self;
		}

		index = map_next_index(self,index);
		++psl_from_ideal;
	}

	if(map_try_rehash(self)){
		FBGC_LOGV(MAP_OBJECT,"Updated table size:%lu | cap:%lu\n",size_fbgc_map_object(self),capacity_fbgc_map_object(self));
		index = map_index_from_hash(self,hash); 
		psl_from_ideal = 0;
		while(psl_from_ideal <= map_get_psl(self,index)){
			index = map_next_index(self,index);
			++psl_from_ideal;
		}
	}

	
	FBGC_LOGV(MAP_OBJECT,"After new allocation and old pair checkings index is :%lu\n",index);

	if(psl_from_ideal > max_psl_fbgc_map_object(self)){
		cast_fbgc_object_as_map(self)->max_psl = psl_from_ideal;
	}
			
	if(map_is_index_empty(self,index)){
		FBGC_LOGD(MAP_OBJECT,"Empty bucket found at %lu\n",index);
		struct fbgc_map_pair p = {key,value};
		map_set_pair_at_index(self,index,&p);
	}
	else{
		FBGC_LOGV(MAP_OBJECT,"Inserting value into %lu(non empty bucket stealing)\n",index);
		
		struct fbgc_map_pair p = {key,value};
		psl_from_ideal = map_get_psl(self,index)+1;
		map_swap_pair_at_index(self,index,&p);
		//dprintf("Now pair is '%s':%d\n",p.key,p.value );
		index = map_next_index(self,index);

		size_t count = 0;
		
		while(p.key != NULL){

			if(psl_from_ideal > max_psl_fbgc_map_object(self)){
				cast_fbgc_object_as_map(self)->max_psl = psl_from_ideal;
			}

			if(psl_from_ideal > map_get_psl(self,index)){
				//if(psl_from_ideal > 5) assert(0);

				FBGC_LOGD(MAP_OBJECT,"Swapping at index :%lu \n",index);
				psl_from_ideal = map_get_psl(self,index);
				map_swap_pair_at_index(self,index,&p);
				//psl_from_ideal = ht_get_psl(ht,index)+1;
			}
			//dprintf("p = {'%s':%d}\n",p.key,p.value);
			++psl_from_ideal;
			index = map_next_index(self,index);
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

	cast_fbgc_object_as_map(self)->size++;
	FBGC_LOGV(MAP_OBJECT,"Length is increased:%lu\n",size_fbgc_map_object(self));
	return self;
	
}

struct fbgc_object * fbgc_map_object_insert_str(struct fbgc_object * self, const char * key,struct fbgc_object * value){
	return fbgc_map_object_insert(self,new_fbgc_str_object(key),value);
}

struct fbgc_map_pair * fbgc_map_object_lookup_map_pair(struct fbgc_object * self, struct fbgc_object * key){
	struct fbgc_map_pair * p  = map_find(self,key);
	return p;
}

struct fbgc_object * fbgc_map_object_lookup(struct fbgc_object * self, struct fbgc_object * key){
	struct fbgc_map_pair * p  = map_find(self,key);
	if(p != NULL){
		FBGC_LOGV(MAP_OBJECT,"%c:Lookup given key =>",print_fbgc_object(key));
		FBGC_LOGV(MAP_OBJECT,"%c:%c",print_fbgc_object(p->key),print_fbgc_object(p->value));
		return p->value;
	}
	return NULL;
}

struct fbgc_object * fbgc_map_object_lookup_str(struct fbgc_object * self, const char * key){
	struct fbgc_map_pair * p  = map_find_str(self,key,strlen(key));
	if(p != NULL) return p->value;
	return NULL;
}

struct fbgc_object * fbgc_map_object_lookup_substr(struct fbgc_object * self, const char * key1,const char * key2){
	struct fbgc_map_pair * p  = map_find_str(self,key1,key2-key1);
	if(p != NULL) return p->value;
	return NULL;
}

bool fbgc_map_object_does_key_exist(struct fbgc_object * self, struct fbgc_object * key){
	return (map_find(self,key) != NULL);
}

bool fbgc_map_object_does_key_exist_str(struct fbgc_object * self, const char * key){
	return (map_find_str(self,key,strlen(key)) != NULL);
}

bool fbgc_map_object_does_key_exist_substr(struct fbgc_object * self, const char * key1,const char * key2){
	return (map_find_str(self,key1,key2-key1) != NULL);
}

struct fbgc_object* fbgc_map_object_get_key_substr(struct fbgc_object * self, const char * key1,const char * key2){
	struct fbgc_map_pair * p  = map_find_str(self,key1,key2-key1);
	if(p != NULL){
		return p->key;
	}
	return NULL;

}


struct fbgc_object * fbgc_map_object_get_value_by_index(struct fbgc_object * self,size_t index){
	struct fbgc_map_pair * p  = map_get_pair(self,index);
	if(p != NULL) return p->value;
	return NULL;
}


static ssize_t map_get_key_index(struct fbgc_object * self, const char * key, size_t len){
	size_t index = map_index_from_hash(self,map_hash_function(key,len));
	struct fbgc_map_pair * p  = map_get_pair(self,index);
	

	for(uint8_t i = 0; p->key != NULL && i <= max_psl_fbgc_map_object(self); ++i){
		//FBGC_LOGE("pkey:%lu,len:%lu| comparison:%d\n",length_fbgc_str_object(p->key),len,strcmp(content_fbgc_str_object(p->key),key));
		if(length_fbgc_str_object(p->key) == len && 
			strcmp(content_fbgc_str_object(p->key),key) == 0){
			return index;
		}
		
		index = map_next_index(self,index);
		p = map_get_pair(self,index);
	}
	return -1;
}

ssize_t fbgc_map_object_get_key_index(struct fbgc_object * self, struct fbgc_object * key){
	return map_get_key_index(self,content_fbgc_str_object(key),length_fbgc_str_object(key));
}

ssize_t fbgc_map_object_get_key_index_str(struct fbgc_object * self, const char * key){
	return map_get_key_index(self,key,strlen(key));
}

ssize_t fbgc_map_object_get_key_index_substr(struct fbgc_object * self,const char *key1, const char *key2){
	return map_get_key_index(self,key1,key2-key1);
}



struct fbgc_map_pair * fbgc_map_object_get_pair(struct fbgc_object * self, size_t index){
	return &cast_fbgc_object_as_map(self)->content[index];
}

// struct fbgc_map_pair * fbgc_map_object_iterator_begin(struct fbgc_object * self){

// }


// struct fbgc_map_pair * fbgc_map_object_iterator_next(struct fbgc_object * self){
	
// }


// struct fbgc_map_pair * fbgc_map_object_iterator_end(struct fbgc_object * self){
// 	//return map_get_pair(obj,capacity_fbgc_map_object(self)-1);
// }


static struct fbgc_object * map_remove(struct fbgc_object * self, const char * key, size_t len){
	struct fbgc_map_pair * p  = map_find_str(self,key,len);
	if(p != NULL){
		cast_fbgc_object_as_map(self)->size--;
		p->key = NULL;
		if(size_fbgc_map_object(self) <= ((cast_fbgc_object_as_map(self)->rehash_load_factor/100.0) * capacity_fbgc_map_object(self)) ){
			//Try to shrink
			map_rehash(self,capacity_fbgc_map_object(self)>>1);
		}
		return p->value;
	}
	return NULL;	
}


struct fbgc_object * fbgc_map_object_remove_str(struct fbgc_object * self, const char * key){
	return map_remove(self,key,strlen(key));
}

struct fbgc_object * fbgc_map_object_remove_substr(struct fbgc_object * self, const char * key1,const char * key2){
	return map_remove(self,key1,key2-key1);
}

struct fbgc_object * fbgc_map_object_remove(struct fbgc_object * self, struct fbgc_object * key){
	return map_remove(self,content_fbgc_str_object(key),length_fbgc_str_object(key));

}

void fbgc_map_object_set_exact_size(struct fbgc_object * self){
	size_t current_size = size_fbgc_map_object(self);
	map_rehash(self,current_size);
}

struct fbgc_object * fbgc_map_object_merge(struct fbgc_object * self, struct fbgc_object * map2, bool update_existed){
	//if key exist in self, check update_existed. If false we won't change it otherwise value will be changed
	for(size_t i = 0; i<capacity_fbgc_map_object(map2); i++){
		struct fbgc_map_pair * p  = map_get_pair(map2,i);
		if(p->key != NULL){
			struct fbgc_map_pair * p2 = map_find(self,p->key);
			if(p2){
				fbgc_map_object_insert(self,p->key, update_existed ? p->value : p2->value);
			}
			else{
				fbgc_map_object_insert(self,p->key, p->value);
			}
		}
	}
	return self;
}



uint8_t print_detailed_fbgc_map_object(struct fbgc_object * obj){
	printf("Capacity : %u, size:%lu, max_psl:%d\n",capacity_fbgc_map_object(obj),size_fbgc_map_object(obj),max_psl_fbgc_map_object(obj));
	printf("{");
	for(size_t i = 0; i<capacity_fbgc_map_object(obj); i++){
		struct fbgc_map_pair * p  = map_get_pair(obj,i);
		if(p->key != NULL){
			ssize_t psl = map_calculate_psl(obj,i,map_index(obj,p->key));
			print_fbgc_object(p->key); printf("(%lu):",i); print_fbgc_object(p->value); printf("psl(%ld)\n",psl);
			printf(", ");
		}
	}
	return printf("}");
}

uint8_t print_fbgc_map_object(struct fbgc_object * obj){
	printf("{");
	for(size_t i = 0; i<capacity_fbgc_map_object(obj); i++){
		struct fbgc_map_pair * p  = map_get_pair(obj,i);
		if(p->key != NULL){
			ssize_t psl = map_calculate_psl(obj,i,map_index(obj,p->key));
			print_fbgc_object(p->key); printf(":"); print_fbgc_object(p->value);
			printf(", ");
		}
	}
	return printf("}");
}



struct fbgc_object * subscript_operator_fbgc_map_object(struct fbgc_object * iterable,struct fbgc_object * index_obj){
	if(index_obj->type != STRING){
		FBGC_LOGE("Index value must be string");
		return NULL;
	}

	return fbgc_map_object_lookup(iterable,index_obj);
}

struct fbgc_object * subscript_assign_operator_fbgc_map_object(struct fbgc_object * iterable,struct fbgc_object * index_obj, struct fbgc_object * rhs){

	if(index_obj->type != STRING){
		FBGC_LOGE("Index value must be string");
		return NULL;
	}
	return fbgc_map_object_insert(iterable,index_obj,rhs);  
}



struct fbgc_object * operator_fbgc_map_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
	assert(0);
}



static struct fbgc_object * abs_operator_fbgc_map_object(struct fbgc_object * self){
	return new_fbgc_int_object(cast_fbgc_object_as_map(self)->size);
}


static inline size_t size_of_fbgc_map_object(struct fbgc_object * self){
    return sizeof(struct fbgc_map_object);
}

static inline void gc_mark_fbgc_map_object(struct fbgc_object * self){
    fbgc_gc_mark_pointer((cast_fbgc_object_as_map(self)->content),sizeof(struct fbgc_map_pair *));
}



const struct fbgc_object_property_holder fbgc_map_object_property_holder = {
	.bits = 
	_BIT_PRINT |
	_BIT_SUBSCRIPT_OPERATOR |
	_BIT_SUBSCRIPT_ASSIGN_OPERATOR |
	_BIT_ABS_OPERATOR | 
	_BIT_SIZE_OF |
	_BIT_GC_MARK
	,
	
	.properties ={
		{.print = &print_fbgc_map_object},
		{.subscript_operator = &subscript_operator_fbgc_map_object},
		{.subscript_assign_operator = &subscript_assign_operator_fbgc_map_object},
		{.abs_operator = &abs_operator_fbgc_map_object},
		{.gc_mark = &gc_mark_fbgc_map_object},
		{.size_of = &size_of_fbgc_map_object},
	}
};