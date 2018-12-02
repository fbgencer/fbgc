#ifndef PARSER_H
#define PARSER_H
#include <iomanip>
#include "lexer.h"


std::vector<fbgtoken> parser_stack;
std::vector<fbgtoken> parser_stack2; // only tokens
std::vector<string> parstr;
std::vector<int> parint;
std::vector<double> pardouble;
std::vector<string> parvar;


std::vector<int> end_index_hold; // works as stack, last end is need for first end_need word
std::vector<int> loop_index_hold;
std::vector<int> cfun_code_hold;


std::vector<string> function_name;
std::vector<int> function_order;

//iterators
int it_int = 0,it_double = 0,it_str = 0,it_word = 0;


/*OPERATOR PRECEDENCE for parser alg.*/
#define get_precedence(T)({\
	(T == INC || T == DECR || T == UMINUS || T == UPLUS) ? 10:\
	(T == DIV  || T == MULT || T == MOD || T==POWER1 || T == POWER2 || T==DIVDIV) ? 9:\
	(T == PLUS || T == MINUS || T== NOT_OP ) ? 8:\
	(T == LSHIFT || T == RSHIFT ) ? 7:\
	(T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ) ? 6:\
	(T == EQ_EQ || T == NOT_EQ   || T == IS_EQ) ? 5:\
	(T == AND_OP ) ? 4:\
	(T == OR_OP) ? 3:\
	(T == ASSIGN ||T == PLUS_ASSIGN || T == MIN_ASSIGN || T == MULT_ASSIGN || T == DIV_ASSIGN) ? 2:\
	(T == COMMA || T == LPARA || T == RPARA) ? 1:0;})
#define opcompare(T1,T2)({\
	(get_precedence(T1) > get_precedence(T2)) ? 2:\
	(get_precedence(T1) == get_precedence(T2)) ? 1:0; })

#define basic_calculation(opT,x,y)({\
	opT == PLUS ?  x+y:\
	opT == MINUS ?  x-y:\
	opT == MULT ? x*y:\
	opT == DIV ? (x*1.0)/y:\
	opT == LOWER ?  x<y: \
	opT == GREATER ?  x>y:\
	opT == LO_EQ ? x<=y:\
	opT == GR_EQ? x>=y:\
	(opT == EQ_EQ || opT == IS_EQ)?  x==y: \
	opT == NOT_EQ ? x!=y:\
	opT == AND_OP? x&&y:\
	opT == OR_OP ?  x||y:\
	opT == DIVDIV ? (1.0/x)+(1.0/y) : 0;})

