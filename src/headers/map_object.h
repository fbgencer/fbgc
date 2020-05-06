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
	size_t len;
	size_t cap;
	uint8_t max_psl;
	struct fbgc_map_pair content[0];
};

static const double REHASH_LOAD_FACTOR = 0.5;
static const size_t MAX_ALLOWED_PSL = -1;
static const size_t MAX_ALLOWED_SWAP = -1;

extern const struct fbgc_object_property_holder fbgc_map_object_property_holder;

#define cast_fbgc_object_as_map(x)(((struct fbgc_map_object*) x))
#define sizeof_fbgc_map_object()(sizeof(struct fbgc_map_object)) 

#define capacity_fbgc_map_object(x)(cast_fbgc_object_as_map(x)->cap)
#define length_fbgc_map_object(x)(cast_fbgc_object_as_map(x)->len)
#define max_psl_fbgc_map_object(x)(cast_fbgc_object_as_map(x)->max_psl)
#define content_fbgc_map_object(x)(cast_fbgc_object_as_map(x)->content)


struct fbgc_object * new_fbgc_map_object(size_t cap);

struct fbgc_object * fbgc_map_object_insert(struct fbgc_object ** self_adr, struct fbgc_object * key, struct fbgc_object * value);
struct fbgc_object * fbgc_map_object_lookup(struct fbgc_object * self, struct fbgc_object * key);
struct fbgc_object * fbgc_map_object_remove(struct fbgc_object ** self, struct fbgc_object * key);

struct fbgc_object * fbgc_map_object_lookup_with_string(struct fbgc_object * self, const char * key);
struct fbgc_object * fbgc_map_object_remove_with_string(struct fbgc_object * self, const char * key);

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