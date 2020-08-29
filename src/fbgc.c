#include "fbgc.h"

#include "../cmodules/cmodules.h"

//Forward declarations
void print_logo();


struct fbgc_object * current_field = NULL;

struct fbgc_object __nil__ = {.type = NIL};
struct fbgc_object * __fbgc_nil = &__nil__;


static void compile_file(struct fbgc_object * main_field,const char *file_name){

	clock_t begin,end;
	double parser_time,interpreter_time;

	assert(main_field);

	begin = clock();
	 
   int par = parse_file(&main_field,file_name);

	
	
	end = clock();
	parser_time = (double)(end - begin) / CLOCKS_PER_SEC; 

	 
	begin = clock();
	if(par) 
		par = interpreter(&main_field); 
	end = clock();
	interpreter_time = (double)(end - begin) / CLOCKS_PER_SEC; 


	#ifdef TIME_DEBUG
	cprintf(110,"\n\n\n\n\n\n[=======================================================================]\n");   
	printf("Execution time [PARSER + LEXER] :%f\n",parser_time);    
	printf("Execution time [INTERPRETER] :%f\n",interpreter_time);  
	printf("Total exc. time %f\n",parser_time+interpreter_time);  
	#endif

	  
}




struct fbgc_object * fbgc_load_module(const char * module_name,const char * fun_name, uint8_t load_key){
		
	//load_key == 0, just return the module
	//load_key == 1, load all and return : global
	//load_key == 2, load specific and return, requires fun_name : local naming

	assert(current_field != NULL);

	const struct fbgc_object_property_holder * p = NULL;
	for(uint8_t i = 0; i<sizeof(__property_holders)/sizeof(__property_holders[0]); ++i){
		p = __property_holders[i];
		int8_t w = _find_property(p->bits,_BIT_NAME);
		if(w != -1){
			//call initializer
			if(strcmp(module_name,p->properties[w].name) == 0){
				break;
			}
			
		}
	}

	if(p == NULL){
		return NULL;
	}
	//call function initializer
	int8_t w = _find_property(p->bits,_BIT_INITIALIZER);
	if(w != -1){
		//call initializer
		p->properties[w].initializer();
	}

	if(load_key != 0){
		w = _find_property(p->bits,_BIT_CONSTRUCTOR);
		if(w != -1){
			//if this module has constructor we will only load its constructor into global scope
			const struct fbgc_cfunction * cc = p->properties[w].constructor;
			if(load_key == 2 && strcmp(fun_name,cc->name) != 0){
				FBGC_LOGE("%s could not be found in %s",fun_name,module_name);
				return NULL; 
			}

			//if load key == 1 or == 2, it does not matter we checked name of the constructor
			add_variable_in_field_object(current_field,cc->name,new_fbgc_cfun_object(cc->function));
		}
		else{

			//no constructor, then this is a normal cmodule like math
			w = _find_property(p->bits,_BIT_METHODS);
			if(w != -1){

				const struct fbgc_object_method * methods = p->properties[w].methods;
				uint8_t len = methods->len;
				while(len--){
					const char * str = methods->method[len].name;
					if(load_key == 2 && strcmp(str,fun_name) != 0){
						continue; //No match continue, if load_key is not 2 then we will add everything
					}
					add_variable_in_field_object(current_field,str,new_fbgc_cfun_object(methods->method[len].function));
				}
			}
			w = _find_property(p->bits,_BIT_MEMBERS);
			if(w != -1){

				const struct fbgc_object_member *members = p->properties[w].members;
				uint8_t len = members->len;
				
				while(len--){
					const char * str = members->member[len].name;
					if(load_key == 2 && my_strcmp(str,fun_name) != 0){
						continue;
					}

					//No self is provided, user must know this
					add_variable_in_field_object(current_field,str,members->member[len].function(NULL,NULL));
				}
			}				
		}
	}

	return new_fbgc_cmodule_object(p);

}

struct fbgc_object * fbgc_load_file(const char * file_name,const char * fun_name, uint8_t load_key){

