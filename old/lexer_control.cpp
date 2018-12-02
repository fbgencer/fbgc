#include "include.h"
#include "lexer.h"
#include <fstream>
#include <cstdio>
#include <cstdlib>



int main(){

	string lexerinput;
	lexerinput = "x = 123";

	lexer(lexerinput);// ?  printlexer() : error("In main, Lexer did not work");
	printf("======================\n");
	for (int i = 0; i < lexint.size(); ++i){
		cout<<lexint[i]<<endl;
	}
		
	return 0;
}
