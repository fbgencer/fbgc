#include "include.h"
#include "new_lexer.h"

void lexer_output_printer(){
	printf("\n===========[TOKEN]==========\n");
	int lexword_ctr = 0;
	int lexint_ctr = 0;
	int lexstr_ctr = 0;
	int lexdouble_ctr = 0;
	
	for(int i = 0; i<lextok.size(); i++) {
		cout<<TYPES_ARRAY[lextok[i]];
		if(lextok[i] == WORD) cout<<"{"<<lexword[lexword_ctr++]<<"}\n";
		else if(lextok[i] == INT) cout<<"{"<<lexint[lexint_ctr++]<<"}\n";
		else if(lextok[i] == DOUBLE) cout<<"{"<<lexdouble[lexdouble_ctr++]<<"}\n";
		else if(lextok[i] == STRING) cout<<"{"<<lexstr[lexstr_ctr++]<<"}\n";

		else cout<<'\n';
	}	
}

typedef struct{
	string str;
	ufbg_token type;
} lexer_trainer;
#define NUM 199
#define OPERATOR 200

const static lexer_trainer lexer_examples[]={
{"_x12_", WORD},
{"a1b2c3", WORD},
{"aAbBcCdDeEfF_gGhHiI", WORD},
{"___fbg___", WORD},
{"_______", WORD},
{"f_b_g_", WORD},
{"1234567", NUM},
{"123.23", NUM},
{"1E16", NUM},
{"1E-16", NUM},
{"0x42" ,NUM},
{"0X123", NUM},
{"0b0010101", NUM},
{"0B10101010", NUM},
{"1+2j", NUM},
{"1+2i", NUM},
{"5+5.123i", NUM},
{"'123ABCDEF'", STRING},
{"\"FBGENCER 8 88 888\"", STRING},
{"=", OPERATOR},
{"==", OPERATOR},
{"^=", OPERATOR},
{"++", OPERATOR},
{"...", OPERATOR},
{".", OPERATOR},
{"=>", OPERATOR}
};
int main(){

	string lexerinput =  "x == --y++";

  /*string line;
  ifstream myfile ("lexer_checker.txt");

  if (myfile.is_open()){
    while ( getline (myfile,line)){
      cout << line << '\n';
    }
    myfile.close();
  }*/
	cout<<"stoi : "<<atof("2E+3")<<endl;
	printf("cevap : %ld",strtol("5e+3",NULL,10));

	if(new_lexer(lexerinput)){
		lexer_output_printer();	
	}
	else error("In main, Lexer did not work");

	return 0;
}