void basic_operator_handler(fbgtoken opToken){
	//printf("\n--------------------------------\nBasic operator handler begin !");
	//printf(": {%s}\n",TYPES_ARRAY[opToken].c_str());

	fbgtoken stklast = parser_stack.back();
	fbgtoken stklast2 = parser_stack[parser_stack.size()-2];

	//printf("TYPE1: {%s} TYPE2:{%s}\n",TYPES_ARRAY[stklast].c_str(),TYPES_ARRAY[stklast2].c_str());

	if(opToken == UMINUS){
		if(stklast == INT) {parint.back() = -parint.back(); return;}	
		else if(stklast == DOUBLE){pardouble.back() = -pardouble.back(); return;}
	}
	else if(opToken == NOT_OP) {
		if(stklast == INT) {parint.back() = !parint.back(); return;}	
		else if(stklast == DOUBLE){pardouble.back() = !pardouble.back(); return;}
	} 
	else if(opToken == UPLUS) return;


	if(stklast == INT && stklast2 == INT){
		int numlast = parint.back(); parint.pop_back();
		int numlast1 = parint.back(); parint.pop_back();
		parint.push_back( basic_calculation(opToken,numlast1,numlast) );
		
		//just pop one num, other token already num
		parser_stack.pop_back();

		//cout<<"Result:"<<parint.back()<<endl;
		//printf("LAST NUM LAST1 NUM\n");

	}
	else if(stklast == INT && stklast2 == DOUBLE){
		int numlast = parint.back(); parint.pop_back();
		double doublelast1 = pardouble.back(); pardouble.pop_back();
		pardouble.push_back( basic_calculation(opToken,doublelast1,numlast) );
		
		//it looks like 3.0 | 2, just pop last token, other one already double
		parser_stack.pop_back();

		//cout<<"Result:"<<pardouble.back()<<endl;
		//printf("LAST NUM LAST1 DOUBLE\n");
	}
	else if(stklast == DOUBLE && stklast2 == INT){
		double doublelast = pardouble.back(); pardouble.pop_back();
		int numlast1 = parint.back(); parint.pop_back();

		pardouble.push_back( basic_calculation(opToken,numlast1,doublelast) );
		
		//it looks like 2 | 3.0, pop both and push double
		parser_stack.pop_back();
		parser_stack.pop_back();
		parser_stack.push_back(DOUBLE);

		//cout<<"Result:"<<pardouble.back()<<endl;
		//printf("LAST DOUBLE LAST1 NUM\n");
	}
	else if(stklast == DOUBLE && stklast2 == DOUBLE){
		double doublelast = pardouble.back(); pardouble.pop_back();
		double doublelast1 = pardouble.back(); pardouble.pop_back();
		pardouble.push_back( basic_calculation(opToken,doublelast1,doublelast) );
		
		//both double, push one of them
		parser_stack.pop_back();

		//cout<<"Result:"<<pardouble.back()<<endl;
		//printf("LAST DOUBLE LAST1 DOUBLE\n");
	}
	else {
		parser_stack.push_back(opToken);
	}
	//printf("END OF THE basic_calculation\n------------------------------\n");
}



