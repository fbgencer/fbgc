#include "include.h"
#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <cstdio>
#include <cstdlib>

	//string lexerinput;
	//lexerinput = "x = 1000; if(1<5) if(2>6) x = 1; y= x; end; x = 5; end";
	//lexerinput += ";";
int main(){

	string line,lexerinput;
  	ifstream myfile ("draft.ufbg");
  	if (myfile.is_open()){
    while ( getline (myfile,line)){
     	//cout << line << '\n';
      	if(line[0] != '%')
      		{
      			lexerinput += line;
      			lexerinput += " "; 
      		}
      	//lexerinput+="\n";

    }
   		 myfile.close();
  	}

	if(!lexer(lexerinput)) {error("In parser_control, Lexer did not work"); return 0;}

  	printf("======================\n");
	for (int i = 0; i < lextok.size(); ++i){cout<<TYPES_ARRAY[lextok[i]]<<endl;}
	printf("======================\n");
	//for (int i = 0; i < lexint.size(); ++i){cout<<lexint[i]<<endl;}
	//printf("======================\n");


	//parser_module mainparser;
	//mainparser.module_name = "main";

	if(!parser() ) {error("In parser_control, Parser did not work"); return 0;}
	printf("\n=========PARINT CONTROL==========\n");
	
	int dummyiter = 0;
	for (int i = 0; i < parser_stack.size(); ++i){
		if(parser_stack[i] == INDEX_S || parser_stack[i] == INDEX_L || parser_stack[i] == INT ||
			parser_stack[i] == WHILE || parser_stack[i] == END_LOOP || parser_stack[i] == IF ||
			parser_stack[i] == ELSIF || parser_stack[i] == ELSE || parser_stack[i] == CFUN || 
			parser_stack[i] == ARG){
			printf("[%d]{%s : %d}\n",dummyiter-1,TYPES_ARRAY[parser_stack[i]].c_str(),parint[dummyiter++]);
		}

	}

	printf("\n=========PARINT==========\n");
	for (int i = 0; i < parint.size(); ++i){cout<<"{"<<parint[i]<<"}";}
	printf("\n=========PARDOUBLE=======\n");
	for (int i = 0; i < pardouble.size(); ++i){cout<<"{"<<pardouble[i]<<"}";}
	printf("\n=========PARVAR=======\n");
	for (int i = 0; i < parvar.size(); ++i){cout<<"{"<<parvar[i]<<"}";}
	printf("\n=========LEXSTR=======\n");
	for (int i = 0; i < lexstr.size(); ++i){cout<<"{"<<lexstr[i]<<"}";}
	printf("\n\n");

   /*for(int i = 0; i<sizeof(math_function_table)/sizeof(string); i++){
   	printf(">> %s\n",math_function_table[i].c_str());
  	}*/
	//printf("NUM:%d,%d a:%02x,b:%02x\n",641>>6,641&0x3f,65>>6,65&0x3f);
  	/*int nunu = 12; int a = nunu>>8; int b = nunu&0xff; 
  	printf("NUM:%d a:%02x,b:%02x\n",nunu,a,b);
  	int a2 = 0x01; int b2 = 0x00;
  	for (int i = 0; i < 8; ++i){
  		for (int j = 0; j < 64; ++j){
  			printf("{%d,%d}  NUM:%d \n",i,j,(i<<6)|(j) );
  		}
  	}*/
  	


  	//sayıyı iki parcaya ayır 
	//a = sayı>>8, b = sayı&0xff, geri toplamak icin sayı = (a<<8)|b

	return 0;
}

