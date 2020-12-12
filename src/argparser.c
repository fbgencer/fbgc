#include "fbgc.h"
#include "cmodules.h"

struct fbgc_object * fbgc_load_module(const char * module_name,const char * fun_name, uint8_t load_key){
		
	//load_key == 0, just return the module
	//load_key == 1, load all and return : global
	//load_key == 2, load specific and return, requires fun_name : local naming

	assert(current_field != NULL);

	const struct fbgc_object_property_holder * p = NULL;
	for(uint8_t i = 0; i<CMODULE_PROPERTY_HOLDER_ARRAY_LEN; ++i){
		p = cmodule_property_holder_array[i];
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
			add_variable_in_field(current_field,cc->name,new_fbgc_cfun_object(cc->function));
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
					add_variable_in_field(current_field,str,new_fbgc_cfun_object(methods->method[len].function));
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
					add_variable_in_field(current_field,str,members->member[len].function(NULL,NULL));
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
