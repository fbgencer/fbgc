#include "fbgc.h"





const char * object_name_array[]={
	"UNKNOWN",
	"BEGIN","END","END_LOOP","END_FUN","RESET",
	"INT","DOUBLE","COMPLEX","HEX","BIN","INDEX_L","INDEX_S",
	"WORD",
	"VAR",
	"FUN","CFUN","UFUN",
	"STRING",
	"LINKED_LIST",
	"OP",
	"PLUS","UPLUS","MINUS","UMINUS",
	"MULT","DIV","MOD","Q_MARK","POWER1","POWER2","INC","DECR",
	"ASSIGN","PLUS_ASSIGN","MIN_ASSIGN","MULT_ASSIGN","DIV_ASSIGN","CONST_ASSIGN",
	"OR_OP","AND_OP",
	"GREATER","LOWER",//>
	"LO_EQ","GR_EQ","EQ_EQ","NOT_EQ","IS_EQ","BIT_NOT_EQ",  // ?=
	"ISE", // =>
	"R_ARROW","L_ARROW","COL_LO","RW_ARROW","LW_ARROW","BIT_NOT","DOLLAR",
	"BIT_AND_OP","BIT_OR_OP",
	"COLMULT","COLDIV",

	"DIVDIV",// //
	"COLON","SEMICOLON", //;
	"NOT_OP",
	"RSHIFT","LSHIFT",
	"DOT","TWO_DOT","THREE_DOT","TWO_COLON",

	"PARA","LPARA","RPARA","LBRACE","RBRACE", "LBRACK","RBRACK",
	//Loops
	"FOR","WHILE","UNTIL","JUMP","BREAK","CONT",
	//If-Else
	"IF","ELSE","ELIF",
	//Logical
	"AND","OR","NOT",
	//Some Special cases
	"SPACE", "TAB","NLINE", 
	"EOS", // end of string
	"RETURN","IS","DEL","ARG","COMMA",
	"HALT",
	"VALID",
	"IDENTIFIER",
	"NUMBER",
	"EXPRESSION",
	"CLOSED_EXPRESSION",
	"COMP_EXPRESSION",
	"CLOSED_COMP_EXPRESSION",
	"ASSIGNMENT",	
};





static const token_struct operator_token_array[]={
	{ASSIGN,"="},
	{PLUS,"+"},
	{MINUS,"-"},
	{MULT,"*"},
	{DIV,"/"},
	{DOT,"."},
	{POWER1,"^"},
	{POWER2,"**"},
	{PLUS_ASSIGN,"+="},
	{MIN_ASSIGN,"-="},
	{MULT_ASSIGN,"*="},
	{DIV_ASSIGN,"/="},
	{CONST_ASSIGN,":="},
	{AND_OP,"&&"},
	{OR_OP,"||"},
	{BIT_AND_OP,"&"},
	{BIT_OR_OP,"|"},
	{LOWER,"<"},
	{GREATER,">"},
	{LO_EQ,"<="},
	{GR_EQ,">="},
	{EQ_EQ,"=="},
	{IS_EQ,"?="},
	{NOT_EQ,"!="},
	{BIT_NOT_EQ,"~="},
	{INC,"++"},
	{DECR,"--"},
	{ISE,"=>"},
	{R_ARROW,"->"},
	{L_ARROW,"<-"},
	{MOD,"%"},
	{DIVDIV,"//"},
	{NOT_OP,"!"},
	{COLON,":"},
	{SEMICOLON,";"},
	{RSHIFT,">>"},
	{LSHIFT,"<<"},
	{COL_LO,":>"},
	{RW_ARROW,"~>"},
	{LW_ARROW,"<~"},
	{BIT_NOT,"~"},
	{TWO_DOT,".."},
	{THREE_DOT,"..."},
	{TWO_COLON,"::"},
	{DOLLAR,"$"},
	{COLMULT,":*"},
	{COLDIV,":/"},

};

static const token_struct reserved_words_token_array[]={
	{IF,"if"},
	{ELSE,"else"},
	{ELIF,"elif"},
	{END,"end"},
	{BEGIN,"begin"},
	{FOR,"for"},
	{WHILE,"while"},
	{UNTIL,"until"},
	{VAR,"var"},
	{FUN,"fun"},
	{BREAK,"break"},
	{CONT,"cont"},
	{AND,"and"},
	{OR,"or"},
	{NOT,"not"},
	{RETURN,"ret"},
	{JUMP,"jump"},
	{IS,"is"},
	{DEL,"del"},
	{END_FUN,"endfun"}
};

static const token_struct paranthesis_token_array[]={
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"},
};



const char * get_token_as_str(fbgc_token tok){
	for( const token_struct * ptr = operator_token_array; ptr < ( operator_token_array + (sizeof(operator_token_array)/sizeof(token_struct)) ); ptr++ ) 
  		if(tok ==  ptr->tokencode) return ptr->name;

	for( const token_struct * ptr = paranthesis_token_array; ptr < ( paranthesis_token_array + (sizeof(paranthesis_token_array)/sizeof(token_struct)) ); ptr++ ) 
  		if(tok ==  ptr->tokencode) return ptr->name;

  for( 
  	const token_struct * ptr = reserved_words_token_array; 
  	ptr < ( reserved_words_token_array + (sizeof(reserved_words_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(tok == ptr->tokencode ) return ptr->name;

  return NULL;
} 


fbgc_token get_operator_code(const char *str){
  for( const token_struct * ptr = operator_token_array; ptr < ( operator_token_array + (sizeof(operator_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(!memcmp(str,ptr->name,strlen(str)) ) return ptr->tokencode;
  return UNKNOWN;
}


fbgc_token get_operator_code_from_substr(const char *str1,const char *str2){
  for( const token_struct * ptr = operator_token_array; ptr < ( operator_token_array + (sizeof(operator_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(!memcmp(str1,ptr->name,str2-str1) ) return ptr->tokencode;
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
  	ptr < ( reserved_words_token_array + (sizeof(reserved_words_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(!memcmp(str1,ptr->name,str2-str1)) return ptr->tokencode;
  return UNKNOWN;
}