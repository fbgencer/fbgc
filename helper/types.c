#include <stdio.h>


void print_types_as_define(){
  FILE * pFile;
  FILE * out_file;
  int c;
  int n = 0;
  pFile=fopen ("types.txt","r");
  out_file = fopen("types_out.txt","w+");
  if (pFile==NULL) perror ("Error opening file");
  else
  {
    int i = 0; fprintf(out_file,"#define ");
    while((c = getc(pFile)) != EOF){

      if (c != ',' && c!= '\n' && c!= '\t') fprintf(out_file,"%c",c);
      else if(c == ',')fprintf(out_file," %d\n#define ",i++);
    } 
    fprintf(out_file,"TOKEN_LIST_AS_STRINGS()\\\n");
    fclose(pFile);
    fclose(out_file);
  }  
}

void print_types_as_strings(){
  FILE * pFile;
  FILE * out_file;
  int c;
  int n = 0;
  pFile=fopen ("types.txt","r");
  out_file = fopen("types_out.txt","a+");
  if (pFile==NULL) perror ("Error opening file");
  else
  {
    fprintf(out_file,"\"");
    while((c = getc(pFile)) != EOF){

      if (c != ',' && c!= '\n' && c!= '\t') fprintf(out_file,"%c",c);
      else if(c == ',')fprintf(out_file,"\",\\\n\"");
    }
    fprintf(out_file,"\b  "); 
    fclose(pFile);
    fclose(out_file);
  }    
}


int main()
{ 

  print_types_as_define(); 
  print_types_as_strings();
  return 0;
}