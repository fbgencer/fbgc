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
#include "../src/fbgc.h"
#include "fbgc_file.h"



static
struct fbgc_object * fbgc_file_read(struct fbgc_cfun_arg * arg){

	struct fbgc_cstruct_object * so; 

	if(parse_tuple_content(arg,".",&so) == -1)
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
struct fbgc_object * fbgc_file_print(struct fbgc_cfun_arg * arg){
	
	struct fbgc_cstruct_object * so; 
	struct fbgc_str_object * payload;

	if(parse_tuple_content(arg,".s",&so,&payload) == -1)
		return NULL;
	
	struct file_struct * fs = (struct file_struct *) so->cstruct; 
	fprintf(fs->fp,"%s", payload->content);
		
	return fbgc_nil_object;
}


static
struct fbgc_object * fbgc_file_printline(struct fbgc_cfun_arg * arg){
	
	//Not tested yet..

	struct fbgc_cstruct_object * so; 
	struct fbgc_int_object * line_no;
	struct fbgc_str_object * payload;

	if(parse_tuple_content(arg,".is",&so,&line_no,&payload) == -1)
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
struct fbgc_object * fbgc_file_readline(struct fbgc_cfun_arg * arg){
	
	struct fbgc_cstruct_object * so; 
	struct fbgc_int_object * line_no;

	if(parse_tuple_content(arg,".i",&so,&line_no) == -1)
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
struct fbgc_object * fbgc_file_close(struct fbgc_cfun_arg * arg){
	
	struct fbgc_cstruct_object * so; 

	if(parse_tuple_content(arg,".",&so) == -1)
		return NULL;
	
	struct file_struct * fs = (struct file_struct *) so->cstruct; 
	fclose(fs->fp);
		
	return fbgc_nil_object;
}



static struct fbgc_object * subscript_operator_fbgc_file(struct fbgc_object * iterable,struct fbgc_object * index_obj){
	struct fbgc_object * temp[2] = {iterable,index_obj};
	struct fbgc_cfun_arg arg = {.arg = temp, .argc = 2, .kwargs_flag = 0};
	return fbgc_file_readline(&arg);
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


static struct fbgc_object * fbgc_file_fopen(struct fbgc_cfun_arg * arg){
	
	struct fbgc_object * fname;
	struct fbgc_object * open_type;
	if(parse_tuple_content(arg,"ss",&fname,&open_type) == -1)
		return NULL;

	struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *) new_fbgc_cstruct_object(sizeof(struct file_struct), &_fbgc_file_cstruct_property_holder);
	struct file_struct * fs = (struct file_struct *) so->cstruct; 		
	
	fs->fp = fopen(content_fbgc_str_object(fname),content_fbgc_str_object(open_type));
	assert(fs->fp != NULL);

	return (struct fbgc_object *)so;
};

static struct fbgc_cfunction _fbgc_file_fopen_struct =  {.name = "fopen", .function = &fbgc_file_fopen};

const struct fbgc_object_property_holder _fbgc_file_cstruct_property_holder = {
	.bits = 
	_BIT_METHODS | 
	_BIT_SUBSCRIPT_OPERATOR |
	_BIT_NAME |
	_BIT_CONSTRUCTOR
	,
	.properties ={
		{.methods = &_fbgc_file_cstruct_methods},
		{.subscript_operator = &subscript_operator_fbgc_file},
		{.name = "file"},
		{.constructor = &_fbgc_file_fopen_struct}
	}
};
