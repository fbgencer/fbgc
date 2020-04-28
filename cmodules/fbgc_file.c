#include "../src/fbgc.h"
#include "fbgc_file.h"

static
struct fbgc_object * fbgc_file_fread(struct fbgc_object ** arg,int argc){

	struct fbgc_cstruct_object * so; 

	if(!parse_tuple_content(arg,argc,".s",&so))
		return NULL;

	//= cast_fbgc_object_as_cstruct(arg[0]);

	struct file_struct * fs = (struct file_struct *) so->cstruct; 

	//long old_fp_loc = ftell(fs->fp);

	fseek (fs->fp , 0 , SEEK_END);
		long file_text_len = ftell(fs->fp);
		rewind(fs->fp);

	struct fbgc_object * str = new_fbgc_str_object_empty(file_text_len);
	size_t res = fread(content_fbgc_str_object(str),1,file_text_len,fs->fp);
	assert(res == file_text_len);

	//fseek(fs->fp,0,old_fp_loc); 

	return str;
}


static
struct fbgc_object * fbgc_file_fprint(struct fbgc_object ** arg,int argc){
	if(argc == 2){
		struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
		
			struct file_struct * fs = (struct file_struct *) so->cstruct; 
			fprintf(fs->fp,"%s", content_fbgc_str_object(arg[1]));
		
		return __fbgc_nil;
	}
	cprintf(100,"fprint takes exactly two arguments!\n");
	return NULL;	
}

static
struct fbgc_object * fbgc_file_freadline(struct fbgc_object ** arg,int argc){
	if(argc == 2){
		struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
		if(arg[1]->type == INT){
			struct file_struct * fs = (struct file_struct *) so->cstruct; 

			int which_line = cast_fbgc_object_as_int(arg[1])->content;
			if(which_line < 0) {
				cprintf(100,"Line no cannot be negative\n");
				return NULL;
			}

			long old_fp_loc = ftell(fs->fp);

			rewind(fs->fp); //set the file pointer to start
			
			int c = 0;
			int count_char = 0;
			int count_line = 0;

			do{
				c = fgetc(fs->fp);
				if(c == '\n'){
					if(count_line == which_line) break;
					count_line++;
					count_char = -1; //eat newline
				}
				count_char++;
			}while(c != EOF);

			if(count_line != which_line){
				cprintf(100,"File contains %d line(s), cannot find %d. line\n",count_line,which_line);
				return NULL;
			}
			
			fseek(fs->fp,-count_char-1,SEEK_CUR);
			struct fbgc_object * str = new_fbgc_str_object_empty(count_char);
			
			size_t res = fread(content_fbgc_str_object(str),1,count_char,fs->fp);

			fseek(fs->fp,old_fp_loc,SEEK_SET); //set the file pointer to its old location
			if(res == count_char)
				return str;
			else 
				return NULL;
		}		
	}
	
	cprintf(100,"freadline takes exactly two arguments!\n");
	return NULL;
}

static
struct fbgc_object * fbgc_file_fclose(struct fbgc_object ** arg,int argc){
	
	struct fbgc_cstruct_object * so; 

	if(!parse_tuple_content(arg,argc,".",&so))
		return NULL;
	
	struct file_struct * fs = (struct file_struct *) so->cstruct; 
	fclose(fs->fp);
		
	return __fbgc_nil;
}

static struct fbgc_object_method _fbgc_file_cstruct_methods = {
	.len = 4,
	.method = {
		{.name = "fread", .function = &fbgc_file_fread},
		{.name = "fprint", .function = &fbgc_file_fprint},
		{.name = "freadline", .function = &fbgc_file_freadline},
		{.name = "fclose", .function = &fbgc_file_fclose},
	}
};


const struct fbgc_object_property_holder fbgc_file_cstruct_property_holder = {
	.bits = 
	_BIT_METHODS
	,
	.properties ={
		{.methods = &_fbgc_file_cstruct_methods},		
	}
};


static struct fbgc_object * fbgc_file_fopen(struct fbgc_object ** arg,int argc){
	
	if(argc == 2 && arg[0]->type == STRING && arg[1]->type == STRING){
		struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *) new_fbgc_cstruct_object(sizeof(struct file_struct), &fbgc_file_cstruct_property_holder);
		struct file_struct * fs = (struct file_struct *) so->cstruct; 		
	
		fs->fp = fopen(content_fbgc_str_object(arg[0]),content_fbgc_str_object(arg[1]));
		assert(fs->fp != NULL);

		return (struct fbgc_object *)so;
	}
	
	cprintf(100,"fopen takes exactly two arguments!\n");
	return NULL;	
};





const struct fbgc_cmodule fbgc_file_module = 
{
	.initializer = {.name = "file", .function = NULL},
	.functions = {
		{.name = "fopen", .function = &fbgc_file_fopen},
		{NULL,NULL}
	}
};