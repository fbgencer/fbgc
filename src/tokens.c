// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"

//This part of the code is used only for Debug.
//We don't need to know tokens and their string representation. Those are can be found in relexer.c file
//When we print AST we need to know the string repr. of the token so we are using below functions and arrays.

const char * object_name_array[]={
 TOKEN_LIST_AS_STRINGS()
};


const char * gm_name_array[]={
	GRAMMAR_TOKENS_AS_STRINGS()
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
	{LOEQ,"<="},
	{GREQ,">="},
	{EQEQ,"=="},
	{NOTEQ,"!="},
	{RSHIFT_ASSIGN,">>="},
	{LSHIFT_ASSIGN,"<<="},	
	{PLUS_ASSIGN,"+="},
	{MINUS_ASSIGN,"-="},
	{STAR_ASSIGN,"*="},
	{SLASH_ASSIGN,"/="},
	{CARET_ASSIGN,"^="},
	{STARSTAR_ASSIGN,"**="},
	{SLASHSLASH_ASSIGN,"//="},	
	{PERCENT_ASSIGN,"%="},	
	{RSHIFT,">>"},
	{LSHIFT,"<<"},
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
	{RETURN,"ret"}
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





