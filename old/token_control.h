#ifndef TOKENCONTROL_H
#define TOKENCONTROL_H

#include "include.h"


//Basit type checkers
#define isnum(x) ( (x>='0' && x<= '9') )
#define islet(x) ( (x>='a' && x<='z' ) )

//Check current char is op or not.
#define isop(c)(\
c == '+' || c == '-' || c == '*' || c == '/'||c == '='\
|| c == '^' || c == '?'||c == '<' ||c == '>'||c == '.'\
|| c == '%' || c == '~'||c == '>' || c == '|' ||\
c == '&' || c == ':'||c == '!'||c=='$')

#define ispara(c)(c == '(' || c == ')' || c == '[' || c == ']' || c== '{' || c == '}')
#define ishex(c) ((c >= 'a' && c <= 'f' )|| (c >= 'A' && c <= 'F' ))
#define iscomplex(c) (c == 'j' || c == 'i')
//#define isallowlet(c) ( ishex(c) || iscomplex(c))

bool isop_str(const string &str){
  for(iter = 0; iter<sizeof(ops)/sizeof(token_struct); iter++)
    if(str == ops[iter].name) return 1;
  return 0;
}


fbgtoken getPARAcode(const string &str){
  for(iter = 0; iter<sizeof(paras)/sizeof(token_struct); iter++)
    if(str == paras[iter].name) return paras[iter].tokencode;
  return 0;
}

fbgtoken getOPcode(const string &str){
  for(iter = 0; iter<sizeof(ops)/sizeof(token_struct); iter++){
    if(str == ops[iter].name) return ops[iter].tokencode;
  }
  return 0;
}
fbgtoken getRESWORDcode(const string &str){ 
  for(iter = 0; iter<sizeof(res_words)/sizeof(token_struct); iter++){
    if(str == res_words[iter].name) { return res_words[iter].tokencode; }}
  return 0;
}



#endif