	/*struct fbgc_object * prev_field = current_field;	

	struct fbgc_object * file_field = new_fbgc_field_object();
	if(parse_file(&file_field,file_name))
		interpreter(&file_field);
	else 
		assert(0);

	current_field = file_field;

	if(load_key != 0){
		//now merge two field objects, new field and prev field
		//prev_field_local_array_object
		struct fbgc_object * prev_ao = cast_fbgc_object_as_field(prev_field)->locals;
		//current field local array object
		struct fbgc_object * current_ao = cast_fbgc_object_as_field(current_field)->locals;

		unsigned int last_j = 0;

		char atleast_matched = 0;
		

		for(unsigned int i = 0;  i<size_fbgc_array_object(current_ao); ++i){	
			struct fbgc_identifier * ctemp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(current_ao,i);
			struct fbgc_identifier * ptemp_id;
			char found = 0;

			//cprintf(111,"ctemp_id name:[%s]fun:[%s]\n",content_fbgc_str_object(ctemp_id->name),fun_name);
			//cprintf(100,"result %d\n",my_strcmp(fun_name,content_fbgc_str_object(ctemp_id->name)));
			if(load_key == 2 && my_strcmp(fun_name,content_fbgc_str_object(ctemp_id->name)) ){
				//if no match
				continue;
			}

			//so matched with the given function name
			atleast_matched = 1;

			for(unsigned int j = last_j; j<size_fbgc_array_object(prev_ao); ++j){
				ptemp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(prev_ao,j);
				//cprintf(111,"ptemp_id name :"); print_fbgc_object(ptemp_id->name);
				//cprintf(111," | ctemp_id name :"); print_fbgc_object(ctemp_id->name);
				
				if(!my_strcmp(content_fbgc_str_object(ptemp_id->name),content_fbgc_str_object(ctemp_id->name))){
					//change the content of ptemp_id object with the second one
					found = 1;
					last_j = ++j;
					//cprintf(100," (Equal)\n");
					break;
				}
			}

			if(found){
				//cprintf(101,"Found!\n");
				ptemp_id->content = ctemp_id->content;
			}else{			
				//cprintf(101,"creating new local\n");
				prev_ao = push_back_fbgc_array_object(prev_ao,ctemp_id);	
			}
			if(load_key == 2) break;
		}
		cast_fbgc_object_as_field(prev_field)->locals = prev_ao;
		

		if(!atleast_matched){
			cprintf(100,"Error '%s' could not be found in '%s'\n",fun_name,file_name);
		}
		assert(atleast_matched);
	}

	current_field = prev_field;
	return file_field;*/
	return NULL;
}


void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
}