unsigned char parser(){
	int comma_ctr = 0;
	printf("Parser Begin.\n");
	//Parser begin taking output of lexer.
	//lextok,lexstr,lexdouble,lexint output of lexer

	for (int i = 0; i < lextok.size(); ++i){


		if(isINT(lextok[i])) {
			//printf("INT : {%s}\n",TYPES_ARRAY[lextok[i]]);
			parint.push_back(lexint[it_int++]);
			parser_stack.push_back(INT);
		}
		else if(isDOUBLE(lextok[i])){
			//printf("DOUBLE:{%s}\n",TYPES_ARRAY[lextok[i]].c_str());
			pardouble.push_back(lexdouble[it_double++]);
			parser_stack.push_back(DOUBLE);
		}
		else if(isSTR(lextok[i])){
			//printf("STR:{%s}\n",TYPES_ARRAY[lextok[i]].c_str());
			parstr.push_back(lexstr[it_str++]);
			parser_stack.push_back(STRING);
		}
		else if(isWORD(lextok[i])){
			//printf("WORD {%s}\n",TYPES_ARRAY[lextok[i]].c_str());
			//parser_stack.push_back(lextok[i]);
			cout<<"Word name : "<<lexword[it_word]<<endl;

			//word | reset :> new variable allocation
			// word | assign :> maybe new or changing value of var
			if(lextok[i+1] == ASSIGN){
				//User think variable already allocated. Search if there is var name inside variables.
				//search variable is there or not
				if(parvar.size() == 0){
					parvar.push_back(lexword[it_word]);
					parint.push_back(parvar.size()-1);
				}
				else {
					int v = 0; 
					while(parvar[v] != lexword[it_word]){
						v++; if(v==parvar.size()) break;
					}
					//Variable is defined before.
					if( v != parvar.size()){
						parint.push_back(v);
					}
					else { //new var
						parvar.push_back(lexword[it_word]);
						parint.push_back(parvar.size()-1);
					}
				}
				//parser_stack.push_back(VAR); // push var token into main stack
				parser_stack.push_back(INDEX_S); // push variable order into main stack
				it_word++;

			}
			else { //variable is called. Search its existence.
				if(parvar.size() != 0){
					int v = 0; 
					while(parvar[v] != lexword[it_word]){
						v++; if(v==parvar.size()) break;
					}
					//Variable is defined before.
					if( v != parvar.size()){
						parint.push_back(v);
						//parser_stack.push_back(VAR); // push var token into main stack
						parser_stack.push_back(INDEX_L); // push variable order into main stack
						it_word++;
					}
					else { //new var
						error2("%s%s%s","Variable >>' ",lexword[it_word].c_str(),
						" '<< is not created before!\n");
						return 0;
					}
				}
				else {
					error2("%s%s%s","Variable >>' ",lexword[it_word].c_str(),
					" '<< is not created before!\n");
					return 0;
				}
			}
		}
		
		else if(isOP(lextok[i])){
			printf("OPERATOR : {%s}\n",TYPES_ARRAY[lextok[i]].c_str());

			/*UNARYLERI DUZELT!!!!!!!!!!!!!!!!!*/
			//unary durumları
				//x=-5+3, x=-(-(-2)) , 
				// arkası operator|para|iter=sıfır önu kesinlikle sayı,var olmalı
				//unary -> (num|var|word|RPARA) unary (num|var|word|LPARA)
			if(lextok[i] == MINUS || lextok[i] == PLUS){
				if( (i == 0||isOP(lextok[i-1])||lextok[i-1] == LPARA) && 
					(isNUM(lextok[i+1])|| isWORD(lextok[i+1])|| isVAR(lextok[i+1])||
					lextok[i+1] == RPARA || lextok[i+1] == LPARA)
					
				  ){ 
				  		printf("UNARY SITUATION OCCURED\n");
						lextok[i] += 1;
						printf("OPERATOR : {%s}\n",TYPES_ARRAY[lextok[i]].c_str());  
						//if current token minus, minus+1 = unary minus is arranged.
						//Same as for plus. No need to know plus or minus.
					} 
			}

			//If operator is not unary type operator
			//if operator at the end of the stack2 is higher precedence or equal when compare
			//to current one then it must push onto the main stack. 
			if(parser_stack2.size() != 0 && opcompare(parser_stack2.back(),lextok[i])>=1){
				do{
				
					if(isBASIC_OPERATOR(parser_stack2.back())) {
						basic_operator_handler(parser_stack2.back());
						parser_stack2.pop_back();	
					}
					else {
						parser_stack.push_back(parser_stack2.back()); 
						parser_stack2.pop_back();
					}

				}while(opcompare(parser_stack2.back(),lextok[i])>=1);
			}

			parser_stack2.push_back(lextok[i]);

		}
		else if(lextok[i] == LPARA){
			printf("LPARA\n");
			parser_stack2.push_back(LPARA);
		}
		else if(lextok[i] == RPARA){
			printf("RPARA\n");
			while(parser_stack2.back() != LPARA){
					if(isBASIC_OPERATOR(parser_stack2.back())) {
						basic_operator_handler(parser_stack2.back());
						parser_stack2.pop_back();	
					}
					else {
						parser_stack.push_back(parser_stack2.back()); 
						parser_stack2.pop_back();
					}

				//printf("fuck\n");
				/*if(isBASIC_OPERATOR(parser_stack2.back())) 
					basic_operator_handler(parser_stack2.back());
				else parser_stack.push_back(parser_stack2.back());*/
			}
			parser_stack2.pop_back(); // now lpara is popped
			//try is this a function paranthesis
			if(isFUNCTIONABLE(parser_stack2.back())){
				printf("yes we catched a FUNCTIONABLE\n");
				//now push into main stack and pop from stack2
				parser_stack.push_back(parser_stack2.back());
				printf("{: %s}",TYPES_ARRAY[parser_stack.back()].c_str());
				if(parser_stack2.back() == ELSIF){
					parint[end_index_hold.back()] = parser_stack.size()-1;
					end_index_hold.pop_back();
				}
				if(parser_stack2.back() == WHILE) {
					loop_index_hold.push_back(parser_stack.size()-1);
				}
				else loop_index_hold.push_back(-1);
				
				parser_stack2.pop_back();

				//to locate end for this functionable add dummy var to int
				parint.push_back(-1);
				//hold its location. When end comes go to this location and change it for end.
				end_index_hold.push_back(parint.size()-1);
				printf("parint[%d] holds this index\n",end_index_hold.back() );

			}
			else if(parser_stack2.back() == CFUN){
				printf("last stack2 is cfun code:%d\n",cfun_code_hold.back());
				printf("CFUN code>> library:%d , func :%d\n",cfun_code_hold.back()>>6,
					cfun_code_hold.back()&0x3f );
				parser_stack.push_back(ARG);
				parint.push_back(comma_ctr+1); comma_ctr = 0;
				parser_stack.push_back(parser_stack2.back());
				parint.push_back(cfun_code_hold.back());	
				cfun_code_hold.pop_back();
				parser_stack2.pop_back();
			}

		}
		else if(lextok[i] == COMMA){
			printf("Comma :)))\n");
			comma_ctr++;
			while(parser_stack2.back() != LPARA){
					if(isBASIC_OPERATOR(parser_stack2.back())) {
						basic_operator_handler(parser_stack2.back());
						parser_stack2.pop_back();	
					}
					else {
						parser_stack.push_back(parser_stack2.back()); 
						parser_stack2.pop_back();
					}

				//printf("fuck\n");
				/*if(isBASIC_OPERATOR(parser_stack2.back())) 
					basic_operator_handler(parser_stack2.back());
				else parser_stack.push_back(parser_stack2.back());*/
			}			
		}

		// IF is basically a function, act it like this. Push stack2 then add a condition for rpara 
		else if(lextok[i] == IF){
			printf("if geldi\n");
			parser_stack2.push_back(IF);
		}
		else if(lextok[i] == ELSIF){
			printf("elsif geldi\n");
			parser_stack2.push_back(ELSIF);
			//parint[end_index_hold.back()] = parser_stack.size()-1;
			//end_index_hold.pop_back();	
		}
		else if(lextok[i] == ELSE){
			printf("else geldi\n");
			parser_stack.push_back(ELSE);	
			parint[end_index_hold.back()] = parser_stack.size()-1;
			end_index_hold.pop_back();
			parint.push_back(-1);
			end_index_hold.push_back(parint.size()-1);
		}
		else if(lextok[i] == END){
			printf("end geldi\n");
			parser_stack.push_back(END);

			if(loop_index_hold.back() != -1){ //if its not -1 then it must be loop end!
				printf("hello loop\n");
				parser_stack.back() = END_LOOP;
				parint.push_back(loop_index_hold.back());
			}
			loop_index_hold.pop_back();

			parint[end_index_hold.back()] = parser_stack.size()-1;
			end_index_hold.pop_back();
		}
		else if(lextok[i] == WHILE){
			printf("WHiLE geldi\n");
			parser_stack2.push_back(WHILE);

		}
		else if(lextok[i] == CFUN){
			printf("CFUN!!\n");
			parser_stack2.push_back(CFUN);			
			cfun_code_hold.push_back(lexint[it_int++]);
		}
		else if(lextok[i] == FUN){
			if (lextok[i] == WORD){
				printf("Function definition :)\n");
			}
		}

		else if(lextok[i] == RESET || i == lextok.size()-1){
			printf("RESET \n");
			//push stack2 to stack1 by reverse order. 
			while(parser_stack2.size()>0){
				//parser_stack.push_back(parser_stack2.back());
				if(isBASIC_OPERATOR(parser_stack2.back())) 
					basic_operator_handler(parser_stack2.back());
				else parser_stack.push_back(parser_stack2.back());
				parser_stack2.pop_back();

			}
		}
	}
	printf("======end index holder=======\n");
	for (int i = 0; i <  end_index_hold.size(); ++i){
		printf("{%d}",end_index_hold[i]);
	}
	printf("\n======PARSER_STACK=======\n");
	for (int i = 0; i <  parser_stack.size(); ++i){
		printf("{%d : %s}",i,TYPES_ARRAY[parser_stack[i]].c_str());
	}

	printf("\n======PARSER_STACK2=======\n");
	for (int i = 0; i <  parser_stack2.size(); ++i){
		printf("{%d : %s}",i,TYPES_ARRAY[parser_stack2[i]].c_str());
	}
	printf("\n\n");
	return 1;
}
#endif
