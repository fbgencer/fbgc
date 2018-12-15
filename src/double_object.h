#ifndef FBGC_DOUBLE_OBJECTH
#define FBGC_DOUBLE_OBJECTH

struct fbgc_double_object{
    struct fbgc_object base;
    double content;
};

struct fbgc_object * new_fbgc_double_object(double);
struct fbgc_object * new_fbgc_double_object_from_str(const char *);
struct fbgc_object * new_fbgc_double_object_from_substr(const char *,const char *);

void print_fbgc_double_object(struct fbgc_object *);
void free_fbgc_double_object(struct fbgc_object * );

struct
fbgc_object * new_fbgc_double_object(double db_content){
	struct fbgc_double_object *dbo =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = db_content;  
    return (struct fbgc_object*) dbo;
}

struct
fbgc_object * new_fbgc_double_object_from_str(const char * db_str){
  	return new_fbgc_double_object(strtod (db_str,NULL));
}

struct
fbgc_object * new_fbgc_double_object_from_substr(const char * db_str_begin,const char * db_str_end){
	//now we don't wanna check inf or overlfow issue but later we are going to check them
  	return new_fbgc_double_object(strtod (db_str_begin, &db_str_end));
}


void print_fbgc_double_object(struct fbgc_object * obj){
    struct fbgc_double_object * dbo = (struct fbgc_double_object*) obj;
    printf("[Double] :{%f}\n",dbo->content);   
}

void free_fbgc_double_object(struct fbgc_object * obj){
    struct fbgc_double_object *dbo = (struct fbgc_double_object*) obj;
    #ifdef DEBUG
    	//fprintf(fbgc_log_file,"Double Deleting..");
    #endif
    free(dbo);
}



#endif

//for db array

/*
struct 
fbgc_object * add_fbgc_double_object (struct fbgc_object *obj, double inc_content){
    struct fbgc_double_object *dbo = (struct fbgc_double_object*) obj;
    if(obj == NULL){
        dbo =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
        dbo->base.type = DOUBLE;
        dbo->base.next = NULL;
        dbo->content = NULL;   
        dbo->size = 0;
    }

    dbo->content = (double*) realloc(dbo->content,sizeof(double) * ++dbo->size);
    *(dbo->content+dbo->size-1) = inc_content;
    return (struct fbgc_object*) dbo;
}*/
