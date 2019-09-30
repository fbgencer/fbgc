#include "../src/fbgc.h"
#include "fbgc_file.h"

/*
  FILE * pFile;
   char buffer [100];

   pFile = fopen ("myfile.txt" , "r");
   if (pFile == NULL) perror ("Error opening file");
   else
   {
     while ( ! feof (pFile) )
     {
       if ( fgets (buffer , 100 , pFile) == NULL ) break;
       fputs (buffer , stdout);
     }
     fclose (pFile);
   }
*/

new_fbgc_cfunction(fbgc_fopen,"fopen")
{
	
	if(argc == 2 && arg[0]->type == STRING && arg[1]->type == STRING){
		struct fbgc_cstruct_object * so = new_fbgc_cstruct_object(sizeof(struct file_struct), &fbgc_file_module);
		struct file_struct * fs = (struct file_struct *) so->cstruct; 		
	
		fs->fp = fopen(content_fbgc_str_object(arg[0]),content_fbgc_str_object(arg[1]));
		assert(fs->fp != NULL);

		return (struct fbgc_object *)so;
	}
	else {
		cprintf(100,"fopen takes exactly two arguments!\n");
	}

	return NULL;	
}
new_fbgc_cfunction(fbgc_fprint,"fprint")
{

	if(argc == 2){
		struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
		if(so->parent == &fbgc_file_module){
			struct file_struct * fs = (struct file_struct *) so->cstruct; 
			fprintf(fs->fp,"%s", content_fbgc_str_object(arg[1]));
		}		
	}

	return NULL;
}

new_fbgc_cfunction(fbgc_fread,"fread")
{

	if(argc == 1){
		struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
		if(so->parent == &fbgc_file_module){
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
	}
	else {
		cprintf(100,"fread takes exactly one arguments!\n");
	}

	return NULL;
}

new_fbgc_cfunction(fbgc_freadline,"freadline")
{

	if(argc == 2){
		struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
		if(so->parent == &fbgc_file_module && arg[1]->type == INT){
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
			return str;
		}		
	}
	else {
		cprintf(100,"fread takes exactly two arguments!\n");
	}

	return NULL;
}


new_fbgc_cfunction(fbgc_fclose,"fclose")
{
	struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
	if(so->parent == &fbgc_file_module){
		struct file_struct * fs = (struct file_struct *) so->cstruct; 
		fclose(fs->fp);
	}

	return NULL;
}


const struct fbgc_cfunction fbgc_file_initializer_struct = {"file",fbgc_file_initializer};
extern struct fbgc_object * fbgc_file_initializer (struct fbgc_object ** arg,int argc){
	return arg;
}



//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_file_module = 
{
	.initializer = &fbgc_file_initializer_struct,
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_fopen_struct,
		&fbgc_fprint_struct,
		&fbgc_fread_struct,
		&fbgc_freadline_struct,
		&fbgc_fclose_struct,
		NULL
	}
};
