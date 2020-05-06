#include "../src/fbgc.h"
#include "fbgc_file.h"

static
struct fbgc_object * fbgc_file_read(struct fbgc_object ** arg,int argc){

	struct fbgc_cstruct_object * so; 

	if(parse_tuple_content(arg,argc,".",&so) == -1)
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
struct fbgc_object * fbgc_file_print(struct fbgc_object ** arg,int argc){
	
	struct fbgc_cstruct_object * so; 
	struct fbgc_str_object * payload;

	if(parse_tuple_content(arg,argc,".s",&so,&payload) == -1)
		return NULL;
	
	struct file_struct * fs = (struct file_struct *) so->cstruct; 
	fprintf(fs->fp,"%s", payload->content);
		
	return fbgc_nil_object;
}


static
struct fbgc_object * fbgc_file_printline(struct fbgc_object ** arg,int argc){
	
	//Not tested yet..

	struct fbgc_cstruct_object * so; 
	struct fbgc_int_object * line_no;
	struct fbgc_str_object * payload;

	if(parse_tuple_content(arg,argc,".is",&so,&line_no,&payload) == -1)
		return NULL;
	
	struct file_struct * fs = (struct file_struct *) so->cstruct; 
	
	long old_fp_loc = ftell(fs->fp); //Where are we in the file, hold it in order not to lose it

	rewind(fs->fp); //set the file pointer to start
	
	int c = 0;
	int count_char = 0;
	int count_line = 1;

	int to_line = line_no->content;

	do{
		c = fgetc(fs->fp);
		if(c == '\n'){
			if(count_line == to_line) break;
			count_line++;
			count_char = -1; //eat newline
		}
		count_char++;
	}while(c != EOF);

	if(count_line != to_line){
		FBGC_LOGE("File contains %d line(s), cannot find %d.line\n",count_line,to_line);
		return NULL;
	}
	fseek(fs->fp,-count_char-1,SEEK_CUR); //Now set the pointer beginning of this line
	count_char -= length_fbgc_str_object(payload);
	fprintf(fs->fp,"%s%*s", payload->content,count_char,"");
	
	fseek(fs->fp,old_fp_loc,SEEK_SET); //set the file pointer to its old location
		
	return fbgc_nil_object;
}

static
struct fbgc_object * fbgc_file_readline(struct fbgc_object ** arg,int argc){
	
	struct fbgc_cstruct_object * so; 
	struct fbgc_int_object * line_no;

	if(parse_tuple_content(arg,argc,".i",&so,&line_no) == -1)
		return NULL;
		
	struct file_struct * fs = (struct file_struct *) so->cstruct; 

	if(line_no->content < 0) {
		FBGC_LOGE("Line no cannot be negative\n");
		return NULL;
	}

	long old_fp_loc = ftell(fs->fp); //Where are we in the file, hold it in order not to lose it

	rewind(fs->fp); //set the file pointer to start
	
	int c = 0;
	int count_char = 0;
	int count_line = 1;

	do{
		c = fgetc(fs->fp);
		if(c == '\n'){
			if(count_line == line_no->content) break;
			count_line++;
			count_char = -1; //eat newline
		}
		count_char++;
	}while(c != EOF);

	if(count_line != line_no->content){
		FBGC_LOGE("File contains %d line(s), cannot find %d.line\n",count_line,line_no->content);
		return NULL;
	}
	
	fseek(fs->fp,-count_char-1,SEEK_CUR); //Now set the pointer beginning of this line
	struct fbgc_object * str = new_fbgc_str_object_empty(count_char);
	
	size_t res = fread(content_fbgc_str_object(str),1,count_char,fs->fp); //read the whole line

	fseek(fs->fp,old_fp_loc,SEEK_SET); //set the file pointer to its old location
	if(res == count_char)
		return str;
	
	return NULL;	
}

static
struct fbgc_object * fbgc_file_close(struct fbgc_object ** arg,int argc){
	
	struct fbgc_cstruct_object * so; 

	if(parse_tuple_content(arg,argc,".",&so) == -1)
		return NULL;
	
	struct file_struct * fs = (struct file_struct *) so->cstruct; 
	fclose(fs->fp);
		
	return fbgc_nil_object;
}



static struct fbgc_object * subscript_operator_fbgc_file(struct fbgc_object * iterable,struct fbgc_object * index_obj){
	struct fbgc_object * xx[2] = {iterable,index_obj};
	return fbgc_file_readline(xx,2);
}

static struct fbgc_object_method _fbgc_file_cstruct_methods = {
	.len = 5,
	.method = {
		{.name = "read", .function = &fbgc_file_read},
		{.name = "print", .function = &fbgc_file_print},
		{.name = "readline", .function = &fbgc_file_readline},
		{.name = "printline", .function = &fbgc_file_printline},
		{.name = "close", .function = &fbgc_file_close},
	}
};


const struct fbgc_object_property_holder fbgc_file_cstruct_property_holder = {
	.bits = 
	_BIT_METHODS | 
	_BIT_SUBSCRIPT_OPERATOR
	,
	.properties ={
		{.methods = &_fbgc_file_cstruct_methods},
		{.subscript_operator = &subscript_operator_fbgc_file},
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