#include "fbgc.h"

const char * object_name_array[]={
	"UNKNOWN",
	"ATOM",
	"INT",
	"INT16",
	"INT2",
	"DOUBLE",
	"COMPLEX",
	"STRING",
	"IDENTIFIER",
	"WORD",
	"VAR",
	"FUN",
	"REFERENCE",
	"MATRIX",
	"LIST",
	"LINKED_LIST",
	"LINKED_LIST_TAIL",
	"STACK",
	"OP",
	"PLUS",
	"MINUS",
	"STAR",
	"SLASH",
	"MOD",
	"STARSTAR",
	"CARET",
	"OR_OP",
	"AND_OP",
	"GREATER",
	"LOWER",
	"LO_EQ",
	"GR_EQ",
	"EQ_EQ",
	"NOT_EQ",
	"IS_EQ",
	"IF_THEN",
	"R_ARROW",
	"L_ARROW",
	"COL_LO",
	"RW_ARROW",
	"LW_ARROW",
	"COLSTAR",
	"COLSLASH",
	"SLASHSLASH",
	"COLON",
	"RSHIFT",
	"LSHIFT",
	"ASSIGN",
	"PLUS_ASSIGN",
	"MINUS_ASSIGN",
	"STAR_ASSIGN",
	"SLASH_ASSIGN",
	"CONST_ASSIGN",
	"INC",
	"DECR",
	"UPLUS",
	"UMINUS",
	"NOT_OP",
	"SEMICOLON",
	"TWO_DOT",
	"THREE_DOT",
	"TWO_COLON",
	"TILDE",
	"COMMA",
	"DOT",
	"QUESTION_MARK",
	"DOLLAR",
	"PARA",
	"LPARA",
	"RPARA",
	"LBRACE",
	"RBRACE",
	"LBRACK",
	"RBRACK",
	"FOR",
	"WHILE",
	"UNTIL",
	"JUMP",
	"BREAK",
	"CONT",
	"IF",
	"ELSE",
	"ELIF",
	"IF_BEGIN",
	"ELSE_BEGIN",
	"BEGIN",
	"END",
	"START",
	"FINISH",
	"LOAD",
	"AND",
	"OR",
	"NOT",
	"SPACE",
	"TAB",
	"NEWLINE",
	"RETURN",
	"EXPRESSION",
	"ASSIGNMENT",
	"STATEMENT",
	"ASSIGNMENT_EXPRESSION",
	"UNARY_EXPRESSION",
	"BINARY_EXPRESSION",
	"BALANCED_IDENTIFIER_LIST",
	"UNBALANCED_IDENTIFIER_LIST",
	"BALANCED_EXPRESSION_LIST",
	"UNBALANCED_EXPRESSION_LIST",
	"VOID_EXPRESSION",
	"PARA_VOID_EXPRESSION",
	"PARA_EXPRESSION",
	"PARA_ASSIGNMENT_EXPRESSION",
	"PARA_BALANCED_EXPR_LIST",
	"PARA_IDENTIFIER_LIST",
	"PREPROCESSOR",
};





const token_struct operator_token_array[]={
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"},	
	{ASSIGN,"="},
	{PLUS,"+"},
	{MINUS,"-"},
	{STAR,"*"},
	{SLASH,"/"},
	{DOT,"."},
	{MOD,"%"},
	{COMMA,","},
	{CARET,"^"},
	{STARSTAR,"**"},
	{UPLUS,"+"}, //delete these
	{UMINUS,"-"},	
	{PLUS_ASSIGN,"+="},
	{MINUS_ASSIGN,"-="},
	{STAR_ASSIGN,"*="},
	{SLASH_ASSIGN,"/="},
	{CONST_ASSIGN,":="},
	{AND_OP,"&"},
	{OR_OP,"|"},
	{LOWER,"<"},
	{GREATER,">"},
	{LO_EQ,"<="},
	{GR_EQ,">="},
	{EQ_EQ,"=="},
	{IS_EQ,"?="},
	{NOT_EQ,"!="},
	{INC,"++"},
	{DECR,"--"},
	{IF_THEN,"=>"},
	{R_ARROW,"->"},
	{L_ARROW,"<-"},
	{SLASHSLASH,"//"},
	{NOT_OP,"!"},
	{COLON,":"},
	{SEMICOLON,";"},
	{RSHIFT,">>"},
	{LSHIFT,"<<"},
	{COL_LO,":>"},
	{RW_ARROW,"~>"},
	{LW_ARROW,"<~"},
	{TILDE,"~"},
	{TWO_DOT,".."},
	{THREE_DOT,"..."},
	{TWO_COLON,"::"},
	{DOLLAR,"$"},
	{COLSTAR,":*"},
	{COLSLASH,":/"},

};

const token_struct reserved_words_token_array[]={
	{IF,"if"},
	{ELSE,"else"},
	{ELIF,"elif"},
	{END,"end"},
	{FOR,"for"},
	{WHILE,"while"},
	{UNTIL,"until"},
	{BREAK,"break"},
	{CONT,"cont"},
	{RETURN,"ret"},
	{LOAD,"load"},
	{JUMP,"jump"}
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
  	ptr < ( reserved_words_token_array + (sizeof(reserved_words_token_array)/sizeof(token_struct)) ); ptr++ ) {
	  //	printf("ptr:%s s1:%s s2:%s result:%d\n",ptr->name,str1,str2,memcmp(str1,ptr->name,strlen(ptr->name)));
	  	if(!memcmp(str1,ptr->name,strlen(ptr->name))){
	  		return ptr->tokencode;
	  	}
 	}

  return UNKNOWN;
}