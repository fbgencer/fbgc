#ifndef LEXER_H
#define LEXER_H
#include "include.h"
#include "tokens.h"
#include "error.h"



//unsigned char uminus_flag,uplus_flag = 0;


//LEXER OUTPUTS FOR PARSER
//fbgtoken is unsigned char :'(
std::vector<fbgtoken> lextok;
std::vector<string> lexstr;
std::vector<string> lexword;
std::vector<int> lexint;
std::vector<double> lexdouble;





fbgtoken crt_token = UNKNOWN,pre_token = UNKNOWN;

string tempstr;
char *mys,*ps;


//TODO, WRITE A VECTOR.H
void lexpush(){


  if(isINT(crt_token)) {
    lexint.push_back(strtol(tempstr.c_str(),NULL,10));
   // llexint.push_back(strtol(tempstr.c_str(),NULL,10)); 
  }
  else if(isDOUBLE(crt_token)) {
    lexdouble.push_back(stold(tempstr));
    //llexdob.push_back(stold(tempstr));
  }
  else if(isSTR(crt_token)){
      lexstr.push_back(tempstr);
  }
  else if(isWORD(crt_token)){
   /* for(int q = 0; q<sizeof(math_function_table)/sizeof(string); q++){
      if(tempstr == print_function_table[q]) {
        crt_token = CFUN;
        lexint.push_back((PRINT_UFBG_CODE<<6)|(q));
      }
      else if(tempstr == math_function_table[q]) {
        crt_token = CFUN;
        lexint.push_back((MATH_UFBG_CODE<<6)|(q));
      }
    }*/
      if(crt_token != CFUN) lexword.push_back(tempstr);
  }

  lextok.push_back(crt_token);
  pre_token = crt_token;
}

#define pushc()({tempstr.push_back(*(ps++));})
#define eatspace()({  while(isspace(*(ps))) ps++; })
#define eatcomment(){ while(*(ps) != '\n' && *ps != '\0') ps++; ps++; }

void nexttoken(){  
  //update tokens
  if(crt_token != UNKNOWN) { 
   // syntax();//write a syntax checker
    lexpush(); tempstr = ""; 
  }
  if(*ps == ' ') eatspace();
  //if(isspace(*(ps))) {eatspace(); crt_token = SPACE; nexttoken();}
  else if(*(ps) == '#') eatcomment();

  if(isdigit(*(ps)) || (*ps == '.' && isdigit(*(ps+1))) ) crt_token = INT;//
  else if(isalpha(*(ps)) || (*(ps) == '_' && isalpha(*(ps+1))) ) crt_token = WORD;
  else if(ispara(*(ps))) crt_token = PARA; 
  else if(isop(*(ps))) crt_token = OP; 
  else if(*(ps) == '"' || *(ps) =='\'') crt_token = STRING;
  else if(*(ps) == ';' ) crt_token = RESET;
  else if(*(ps) == ',') crt_token = COMMA;
  else if(*(ps) == '\0') crt_token = EOS;
  else if(*ps == '\t') crt_token = TAB;
  else if(*ps == '\n') crt_token = NLINE;
  else crt_token = UNKNOWN;
 	//printf("crt_token :%s\n",TYPES_ARRAY[crt_token].c_str() );
}


