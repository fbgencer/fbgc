#include "fbgc.h"

struct general_symbol_container general_symbols = {.sym = NULL};

struct fbgc_object * initialize_fbgc_symbol_table(){
    //struct fbgc_tuple_object * table = (struct fbgc_tuple_object*) new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
   	fbgc_symbols = new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
    return NULL;
}
struct fbgc_ll_base * new_fbgc_symbol_from_substr(const char * str1,const char * str2){
	
	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Symbol table from substring\n");
	#endif


	//General symbol holder see symbol_table.h
	struct fbgc_object ** symbols = content_fbgc_tuple_object(fbgc_symbols);
	
	#ifdef SYMBOL_TABLE_DEBUG
		cprintf(111,"Trying to find str in old symbols\n");
	#endif

	for(size_t i = 0; i<size_fbgc_tuple_object(fbgc_symbols); i++){
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		// Find something proper, don't check the string size first but without checking the first match of n characters will return true 
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		if( strlen(content_fbgc_str_object(symbols[i])) == str2-str1 && !my_strncmp(content_fbgc_str_object(symbols[i]),str1,str2-str1)  ){

			#ifdef SYMBOL_TABLE_DEBUG
				cprintf(110,"Found!\n");
			#endif
			return _new_fbgc_ll_identifier(i); 
		}
	}


	#ifdef SYMBOL_TABLE_DEBUG
		cprintf(111,"Couldn't found, creating symbol!\n");
	#endif	
	//end = clock();
	//search_time = (double)(end - begin) / CLOCKS_PER_SEC; 
	//printf("Time :%f\n",search_time );

	struct fbgc_object * temp_obj =  new_fbgc_str_object_from_substr(str1,str2);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"OLD symbol table : ");
	//:>print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	cprintf(110,"New symbol from s1:%s to s2:%s\n",str1,str2);
	cprintf(110,"Symbol has been created as cstring : ");
	print_fbgc_cstr_object(temp_obj);
	cprintf(111,"\n");
	#endif

	
	fbgc_symbols = push_back_fbgc_tuple_object(fbgc_symbols,temp_obj);

	//:>cast_fbgc_object_as_field(field_obj)->symbols = table;
	//fbgc_symbols = table;

	//table pointer may not be the same anymore
	symbols = content_fbgc_tuple_object(fbgc_symbols);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Succesfully added in the symbol table : ");
	//:>print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	#endif	

	return _new_fbgc_ll_identifier(size_fbgc_tuple_object(fbgc_symbols)-1);
}


void print_fbgc_symbol_table(struct fbgc_object * ll){


	cprintf(010,"[~~~~~~~~~~~~[SYMBOL TABLE]~~~~~~~~~~~~~]\n");
//	print_fbgc_ll_object(ll,"SYMBOLS:");
	cprintf(110,"\n");
	cprintf(010,"[~~~~~~~~~~~~~~~~~~~~~~~~~]\n");

}


