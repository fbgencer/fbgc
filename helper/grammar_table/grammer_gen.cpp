#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


typedef struct {
  string return_str;
  string left_condition;
  string input;
} gm_struct;

/*
  ATOM: 
    (START | &BINARY_OPERATOR | UNBALANCED_LIST) ('INT' | 'STRING' | 'DOUBLE' | 'ID')
*/

void print_gm_struct(gm_struct gm){
  cout<<gm.return_str<<":"<<gm.left_condition<<" "<<gm.input<<endl;
}

int main(){

  vector<gm_struct> gm;

  gm_struct p;
  p.return_str = "ATOM";
  p.left_condition = "START";
  p.input = "INT";

  gm.push_back(p);

  print_gm_struct(gm[0]);


 
 /* FILE * pFile;
   char buffer [300];

   pFile = fopen ("fbgc_grammar.txt" , "r");
   int i = 0;
   if (pFile != NULL){
     while ( ! feof (pFile) )
     {

       if ( fgets (buffer , 100 , pFile) == NULL ) break;
       printf("[%d]:%s",i++,buffer);
     }
     fclose (pFile);
   }
   else	perror ("Error opening file");*/

   return 0;

}