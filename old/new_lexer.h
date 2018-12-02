#ifndef LEXER_H
#define LEXER_H
#include "include.h"
#include "tokens.h"
#include "error.h"



//unsigned char uminus_flag,uplus_flag = 0;


//LEXER OUTPUTS FOR PARSER
//ufbg_token is unsigned char :'(
std::vector<ufbg_token> lextok;
std::vector<string> lexstr;
std::vector<string> lexword;
std::vector<int> lexint;
std::vector<double> lexdouble;


/*
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
    for(int q = 0; q<sizeof(math_function_table)/sizeof(string); q++){
      if(tempstr == print_function_table[q]) {
        crt_token = CFUN;
        lexint.push_back((PRINT_UFBG_CODE<<6)|(q));
      }
      else if(tempstr == math_function_table[q]) {
        crt_token = CFUN;
        lexint.push_back((MATH_UFBG_CODE<<6)|(q));
      }
    }
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
*/

/*
  NEW lexer commands
*/




uint8_t new_lexer( const string &str ){ 
  char *from_string_to_str_ptr = strdup(str.c_str());
  char *sptr = from_string_to_str_ptr;
  string tempstr;
  ufbg_token current_token = UNKNOWN;

#define push_character()({tempstr.push_back(*(sptr));})
#define pop_character()({tempstr.pop_back();})
#define next()( sptr++)
#define previous()(sptr--)
#define eatspace()({ do{next();} while(isspace(*(sptr)));  })
#define push_token_into_RESERVED_WORD_container()({lextok.push_back(current_token); tempstr ="";})
#define push_token_into_WORD_container()({ lexword.push_back(tempstr); lextok.push_back(WORD); tempstr =""; } )
#define push_token_into_STRING_container()({ lexstr.push_back(tempstr); lextok.push_back(STRING); tempstr =""; } )
#define push_token_into_INTEGER_container(base)({ lexint.push_back(strtol(tempstr.c_str(),NULL,base)); lextok.push_back(INT); tempstr =""; } )
#define push_token_into_DOUBLE_container()({ lexdouble.push_back(stold(tempstr)); lextok.push_back(DOUBLE); tempstr ="";} )

  printf("\t[ New lexer ]\nInput:%s |<\n",from_string_to_str_ptr);

  while( *sptr != '\0' ){
    //WORD : '[a-zA-Z_][a-zA-Z0-9_]*'
    if(is_letter(*sptr) || is_underscore(*sptr)){
      while(is_letter(*sptr) || is_underscore(*sptr) || is_digit(*sptr)){
        push_character();
        next();
      }

      //now check the current tempstr, is it reserved code or not ? 
      current_token = get_reserved_word_code(tempstr);
      if(current_token != 0){
        //token is a reserved word.
        //if token is a reserved word, we dont need to hold its string representation.
        push_token_into_RESERVED_WORD_container();
        current_token = WORD;
      }
      else {
        //token is an arbitrary word
        current_token = WORD;
        push_token_into_WORD_container();
      }
    }
    //STRING
    else if(is_singlequote(*sptr) || is_quote(*sptr)){
      //now take everything until we see another single quote
      char string_definition_char = *sptr; // maybe < " > maybe < ' > 
      next(); // get rid of the single quote sign
      while(*sptr !='\0'){ // lets save ourselves from endless loop
        push_character();
        next();
        // " bla bla\"xx "
        if(is_backslash(*sptr)){
          next(); 
          if( !(is_singlequote(*sptr)|| is_quote(*sptr) || is_backslash(*sptr)) ) return error("Undefined character."); 
          
        }
        else if( (is_singlequote(*sptr) && is_quote(string_definition_char) ) || ( is_quote(*sptr) && is_singlequote(string_definition_char) ) ){
          // " xx'xx " situation and ' xx"xx ' situation 
            push_character();
            next(); 
        }
        else if( *sptr == string_definition_char ){
          //now we completed our string
          next();
          break;
        }
        //now check some errors
        else if(*sptr == '\0') { return error("String definition is not finished."); } 
      }

      //we couldn't find an error, everything is expected
      current_token = STRING;
      push_token_into_STRING_container();
    }
    //OPERATOR
    else if(is_operator(*sptr)){
      //now an operator came, we don't know it is one length operator or 2 or 3, we have to check
      push_character();
      next();
      while(is_operator(*sptr) && *sptr != '\0'){ //it will run for every operator character
         

        //check unary operators,we are not sure it is unary or not we have to look other tokens as well
       /* if(is_unary(*sptr)){
          //check the tempstr,  maybe we have already an operator
          current_token = get_operator_code(tempstr);
          if(current_token != 0) 
         // ufbg_token previous_token lextok[lextok.size()-1];
          //if(previous_token == || previous_token == UNKNOWN)
            
        }*/
        
        //printf("previous token :%s\n",TYPES_ARRAY[lextok[lextok.size()-1]].c_str() );
        //if()
        push_character();
        next();       
      }

      //now we took all the operators, lets check like reserved word checking
      current_token = get_operator_code(tempstr);
      if(current_token != 0){
        //token is a defined operator.
        //if token is a defined operator we dont need to hold its string representation.
        //push it into the reserved word container
        push_token_into_RESERVED_WORD_container();
      }
      else {
        //token is zero, so it is an undefined operator, give an error
        return error("Undefined operator.");
      }
    }
    else if(isspace(*sptr)) eatspace();
    //NUMERIC
    else if( is_digit(*sptr) ){
      //now we have a digit like [0-9], we have check whether it is binary, hexa or scientific mantissa :{0b001, 0x02, 5E-6} 
      //without pushing lets check first if its not zero then we dont have binary or hexa
      if(*sptr != '0' ){
        //now here we know that the incoming number is not 0x.. or 0b.. , but it can be 2E-3 or 2E+3 these types are double type
        push_character();
        next(); 
        while(*sptr != '\0'){
          if(*sptr == '.'){
            //okey now we know that number will be double or there will be an error
            
          }
          else if(*sptr == 'E' || *sptr == 'e'){
            //exponential
          }

        }
      }
      else if(*sptr == '0'){
        //without pushing we know it is 0, we can push it later
        push_character();
        next();
        if(*sptr == 'b' || *sptr == 'B'){
          //binary input
          pop_character(); // we don't need 0b part we need the part that coming from after b
          next(); // now b is also gone
          while( *sptr != '\0'){
            if(*sptr != '0' && *sptr != '1'){ 
              //lets give some specific errors, or break to loop for succesful output
              if(is_operator(*sptr)) {
                //now this can be correct
                if(tempstr=="") return error("Syntax error while defining binary");
                break;
              }
              else if(is_digit(*sptr)) return error("Binary can only take 1 or 0");
              else return error("Error because of binary definiton");
            }
            push_character();
            next();
          }
          current_token = INT;
          push_token_into_INTEGER_container(2); //base = 2
        }
        else if(*sptr == 'x' || *sptr == 'X'){
         //without pushing we know it is 0, we can push it later
          pop_character();
          next(); // we got rid of 0 and x, we have convert previous number in 16 base

          while(*sptr != '\0'){ 
            if(!is_hexadecimal(*sptr) && !is_digit(*sptr) && is_operator(*sptr)){ 
              // it can be 0x=, prevent this
              if(tempstr=="") return error("Syntax error while defining hexadecimal");
              //now hexadecimal number is completed.
              break;
            }
            else if(!is_hexadecimal(*sptr) && is_letter(*sptr) ){ //to prevent 0x0GFH, and 0x2x01 0xx12 
              return error("Error because of hexadecimal definiton use between 0 and F");
            }
            push_character();
            next(); 
          }
          current_token = INT;
          push_token_into_INTEGER_container(16); //base = 16

        }
      }
    }
    //undefined
    else return error("Undefined state");
    
  }

  printf("current_token : %s",TYPES_ARRAY[current_token].c_str());
  return 1;
}

/*
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
*/




#endif