/*
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
//#include <iostream>
//#include <functional>
//#include <string>

#define DEBUG_

#ifdef DEBUG
#define dprintf(...) printf(__VA_ARGS__);
#else
#define dprintf(...)
#endif

//std::hash<std::string> str_hash;
int strdup_ctr= 0;

static const double REHASH_LOAD_FACTOR = 0.5;
static const size_t MAX_ALLOWED_PSL = -1;
static const size_t MAX_ALLOWED_SWAP = -1;

typedef struct pair{
	const char * key;
	int value;
}pair;

bool rehash_at_next_insertion = false;

typedef struct hashtable{
	pair * kv_list;
	size_t cap;
	size_t len;
	ssize_t max_psl;
}hashtable;

hashtable * ht_new(size_t cap);
void ht_print(hashtable * ht);
void ht_clear(hashtable * ht);
size_t ht_hash(hashtable * ht, const char * key);
size_t ht_index_from_hash(hashtable * ht,size_t hash);
size_t ht_index(hashtable * ht, const char * key);
ssize_t ht_get_psl(hashtable *ht ,size_t index);
bool ht_compare_key_at_index(hashtable * ht, size_t index, const char * key);
void ht_change_value_at_index(hashtable * ht, size_t index, int value);
size_t ht_next_index(hashtable * ht, size_t index);
bool ht_try_rehash(hashtable * ht);
void ht_rehash(hashtable * ht);
pair * ht_get_pair(hashtable * ht, size_t index);
ssize_t ht_abs(ssize_t i);
ssize_t ht_calculate_psl(hashtable * ht,size_t index, size_t index_from_hash);


bool ht_is_index_empty(hashtable *ht, size_t index);
void ht_set_new_pair_at_index(hashtable * ht,size_t index, pair * p);
void ht_set_pair_at_index(hashtable * ht,size_t index, pair * p);
void ht_swap_pair_at_index(hashtable * ht,size_t index, pair * p);
void ht_swap_pairs(hashtable * ht, size_t index1, size_t index2);

void ht_insert_value_on_rehash(hashtable * ht, size_t index, ssize_t psl_from_ideal,  pair * p);

void ht_insert(hashtable * ht,const char * key, int value){
	dprintf("========================Insert['%s':%d]===========================\n",key,value);
	dprintf("Current table size:%lu | cap:%lu\n",ht->len,ht->cap);
	size_t hash = ht_hash(ht,key); dprintf("hash :%lu\n",hash);
	size_t index = ht_index_from_hash(ht,hash); dprintf("index :%lu\n",index);

	ssize_t psl_from_ideal = 0;
	//size_t psl_of_existed = ht_get_psl(ht,index);

	while(psl_from_ideal <= ht_get_psl(ht,index)){
		dprintf("{In while..\n");
		if(ht_compare_key_at_index(ht,index,key)){
			//Change the value
			dprintf("This is our old content, changing the value!\n");
			ht_change_value_at_index(ht,index,value);
			return;
		}

		index = ht_next_index(ht,index);
		++psl_from_ideal;
		dprintf("end of while}\n");
	}

	if(ht_try_rehash(ht)){
		dprintf("Updated table size:%lu | cap:%lu\n",ht->len,ht->cap);
		index = ht_index_from_hash(ht,hash); dprintf("index :%lu\n",index);
		psl_from_ideal = 0;
		while(psl_from_ideal <= ht_get_psl(ht,index)){
			index = ht_next_index(ht,index);
			++psl_from_ideal;
		}
	}

	dprintf("After everything index is :%lu\n",index);

	if(ht_is_index_empty(ht,index)){
		dprintf("Empty bucket found at %lu\n",index);
		pair p = {key,value};
		ht_set_new_pair_at_index(ht,index,&p);
	}
	else{
		//dprintf("XXXXXXXXXXXXXXXXXXXXx\n"); ht_print(ht);dprintf("XXXXXXXXXXXXXXXXXXXXx\n");
		dprintf("Inserting value into %lu\n",index);
		++strdup_ctr;
		pair p = {strdup(key),value};
		psl_from_ideal = ht_get_psl(ht,index)+1;
		ht_swap_pair_at_index(ht,index,&p);
		dprintf("Now pair is '%s':%d\n",p.key,p.value );
		index = ht_next_index(ht,index);

		size_t count = 0;
		
		while(p.key != NULL){
			dprintf("psl now :%ld\n",psl_from_ideal);
			if(psl_from_ideal > ht_get_psl(ht,index)){
				//if(psl_from_ideal > 5) assert(0);

				dprintf("Swapping at index :%lu \n",index);
				psl_from_ideal = ht_get_psl(ht,index);
				ht_swap_pair_at_index(ht,index,&p);
				//psl_from_ideal = ht_get_psl(ht,index)+1;
			}
			dprintf("p = {'%s':%d}\n",p.key,p.value);
			++psl_from_ideal;
			index = ht_next_index(ht,index);
			++count;
			if(MAX_ALLOWED_SWAP != -1 && count > MAX_ALLOWED_SWAP){
				printf("MAX_ALLOWED_SWAP:%ld exceeded\n",count );
				//ht_rehash(ht);
				//--count;
				rehash_at_next_insertion = true;
				//assert(0);
			}
		}
		//printf("Setting value at index :%lu \n",index);
		//ht_set_pair_at_index(ht,index,&p);
		//ht_swap_pairs(ht,index,index+psl_from_ideal);
//          insert_value(ibucket, dist_from_ideal_bucket, bucket_entry::truncate_hash(hash), std::forward<Args>(value_type_args)...);		
	}

	ht->len++;
	dprintf("Length is increased:%lu\n",ht->len);
}


int ht_lookup(hashtable * ht, const char * key){
	size_t index = ht_index(ht,key);
	//printf("Found index :%lu\n",index );
	pair * p  = ht_get_pair(ht,index);
	if(p->key != NULL){
		for(uint8_t i = 0; i < ht->max_psl && strcmp(p->key,key) != 0; ++i){
			index = ht_next_index(ht,index);
			p  = ht_get_pair(ht,index);
		}
		dprintf("Lookup given key:'%s', founded :'%s' value:%d\n",key,p->key,p->value);
		return p->value;
	}
	else{
		printf("Key[%s] is not in the map\n",key);
		assert(0);
	}
	
}






//================================================================================================================
void fill_ht_from_file(hashtable * ht){
	FILE * fp = fopen("dictionary.txt","r");
	char buf[1000] = {0};
	int c = 0;
	int i = 0;

	int row = 0;

	for(;;){
		while((c = fgetc(fp)) != EOF){
			if(c == '\n'){
				++row;
				buf[i] = 0;
				c = 0;
				i = 0;

				break;
			}
			else if(c == '\r'){
				;
			}
			else{
				buf[i++] = c; 
			}
		}
		if(c == EOF) break;


		//dprintf("%s(%ld) @ %d\n",buf,strlen(buf),row);

		ht_insert(ht,buf,row);
		//ht_insert(ht,buf,row);
		if(0 && row > 56){
			//printf("^^^^^^^^^^^^^BREAK^^^^^^^^^^^^^^^ strdup_ctr:%d\n",strdup_ctr);
			break;
		}
	}
	printf("....\n");
	fclose(fp);		
}

void lookup_ht_from_file(hashtable * ht){
	FILE * fp = fopen("dictionary.txt","r");
	char buf[1000] = {0};
	int c = 0;
	int i = 0;

	int row = 0;

	for(;;){
		while((c = fgetc(fp)) != EOF){
			if(c == '\n'){
				++row;
				buf[i] = 0;
				c = 0;
				i = 0;

				break;
			}
			else if(c == '\r'){
				;
			}
			else{
				buf[i++] = c; 
			}
		}
		if(c == EOF) break;
		ht_lookup(ht,buf);
		//printf("Looking up ['%s'] = '%d'\n",buf,ht_lookup(ht,buf));
		//ht_insert(ht,buf,row);
		if(0 && row > 100000){
			//printf("^^^^^^^^^^^^^BREAK^^^^^^^^^^^^^^^ strdup_ctr:%d\n",strdup_ctr);
			break;
		}
	}
	printf("....\n");
	fclose(fp);		

}



int main(){

	hashtable * ht = ht_new(0);
	fill_ht_from_file(ht);
	ht_print(ht);
	printf("-------------\n");
	lookup_ht_from_file(ht);

	ht_clear(ht);

	return 0;
}
//================================================================================================================


void ht_insert_value_on_rehash(hashtable * ht, size_t index, ssize_t psl_from_ideal,  pair * p){
	for(;;){
	    if(psl_from_ideal > ht_get_psl(ht,index)){
	        if(ht_is_index_empty(ht,index)){
	        	dprintf("ht_insert_value_on_rehash:index %lu empty\n",index);
	        	ht_set_pair_at_index(ht,index,p);
	            //m_buckets[ibucket].set_value_of_empty_bucket(dist_from_ideal_bucket, hash, std::move(value));
	            return;
	        }
	        else {
	        	dprintf("ht_insert_value_on_rehash:swapping pairs at index %lu\n",index);
	        	ht_swap_pair_at_index(ht,index,p);
	            //m_buckets[ibucket].swap_with_value_in_bucket(dist_from_ideal_bucket, hash, value);
	        }
	    }
	    
	    ++psl_from_ideal;
	    index = ht_next_index(ht,index);
	}
}

void ht_rehash(hashtable * ht){
	if(ht->cap == 0) ht->cap = 1;
	size_t old_cap = ht->cap;
	ht->cap <<= 1; //multiply by two
	hashtable * new_ht = ht_new(ht->cap);
	//pair * new_kv_list = (pair *) malloc(ht->cap*sizeof(pair));
	ht->max_psl = 0;
	for(size_t i = 0; i<old_cap; ++i){
		pair * p = ht_get_pair(ht,i);
		if(p->key == NULL) continue;
		size_t index = ht_index(new_ht,p->key);
		dprintf("Moving old key '%s':%d|old location:%lu | new location :%lu\n",p->key,p->value,i,index);
		//Do not copy the content just borrow the pointers
		ht_insert_value_on_rehash(new_ht,index,0,p);
	}

	free(ht->kv_list);
	ht->kv_list = new_ht->kv_list;
	free(new_ht);
}

void ht_swap_pair_at_index(hashtable * ht,size_t index, pair * p){
	pair * at_index = ht_get_pair(ht,index);

	pair temp = {at_index->key,at_index->value};
	memmove(at_index,p,sizeof(pair));
	memmove(p,&temp,sizeof(pair));
	//at_index->value = p->value;
	//at_index->key = p->key;

	//p->key = temp.key;
	//p->value = temp.value;

}

void ht_set_new_pair_at_index(hashtable * ht,size_t index, pair * p){
	pair * at_index = ht_get_pair(ht,index);
	dprintf(".........................................................................\n");
	at_index->key = strdup(p->key);
	at_index->value = p->value;
	++strdup_ctr;
	dprintf("New pair is written at %lu : %s:%d\n",index,p->key,p->value );
}

void ht_set_pair_at_index(hashtable * ht,size_t index, pair * p){
	pair * at_index = ht_get_pair(ht,index);
	memcpy(at_index,p,sizeof(pair));
}


bool ht_is_index_empty(hashtable *ht, size_t index){
	pair * p = ht_get_pair(ht,index);
	return p->key == NULL;
}

bool ht_try_rehash(hashtable * ht){
	if(ht->len >= (REHASH_LOAD_FACTOR * ht->cap || rehash_at_next_insertion) ){
		dprintf("________rehash!___________\n");
		ht_rehash(ht);
		rehash_at_next_insertion = false;
		return true;
	}

	return false;
}

size_t ht_next_index(hashtable * ht, size_t index){
	return (index + 1) & (ht->cap-1);
}

void ht_change_value_at_index(hashtable * ht, size_t index, int value){
	pair * p = ht_get_pair(ht,index);
	p->value = value;
}

bool ht_compare_key_at_index(hashtable * ht, size_t index, const char * key){
	pair * p = ht_get_pair(ht,index);
	if(p->key == NULL) return false;
	dprintf("Comparing :%s<->%s\n",key,p->key);
	return !strcmp(p->key,key);
}

ssize_t ht_abs(ssize_t i){
	return i >=0 ? i : -i;
}

ssize_t ht_calculate_psl(hashtable *ht, size_t index, size_t index_from_hash){
	if(index >= index_from_hash)
		return index - index_from_hash;
	else
		return ht->cap - index_from_hash + index;
}

ssize_t ht_get_psl(hashtable *ht, size_t index){
	pair * p = ht_get_pair(ht,index);
	if(p->key == NULL){
		dprintf("PSL returning:-1\n");
		return -1;
	}
	dprintf("index:%ld, ht_index:%ld\n",index,ht_index(ht,p->key) );
	ssize_t val = ht_calculate_psl(ht,index,ht_index(ht,p->key));

	if(val > ht->max_psl){
		dprintf("\t\t:::::::::max psl updated :%ld|%ld\n",val,ht->max_psl );
		dprintf(">> p->key:%s\n",p->key);
		ht->max_psl = val;
	}
	if(MAX_ALLOWED_PSL != -1 && ht->max_psl > MAX_ALLOWED_PSL){
		//ht_rehash(ht);
		rehash_at_next_insertion = true;
		//assert(0);
	}

	dprintf("PSL returning!:%ld\n",val );
	return val;

}

size_t ht_hash(hashtable * ht, const char * key){
	#ifdef CPP_HASH
		return str_hash(key);
	#else
    	unsigned long hash = 5381;
    	int c;

    	while (c = *key++)
        	hash = ((hash << 5) + hash) + c; // hash * 33 + c 

    	return hash;
	#endif
}

size_t ht_index_from_hash(hashtable * ht,size_t hash){

	return ht->cap != 0 ?  hash & (ht->cap - 1) : 0;
}

size_t ht_index(hashtable * ht, const char * key){
	return ht_index_from_hash(ht,ht_hash(ht,key));
}

pair * ht_get_pair(hashtable * ht, size_t index){
	return &ht->kv_list[index];
}

hashtable * ht_new(size_t cap){
	hashtable * ht = (hashtable * )malloc(sizeof(hashtable));
	ht->kv_list = (pair *) malloc(cap*sizeof(pair));
	ht->cap = cap;
	while(cap--){
			pair * p = &ht->kv_list[cap];
			p->key = NULL;
	}
	ht->len = 0;
	ht->max_psl = 0;
	return ht;
}




void ht_print(hashtable * ht){
	for (int i = 0; i < ht->cap; ++i){
		pair * p = ht_get_pair(ht,i);
		if(p->key != NULL){
			ssize_t psl = ht_calculate_psl(ht,i,ht_index(ht,p->key));
			
			//if(psl>5)
				printf("(%d)['%s'] = %d | psl:%ld\n",i,p->key,p->value,psl);
		}
	}
	printf("Table capacity :%lu | table->len %lu | max_psl:%ld\n",ht->cap,ht->len,ht->max_psl);
}


void ht_clear(hashtable * ht){

	for (size_t i = 0; i < ht->cap; ++i){
		pair * p = &ht->kv_list[i];
		if(p->key != NULL){
			//dprintf("Deleting %lu | '%s'\n",i,p->key);
			free((char*)p->key);
		}		
	}


	ht->len = 0;
	free(ht->kv_list);
	free(ht);
}

*/