int8_t parse_tuple_content(struct fbgc_cfun_arg * cfun_arg,const char * format, ...){

	
	//cprintf(111,"parsing tuple sz:%d | format'%s'\n",sz,format);

	//Verilecek hatalar
	//eğer hiç arg verilmemişse argdaki objelerin sadece tipleri kontrol edilecek
	//eğer arg varsa mümkün olan arg'a kadar arrayin o objesi assign edilecek
	//argüman sayısı uyuşmazsa match hatası verilip null dönülecek
	// parse(a,b,"ij");

	struct fbgc_object ** c = cfun_arg->arg;
	int sz = cfun_arg->argc;

    va_list args;
    va_start(args, format);
    
    uint8_t have_arg = 0;
    if(*format == '!'){
    	//so only verification, assuming no arg is passed
    	//cprintf(110,"Not assignment, only verification\n");
    	++format;
    }
    else have_arg = 1;


    uint8_t ok = 0;
    int8_t level = 0;

    const char * level_start = format;

    uint8_t count_match = 0;
    
	for(int i = 0; i<sz; ++i){
		struct fbgc_object * co = c[i];
		//cprintf(111,"Current[%d] type :%s | format:%c\n",i,objtp2str(co),*format);

	switch(*format){
		case 'b':{
			ok = (co->type == LOGIC);
			//cprintf(001,"b:logic:%d\n",ok);
			break;
		}
		case 'i':{
			ok = (co->type == INT);
			//cprintf(001,"i:int:%d\n",ok);
			break;
		}
		case 'd':{
			ok = (co->type == DOUBLE);
			//cprintf(001,"d:double:%d\n",ok);
			break;
		}
		case 'j':{
			ok = (co->type == COMPLEX);
			//cprintf(001,"c:complex:%d\n",ok);
			break;
		}
		case 's':{
			ok = (co->type == STRING);
			//cprintf(001,"s:string:%d\n",ok);
			break;
		}
		case 'm':{
			ok = (co->type == MATRIX);
			////cprintf(001,"m:matrix:%d\n",ok);
			break;
		}
		case 't':{
			ok = (co->type == TUPLE);
			//cprintf(001,"t:tuple:%d\n",ok);
			break;
		}
		case 'r':{
			ok = (co->type == RANGE);
			//cprintf(001,"r:range:%d\n",ok);
			break;
		}		
		case '.':{
			ok = (co->type == CSTRUCT);
			//cprintf(001,".:cstruct:%d\n",ok);
			break;	
		}
		case 'o':{
			//any type 
			ok = 1;
			//cprintf(001,"o:any:%d\n",ok);
			break;
		}
		case '|':{
			if(ok)
				goto end_of_parse_tuple_content;
			
			++format;
			//cprintf(001,"or\n");
			continue;
		}
		case ':':{
			//cprintf(001,"colon\n");
			if(ok == 1){
				//No need to print error message
			}
			else{
				const char * er_start = ++format;
				while(*er_start != '\0' && *er_start != '|')
					++er_start;
				//Now we have error message..

			}
			break;
		}
		case '*':{
			//cprintf(001,"star\n");
			break;
		}
		case '+':{
			//cprintf(001,"plus\n");
			if(count_match){
				format = level_start;
				continue;
			}
			else ok = 0;
			break;
		}
		case '\0':{
			//cprintf(010,"end of format!\n");
			if(ok){
				//printf("args is valid!\n");
			}
			else {
				FBGC_LOGE("args is not valid\n");
				count_match = 0;
			}

			goto end_of_parse_tuple_content;
			
		}
		default:{
			assert(0);
		}
	}

	if(ok == 1){
		++count_match;
		if(have_arg){
			//cprintf(111,"We have args, now assigning them\n");
			struct fbgc_object ** o = va_arg(args, struct fbgc_object **);
			*o = c[i];
		}
		ok = 0;
	}
	else{
		
		while(*format != '\0' && *format != '|')
			++format;

		if(*format == '\0'){
			count_match = 0;
			FBGC_LOGE("Arg match error!\n");
			break;
		}
		else{
			//printf("----------------\n");
			++level;
			count_match = 0;
			i = -1;
		}
	}

	++format;
	}


	end_of_parse_tuple_content:
	va_end(args);
	level = (count_match > 0) ? level : -1;
	//cprintf(111,"Returning level:%d\n",level);
	return level;
	
}

