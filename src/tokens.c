#include "fbgc.h"

//This part of the code is used only for Debug.
//We don't need to know tokens and their string representation. Those are can be found in relexer.c file
//When we print AST we need to know the string repr. of the token so we are using below functions and arrays.

const char * object_name_array[]={
 TOKEN_LIST_AS_STRINGS()
};

const token_struct operator_token_array[]={
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"},
	{STARSTAR,"**"},
	{SLASHSLASH,"//"},
	{LO_EQ,"<="},
	{GR_EQ,">="},
	{EQ_EQ,"=="},
	{NOT_EQ,"!="},
	{R_SHIFT_ASSIGN,">>="},
	{L_SHIFT_ASSIGN,"<<="},	
	{PLUS_ASSIGN,"+="},
	{MINUS_ASSIGN,"-="},
	{STAR_ASSIGN,"*="},
	{SLASH_ASSIGN,"/="},
	{CARET_ASSIGN,"^="},
	{STARSTAR_ASSIGN,"**="},
	{SLASHSLASH_ASSIGN,"//="},	
	{PERCENT_ASSIGN,"%="},	
	{R_SHIFT,">>"},
	{L_SHIFT,"<<"},
	{ASSIGN,"="},
	{SEMICOLON,";"},
	{COMMA,","},
	{DOT,"."},
	{COLON,":"},
	{CARET,"^"},
	{TILDE,"~"},
	{PERCENT,"%"},
	{EXCLAMATION,"!"},
	{LOWER,"<"},
	{GREATER,">"},
	{PIPE,"|"},
	{AMPERSAND,"&"},
	{SLASH,"/"},
	{STAR,"*"},
	{MINUS,"-"},
	{PLUS,"+"},
	{UPLUS,"u+"},
	{UMINUS,"u-"},
};

const token_struct reserved_words_token_array[]={
	{IF,"if"},
	{ELSE,"else"},
	{ELIF,"elif"},
	{END,"end"},
	{FUN_MAKE,"fun"},
	{WHILE,"while"},
	{BREAK,"break"},
	{CONT,"cont"},
	{RETURN,"ret"},
	{LOAD,"load"},
};


const char * get_token_as_str(fbgc_token tok){
	for( const token_struct * ptr = operator_token_array; ptr < ( operator_token_array + (sizeof(operator_token_array)/sizeof(token_struct)) ); ptr++ ) 
  		if(tok ==  ptr->tokencode) return ptr->name;
  	for( 
  		const token_struct * ptr = reserved_words_token_array; 
  		ptr < ( reserved_words_token_array + (sizeof(reserved_words_token_array)/sizeof(token_struct)) ); 
  		ptr++ 
  		) 
  	if(tok == ptr->tokencode ) return ptr->name;

  return NULL;
} 

/*
	TODO
	Write a good memory comparison function..

*/

fbgc_token get_operator_code(const char *str){
  for( const token_struct * ptr = operator_token_array; ptr < ( operator_token_array + (sizeof(operator_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(!memcmp(str,ptr->name,strlen(str)) ) return ptr->tokencode;
  return UNKNOWN;
}


fbgc_token get_operator_code_from_substr(const char *str1,const char *str2){
	for( const token_struct * ptr = operator_token_array; ptr < ( operator_token_array + (sizeof(operator_token_array)/sizeof(token_struct)) ); ptr++ ) 
  		if(str2-str1 == strlen(ptr->name) && !memcmp(str1,ptr->name,str2-str1 ) )  {
  			//cprintf(001,"Comp str1:[%s],str[%s],ptr[%s] strlen(ptr->name)=%d, str2-str1=%d\n",str1,str2,ptr->name,strlen(ptr->name)==str2-str1,102);
  			return ptr->tokencode;
  		}
  	return UNKNOWN;
}

fbgc_token get_reserved_word_code(const char *str){ 

  for( 
  	const token_struct * ptr = reserved_words_token_array; 
  	ptr < ( reserved_words_token_array + (sizeof(reserved_words_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(!memcmp(str,ptr->name,strlen(str))) return ptr->tokencode;
  return UNKNOWN;
}

fbgc_token get_reserved_word_code_from_substr(const char *str1, const char *str2){ 
	
  for( 
  	const token_struct * ptr = reserved_words_token_array; 
  	ptr < ( reserved_words_token_array + (sizeof(reserved_words_token_array)/sizeof(token_struct)) ); ptr++ ) {
	  //	printf("ptr:%s s1:%s s2:%s result:%d\n",ptr->name,str1,str2,memcmp(str1,ptr->name,strlen(ptr->name)));
	  	if(!memcmp(str1,ptr->name,strlen(ptr->name))){
	  		return ptr->tokencode;
	  	}
 	}

  return UNKNOWN;
}