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
		&fbgc_fclose_struct,
		NULL
	}
};