int8_t parse_keywordargs_content(struct fbgc_cfun_arg * cfun_arg,const char ** keywords, const char * format, ...){

	
	//cprintf(111,"parsing tuple sz:%d | format'%s'\n",sz,format);

	//Verilecek hatalar
	//eğer hiç arg verilmemişse argdaki objelerin sadece tipleri kontrol edilecek
	//eğer arg varsa mümkün olan arg'a kadar arrayin o objesi assign edilecek
	//argüman sayısı uyuşmazsa match hatası verilip null dönülecek
	// parse(a,b,"ij");
	struct fbgc_object * kwargs = NULL;
	if(cfun_arg->kwargs_flag){
		kwargs = cfun_arg->arg[cfun_arg->argc];
	}
	else return 0;

	struct fbgc_object ** c = cfun_arg->arg;
	size_t sz = size_fbgc_map_object(kwargs);

    va_list args;
    va_start(args, format);
    
    uint8_t have_arg = 0;
    if(*format == '!'){
    	//so only verification, assuming no arg is passed
    	//cprintf(110,"Not assignment, only verification\n");
    	++format;
    }
    else have_arg = 1;

    //XXXXXXXXXXXXXXXXXXXXXXXXX
    //Give error for undefined keyword entries!
    //Now it is missing
    //XXXXXXXXXXXXXXXXXXXXXXXXX


    uint8_t ok = 0;
    int8_t level = 0;

    const char * level_start = format;

    uint8_t count_match = 0;
    
    for(const char * ckw = keywords[0]; ckw != NULL; ckw = *(++keywords)){
    	struct fbgc_object * co = fbgc_map_object_lookup_str(kwargs,ckw);
    	if(co == NULL){
			struct fbgc_object ** o = va_arg(args, struct fbgc_object **);
			*o = NULL;
    		continue; 		
    	}

	switch(*format){
		case 'b':{
			ok = (co->type == LOGIC);
			//cprintf(001,"b:logic:%d\n",ok);
			break;
		}
		case 'i':{
			ok = (co->type == INT);
			//cprintf(001,"i:int:%d\n",ok);
			break;
		}
		case 'd':{
			ok = (co->type == DOUBLE);
			//cprintf(001,"d:double:%d\n",ok);
			break;
		}
		case 'j':{
			ok = (co->type == COMPLEX);
			//cprintf(001,"c:complex:%d\n",ok);
			break;
		}
		case 's':{
			ok = (co->type == STRING);
			//cprintf(001,"s:string:%d\n",ok);
			break;
		}
		case 'm':{
			ok = (co->type == MATRIX);
			////cprintf(001,"m:matrix:%d\n",ok);
			break;
		}
		case 't':{
			ok = (co->type == TUPLE);
			//cprintf(001,"t:tuple:%d\n",ok);
			break;
		}
		case 'r':{
			ok = (co->type == RANGE);
			//cprintf(001,"r:range:%d\n",ok);
			break;
		}		
		case '.':{
			ok = (co->type == CSTRUCT);
			//cprintf(001,".:cstruct:%d\n",ok);
			break;	
		}
		case 'o':{
			//any type 
			ok = 1;
			//cprintf(001,"o:any:%d\n",ok);
			break;
		}
		case '|':{
			if(ok)
				goto end_of_parse_tuple_content;
			
			++format;
			//cprintf(001,"or\n");
			continue;
		}
		case ':':{
			//cprintf(001,"colon\n");
			if(ok == 1){
				//No need to print error message
			}
			else{
				const char * er_start = ++format;
				while(*er_start != '\0' && *er_start != '|')
					++er_start;
				//Now we have error message..

			}
			break;
		}
		case '*':{
			format = level_start;
			continue;
		}
		case '+':{
			//cprintf(001,"plus\n");
			if(count_match){
				format = level_start;
				continue;
			}
			else ok = 0;
			break;
		}
		case '\0':{
			//cprintf(010,"end of format!\n");
			if(ok){
				//printf("args is valid!\n");
			}
			else {
				FBGC_LOGE("args is not valid\n");
				count_match = 0;
			}

			goto end_of_parse_tuple_content;
			
		}
		default:{
			assert(0);
		}
	}

	if(ok == 1){
		++count_match;
		if(have_arg){
			//cprintf(111,"We have args, now assigning them\n");
			struct fbgc_object ** o = va_arg(args, struct fbgc_object **);
			*o = co;
		}
		ok = 0;
	}
	else{
		
		while(*format != '\0' && *format != '|')
			++format;

		if(*format == '\0'){
			count_match = 0;
			FBGC_LOGE("Arg match error!\n");
			break;
		}
		else{
			//printf("----------------\n");
			++level;
			count_match = 0;
		}
	}

	++format;
	}


	end_of_parse_tuple_content:
	va_end(args);
	level = (count_match > 0) ? level : -1;
	//cprintf(111,"Returning level:%d\n",level);
	return level;
	
	return 0;
}


static unsigned long fhash(const void * keyp,size_t len){
	unsigned long hash = 5381;
	
	const char * key = (const char * )keyp;
	while(len--){
		char c = *key++;
		hash = ((hash << 5) + hash) + c; // hash * 33 + c
	}
	return hash;
}

/*
	GC 

*/
/*
Senaryolar

struct{
	struct fbgc_object base;
	int size;
	struct fbgc_object ** contents;
	
	veya 

	struct fbgc_object * io;
	struct fbgc_object * do;
	struct fbgc_object * ao;

} tuple;
Gc fonksiyonu içinde contents array olarak marklanır, burada contents queue içine atılır ve daha sonra 
contentin referanslarına gidilerek onların gc fonksiyonları çağırılır.

struct{
	struct fbgc_object base;
	int size;
	int ** contents;
} int_arrays;

Gc fonksiyonu içinde pointer obje tutmadığı için pointer_register fonksiyonu ile marklanır
Mark yerine burada register edilen pointer


struct{
	struct fbgc_object base;
	int size;
	int * contents;
} int_arrays2;

struct{
	int x;
	int y;
	double * z;
}foo;

*/




