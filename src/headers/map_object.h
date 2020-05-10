#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_map_pair{
    struct fbgc_object * key; //fbgc_str_object
    struct fbgc_object * value; //any fbgc_object
};

struct fbgc_map_object{ 
	struct fbgc_object base;
	size_t size;
	struct fbgc_map_pair * content;
	uint8_t max_psl;
	uint8_t rehash_load_factor : 7; // it must be percent, 50 means if the half of the table is filled then we will try rehashing
	uint8_t rehash_at_next_insertion : 1;
	
};


#define MAX_ALLOWED_PSL 5

extern const struct fbgc_object_property_holder fbgc_map_object_property_holder;

#define cast_fbgc_object_as_map(x)(((struct fbgc_map_object*) x))
#define sizeof_fbgc_map_object()(sizeof(struct fbgc_map_object)) 

#define capacity_fbgc_map_object(x)(capacity_fbgc_raw_memory(cast_fbgc_object_as_map(x)->content,sizeof(struct fbgc_map_pair) ))

#define size_fbgc_map_object(x)(cast_fbgc_object_as_map(x)->size)
#define max_psl_fbgc_map_object(x)(cast_fbgc_object_as_map(x)->max_psl)
#define content_fbgc_map_object(x)(cast_fbgc_object_as_map(x)->content)


struct fbgc_object * new_fbgc_map_object(size_t cap,uint8_t rlf);
struct fbgc_object * new_fbgc_map_object_exact(size_t cap, uint8_t rlf);

struct fbgc_object * fbgc_map_object_insert(struct fbgc_object * self_adr, struct fbgc_object * key, struct fbgc_object * value);



struct fbgc_object * fbgc_map_object_remove(struct fbgc_object * self, struct fbgc_object * key);
struct fbgc_object * fbgc_map_object_remove_str(struct fbgc_object * self, const char * key);
struct fbgc_object * fbgc_map_object_remove_substr(struct fbgc_object * self, const char * key1,const char * key2);


ssize_t fbgc_map_object_get_key_index(struct fbgc_object * self, struct fbgc_object * key);
ssize_t fbgc_map_object_get_key_index_str(struct fbgc_object * self, const char * key);
ssize_t fbgc_map_object_get_key_index_substr(struct fbgc_object * self,const char *key1, const char *key2);


struct fbgc_object * fbgc_map_object_lookup(struct fbgc_object * self, struct fbgc_object * key);
struct fbgc_object * fbgc_map_object_lookup_str(struct fbgc_object * self, const char * key);
struct fbgc_object * fbgc_map_object_lookup_substr(struct fbgc_object * self, const char * key1,const char * key2);


bool fbgc_map_object_does_key_exist(struct fbgc_object * self, struct fbgc_object * key);
bool fbgc_map_object_does_key_exist_str(struct fbgc_object * self, const char * key);
bool fbgc_map_object_does_key_exist_substr(struct fbgc_object * self, const char * key1,const char * key2);

struct fbgc_object * fbgc_map_object_get_key_substr(struct fbgc_object * self, const char * key1,const char * key2);
struct fbgc_object * fbgc_map_object_get_value_by_index(struct fbgc_object * self,size_t index);


void fbgc_map_object_set_exact_size(struct fbgc_object * self);



uint8_t print_detailed_fbgc_map_object(struct fbgc_object * obj);
uint8_t print_fbgc_map_object(struct fbgc_object * obj);

/*! \cond DOCUMENT_EVERYTHING
	Lets not document this part, just log definitions nothing important 
*/
#ifdef LOG_MAP_OBJECT
#define MAP_OBJECT_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define MAP_OBJECT_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _MAP_OBJECT_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _MAP_OBJECT_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define MAP_OBJECT_LOGV(...)
#define MAP_OBJECT_LOGD(...)
#define _MAP_OBJECT_LOGV(...)
#define _MAP_OBJECT_LOGD(...)
#endif


#ifdef  __cplusplus
}
#endif

#endif