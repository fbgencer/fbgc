#include <stdio.h>


void print_types_as_define(){
  FILE * pFile;
  int c;
  int n = 0;
  pFile=fopen ("types.txt","r");
  if (pFile==NULL) perror ("Error opening file");
  else
  {
    int i = 0; printf("#define ");
    while((c = getc(pFile)) != EOF){

      if (c != ',' && c!= '\n' && c!= '\t') printf("%c",c);
      else if(c == ',')printf(" %d\n#define ",i++);
    } 
    fclose (pFile);
  }  
}

void print_types_as_strings(){
  FILE * pFile;
  int c;
  int n = 0;
  pFile=fopen ("types.txt","r");
  if (pFile==NULL) perror ("Error opening file");
  else
  {
    printf("\t\"");
    while((c = getc(pFile)) != EOF){

      if (c != ',' && c!= '\n' && c!= '\t') printf("%c",c);
      else if(c == ',')printf("\",\n\t\"");
    } 
    fclose (pFile);
  }    
}


int main()
{ 
  print_types_as_strings();
  printf("\n\n");
  print_types_as_define();
  printf("\n\n");
  return 0;
}