#ifndef MODULE_TEST
int main(int argc, char **argv){

	#ifdef FBGC_LOGO
		print_logo();
	#endif   

//******************************************************************
	initialize_fbgc_memory_block();

	/*int * lol = (int * ) fbgc_malloc(sizeof(int)*10);
	int val[10] = {10,20,30,40,50,60,70,80,90,100};
	memcpy(lol,val,sizeof(int)*10);


	int val2[10] = {1,2,3,4,5,6,7,8,9,10};
	int * lol2 = (int * ) fbgc_malloc(sizeof(int)*20);
	memcpy(lol2,val2,sizeof(int)*10);

	print_fbgc_memory_block();
	

	cprintf(100,"capacity lol :%ld\n",capacity_fbgc_raw_memory(lol,sizeof(int)));
	cprintf(100,"capacity lol2 :%ld\n",capacity_fbgc_raw_memory(lol2,sizeof(int)));*/
	
	
	
	//struct fbgc_object * io = new_fbgc_int_object(10);
	//struct fbgc_object * to = new_fbgc_tuple_object(0);
	//push_back_fbgc_tuple_object(to,io);
	//struct fbgc_object * io1 = new_fbgc_int_object(20);
	//struct fbgc_object * io2 = new_fbgc_int_object(30);

	//printf("sizeof ? %lu\n",size_of_fbgc_object(io));


	// struct fbgc_queue * q = new_fbgc_queue(2,sizeof(int));
	// int a = 5;
	// fbgc_queue_push_back(q,&a);
	// a = 6;
	// fbgc_queue_push_back(q,&a);
	// fbgc_queue_pop(q);
	// a = 7;
	// fbgc_queue_push_back(q,&a);
	// fbgc_queue_pop(q);
	// a = 8;
	// fbgc_queue_push_back(q,&a);
	
	// //a = 9;
	// //fbgc_queue_push_back(q,&a);
	// //fbgc_queue_pop(q);


	// // struct fbgc_raw_buffer * rb = cast_from_raw_buffer_data_to_raw_buffer(q->data);
	// // printf("rb cap = %d | data p:%p | rb%p\n",rb->capacity,q->data,rb );
	// // uint8_t * d = (uint8_t*)rb;
	// // for (int i = 0; i < sizeof(struct fbgc_raw_buffer)+rb->capacity; ++i)
	// // {	
	// // 	printf("%p[%d]:%02x|%d\n",d,i,*d,*d );
	// // 	d++;
	// // }

	// print_fbgc_memory_block();

	// printf("queue front %p | sz:%ld\n",fbgc_queue_front(q),size_fbgc_queue(q));

	//printf("sctr : %d\n",global_interpreter_packet->sctr );

	//struct fbgc_object * to = new_fbgc_tuple_object(0);

	// struct fbgc_object * to3 = new_fbgc_tuple_object(2);

	// struct fbgc_object * io = new_fbgc_int_object(10);
	// struct fbgc_object * io2 = new_fbgc_int_object(11);
	// struct fbgc_object * io3 = new_fbgc_int_object(12);
		

	
	// struct fbgc_object * to2 = new_fbgc_tuple_object(2);
	
	// push_back_fbgc_tuple_object(to,io);
	// push_back_fbgc_tuple_object(to,io2);
	// push_back_fbgc_tuple_object(to,io3);
	// push_back_fbgc_tuple_object(to,to2);
	// push_back_fbgc_tuple_object(to2,io2);
	// push_back_fbgc_tuple_object(to2,to3);
	// push_back_fbgc_tuple_object(to3,io3);
	// push_back_fbgc_tuple_object(to3,io);


	/*struct fbgc_object * s = new_fbgc_int_object(0);
	struct fbgc_object * e = new_fbgc_int_object(10);
	struct fbgc_object * ro = new_fbgc_range_object(s,e);

	push_back_fbgc_tuple_object(to,ro);*/

	
	// typedef struct foo{
	// 	uint8_t x;
	// 	uint8_t * ptr;
	// }foo;

	// foo pp;
	// pp.x = 55;
	//pp.ptr = (uint8_t *)fbgc_malloc(10);
	
	// struct fbgc_int_object *into =  (struct fbgc_int_object*) fbgc_malloc_object(sizeof_fbgc_int_object()*3);
	// for(int i = 0; i<3; ++i){
	// 	into[i].base.type = INT;
	// 	into[i].content = 10*i; 
	// }
	

	// //set_raw_buffer_gc_dark(pp.ptr);

	// //fbgc_gc_register_pointer(&pp.ptr,pp.ptr,sizeof(foo*),sizeof(foo));

	// //print_fbgc_memory_block();
	// fbgc_gc_init(NULL);
	// fbgc_gc_mark_pointer(into,3);

	// fbgc_gc_mark();

	// print_fbgc_memory_block();

	/*foo ** pbuffer = (foo **) fbgc_malloc(sizeof(foo*)*2);
	fbgc_gc_register_pointer(pbuffer,2,sizeof(foo*),0);

	

	//struct traceable_pointer_entry * tpe = &(fbgc_gc.ptr_list.data[0]);
	//printf("tpe ptr %p\n",tpe->ptr );

	pbuffer[0] = (foo *) fbgc_malloc_static(sizeof(foo)*3);
	pbuffer[1] = NULL;

	pbuffer[0]->x[0] = 0xFB;
	pbuffer[0]->x[1] = 55;*/
	
	/*
 
	//
	//fbgc_gc_register_pointer(pbuffer[0],sizeof(foo),3,sizeof(uint8_t)*2);

	uint8_t * b1 = (uint8_t *) fbgc_malloc(sizeof(uint8_t)*5);
	uint8_t arr[5] = {5,10,15,20,30};
	memcpy(b1,arr,5);

	foo * fp = pbuffer[0];

	fp[0].ptr = b1;
	fp[0].x[0] = 10;
	fp[0].x[1] = 50;

	fp[1].ptr = b1;
	fp[1].x[0] = 20;
	fp[1].x[1] = 100;

	fp[2].ptr = b1;
	fp[2].x[0] = 30;
	fp[2].x[1] = 250;*/


	//fbgc_gc_init(io);
	//fbgc_gc_mark();

	
	//parse_string(&main_field,"x = 88\n");
	//cast_fbgc_object_as_field(main_field)->code;

	/*cprintf(100,"sizeof :%lu\n",sizeof(struct fbgc_map_object));

	struct fbgc_object * map = new_fbgc_map_object(2,50);
	fbgc_map_object_insert(map,new_fbgc_str_object("end"),new_fbgc_int_object(333));
	fbgc_map_object_insert(map,new_fbgc_str_object("sep"),new_fbgc_int_object(553));
	
	cprintf(110,"CHECK sep: %d\n",fbgc_map_object_does_key_exist_str(map,"sep"));

	print_detailed_fbgc_map_object(map);
	printf("\n");

	int x = -123;
	int y = -123;
	for(double i = -20; i< 20; ++i)
		printf("Hash val '%g' = %ld\n",i,fhash(&i,sizeof(i))%40);*/

	//fbgc_map_object_remove_str(map,"fbgencer");
	//cprintf(100,"fbgencer found at index :%ld\n",fbgc_map_object_get_key_index_str(map,"samsung"));

	//fbgc_map_object_set_exact_size(map);
	
	//fbgc_map_object_insert(map,new_fbgc_str_object("floler4"),new_fbgc_int_object(553));

	//print_detailed_fbgc_map_object(map);

	initialize_fbgc_symbol_table();
	current_field = new_fbgc_field_object();
	if(argc > 1)
	{   
		if(!strcmp(argv[1],"-s")){
			//compile_one_line(main_field,argv[2]);
		}
		else{
		   compile_file(current_field, argv[1]); 
		}
		//print_fbgc_memory_block();
		
	}
	else{
		compile_file(current_field, "ex.fbgc");
	   //realtime_fbgc(main_field);
	}

	free_fbgc_memory_block();
//******************************************************************

	return 0;
}


void print_logo(){
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen ( "./src/_fbgc_.txt" , "r" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	printf("%s\n",buffer);

	// terminate
	fclose (pFile);
	free (buffer);

}

#endif