uint8_t lexer(const string &str){ 
  mys = strdup(str.c_str());
  ps = mys;
  printf("Main String :%s\n",mys);
  bool digit_con = false; 
  int check_para = 0;

  //first update our ps to see what is coming first
  //next token also determines the type of char like NUM, OP, TAB, COMMA etc.
  nexttoken(); 

  //lets check 
while( *(ps) != '\0' && !ERROR ){ 

    if(crt_token == INT) { 
      if(isdigit( *(ps) ) ){
      	
      	//hexadecimal checker
        if(*(ps) == '0' &&  (*(ps+1) == 'x' || *(ps+1) == 'X') ){ 
          pushc(); pushc();
          do pushc(); while(ishex(*(ps)) || isdigit(*ps));  crt_token = HEX;
        } // immediately eat 'x' or 'X'
        else pushc(); 

       
        
        if(iscomplex(*ps) && !isnum(*(ps+1)) && !iscomplex(*(ps+1)) && !ishex(*(ps+1)) ) {
          ps++; crt_token = COMPLEX; nexttoken(); 
        }
        else if(*ps == 'E' || *ps == 'e') { pushc();
              if( isdigit(*(ps)) || *(ps) == '+' || *(ps) == '-' ) pushc();
              else error("Exponential fucked up.");
              while(isdigit(*ps)) pushc();
              if(*ps == '.')  error("Dot after Exponential");
              crt_token = DOUBLE;
              nexttoken();
        }
        else if(*ps == '.'){
          do pushc(); while(isdigit(*ps));
          if(*ps == '.') error("Too many decimal points in one number.");

          if(iscomplex(*ps)) { ps++; crt_token = COMPLEX;}
          else crt_token = DOUBLE;
          
          nexttoken();
        }
        else if( !isnum(*ps) && !ishex(*ps) && !iscomplex(*ps) ){
          //decide what type of the num 
          nexttoken(); 
        }
        else {
        	//do nothing we checked every situation
        	//if this is the condition then we can double check to ensure it is num
        	if(!isnum(*ps)) error("Digit error");
        }
        if(isalpha(*(ps))){error2("s","Letter after number not allowable!\n");}
      }
      else if(*ps == '.'){
        //floating number without zero .23, .10j etc
          do pushc(); while(isdigit(*ps));
          if(*ps == '.') error("Too many decimal points in one number.");

          if(iscomplex(*ps)) { ps++; crt_token = COMPLEX;}
          else crt_token = DOUBLE;
          
          nexttoken();        	
      }      
      else {  crt_token = UNKNOWN;  }
    }

    else if(crt_token == WORD){  pushc();
       
        if( !isdigit(*(ps)) && !isalpha(*(ps)) && *(ps) != '_'  ){
            //before everything check the string is reserved or not
            (crt_token = getRESWORDcode(tempstr) ) ? 0 : (crt_token = WORD);  
              nexttoken();   digit_con = false; //now digit can come after letter 
        }   
        else digit_con = true;
        
        if(isdigit(*(ps)) && digit_con ){ pushc(); 
          if(!isdigit(*(ps)) && !isalpha(*(ps)) && *(ps) != '_'  ){  
              nexttoken();
              digit_con = false; 
            } 
      }
    }
    else if (crt_token==OP){ 
    	pushc();
        //check is this op 2 char operator
        if( isop(*(ps)) ){

            string optemp = ""; 
            optemp.push_back(*(ps-1));
            optemp.push_back(*(ps));
	        //now we have to check again maybe this is kinda 3char op
	        if(isop(*(ps+1))){
	          	optemp.push_back(*(ps+1));
	          	if(isop_str(optemp)){
	          		//So we have 3 char op, then push last 2
	          		pushc();
	          		pushc();
	          	}
	          	else error("Undefined operator..");
	        }
	        else if(isop_str(optemp)) pushc();
	        else error("Undefined operator..");
        }
        //make sure crt_token is operator then send 
        crt_token = getOPcode(tempstr); // change the specific op type before sending 
        nexttoken();         
    }
    else if(crt_token == STRING){ 
        ps++;// we know pre token is <'> or <"> 
        while( *(ps) != '\0' && *ps != '\'' && *ps != '\"'){
          pushc(); 
          if(*(ps) == '\'') break;
          else if(*(ps) =='\"') break;
        }
        ps++;
       nexttoken(); 
    }
    else if(crt_token == PARA || crt_token == COMMA || crt_token == RESET || crt_token == TAB || crt_token == NLINE){ 
        pushc();
        if(crt_token == PARA) {
          crt_token =getPARAcode(tempstr); // which para ? 
          check_para++;
        }
        nexttoken();
    }
    //else if(crt_token == SPACE) {printf("fuck space\n" ); }
    else if(crt_token== EOS){ error("EOS!");return 0;}
    else if(crt_token == UNKNOWN) {error("TOKEN UNKOWN"); return 0;}
    else {error("TOKEN UNDEFINED"); return 0;}
}
  if(!ERROR && check_para %2 == 0){return 1;}
  else if(check_para %2 != 0) { error("PARANTHESIS ERROR\n"); return 0;}
  return 0;
}




#endif


/*
sonra kontrol et
void syntax(){
  if(lextok.size() > 0){
   // printf("L: %s crt_token :%s\n",TYPES_ARRAY[pre_token].c_str(),TYPES_ARRAY[crt_token].c_str());
    if(pre_token == WORD && (isOP(crt_token) || crt_token == COMMA || crt_token == RESET || isPARA(crt_token) ) );
     // printf("WORD++++++++++++++++++++++\n");
    else if(isOP(pre_token) && (isNUM(crt_token) || crt_token == WORD || isPARA(crt_token)) || 
      crt_token == MINUS || crt_token == PLUS );
     // printf("OP++++++++++++++++++++++\n");
    else if(isNUM(pre_token) && (isOP(crt_token) || crt_token == COMMA || crt_token == RESET || isPARA(crt_token)) );
      //printf("OP++++++++++++++++++++++\n");
    else if(pre_token == RESET && ( isNUM(crt_token) || crt_token == WORD || isPARA(crt_token) ) );
    //  printf("RESET++++++++++++++++++++++\n");
    else if(isPARA(pre_token) && ( isNUM(crt_token) || crt_token == WORD ||
     isPARA(crt_token)  || crt_token == RESET || crt_token == COMMA) );
      //printf("PARA++++++++++++++++++++++\n");
    else error2("sss","SYNTAX ERROR : [ ",tempstr.c_str()," ]") ;
      //printf("------------------------------\n");
  } 
}
*/