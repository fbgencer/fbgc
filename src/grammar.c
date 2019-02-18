#include "fbgc.h"

/*
	:>-------------------[fbgc_GRAMMAR]-------------------------
	:>Syntax:
	:>	left -> return-token: left-side-token operator
	:>	right-> return-token: operator right-side-token
	:>	&binary_operator means '+' is a binary operator and it returns plus, * returns star, - return minus etc. 
	
	LEFT TABLE

	ATOM: 
		(START | &BINARY_OPERATOR | UNBALANCED_EXPR_LIST) ('INT' | 'STRING' | 'DOUBLE' | 'ID')	
	LPARA: 
		(START| &BINARY_OPERATOR | &UNARY_OPERATOR | &ASSIGNMENT_OP| UNBALANCED_EXPR_LIST) '('

	&UNARY_OPERATOR: 
		(&BINARY_OPERATOR | LPARA | START )* ( + | - | ! | ++ | -- )
		:> FOR EXAMPLE IF ! COMES, IT LOOKS ITS LEFT AND IF IT SEE SOMETHING LIKE X+!Y, (!X), !X IT RETURNS NOT_OP

	&BINARY_OPERATOR:
		EXPRESSION ('+' | '-' | '*' | '/' | '&' | '|' | '==') 
	
	&ASSIGNMENT_OPERATOR:
		ID ('=' | '+=' | '-=' | '*=' | '/=' | '<<=' | '>>=') 
	
	
	UNBALANCED_EXPR_LIST:
		EXPRESSION ',' 
	UNBALANCED_IDENTIFIER_LIST: 
		ID ','	

	PARA_EMPTY:
		LPARA ')' 
	PARA_EXPRESSION:
		LPARA EXPRESSION ')'
	PARA_ASSIGNMENT_EXPRESSION: 
		LPARA ASSIGNMENT_EXPRESSION ')'
	PARA_BALANCED_EXPR_LIST:
		LPARA (BALANCED_EXPR_LIST | EXPRESSION) ')'
	PARA_IDENTIFIER_LIST: 
		LPARA BALANCED_IDENTIFIER_LIST ')'

	LBRACK:
		(START| &BINARY_OPERATOR | &UNARY_OPERATOR | &ASSIGNMENT_OP| UNBALANCED_LIST) '['
	
	MATRIX: 
		LBRACK BALANCED_EXPR_LIST ']'
	
	LBRACE:
		(START| &BINARY_OPERATOR | &UNARY_OPERATOR | &ASSIGNMENT_OP| UNBALANCED_LIST) '{'
	
	LIST:
		LBRACE BALANCED_EXPR_LIST '}'
	

	
	EXPRESSION
		:ATOM
	 	|UNARY_EXPRESSION
	 	|BINARY_EXPRESSION
	 	|PARA_EXPRESSION
	 	|BALANCED_EXPRESSION_LIST
	 	|MATRIX
	 	|LIST

	ASSIGNMENT
		:ASSIGNMENT_EXPRESSION
		|PARA_ASSIGNMENT_EXPRESSION

	STATEMENT
		:EXPRESSION
		|ASSIGNMENT
		

	ELIF:
		IF STATEMENT 'ELIF'
   
	ELSE: 
		(IF | ELIF) STATEMENT 'ELSE'

 	END:
 		(IF | ELIF | ELSE | WHILE | UNTIL) (STATEMENT) 'END'
	
	:>---------------------------------------[RIGHT_TABLE]-----------------------------------------------
	

 	UNARY_EXPRESSION: 
		UNARY_OP EXPRESSION :> !(X+Y) OR !X SITUATION IT RETURNS UNARY_EXPRESSION 	

	BINARY_EXPRESSION: 
		('+' | '-' | '*' | '/' | '&' | '|' | '==' | '>' | '<' | '>=' | '<=' | '&' | '|') EXPRESSION		

	ASSIGNMENT_EXPRESSION: 
		&ASSIGNMENT_OPERATOR STATEMENT		
	IF:	
		'IF' PARA_EXPRESSION
	ELIF:
		ELIF PARA_EXPRESSION 


	
	BALANCED_EXPR_LIST: 
		',' EXPRESSION	
	BALANCED_IDENTIFIER_LIST:
		',' ID

	FUN_CALL: 
		'FUN_ID' (PARA_BALANCED_EXPR_LIST | PARA_EMPTY)		
	WHILE: 
		'WHILE' PARA_EXPRESSION
	UNTIL:
		'UNTIL' PARA_EXPRESSION

	a = [1;2]; 
	b = [5,10,20]; 
	c = [a,[b;8,9,10]]
	
	c:
	    1    5   10   20
	    2    8    9   10			
	
	M:  
	O: 
	G:
	
*/



uint8_t gm_seek_left(struct fbgc_grammar * gm, struct fbgc_object * obj){

	#define gm_left (gm->top)
	#ifdef GRAMMAR_DEBUG
		cprintf(100,"Seek LEFT:[%s], OBJ:[%s] flag{0x%X}\n",object_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)],gm->flag);
	#endif

	if( (is_fbgc_ATOM(get_fbgc_object_type(obj))  ) 
		&& 
		( is_fbgc_BINARY_OPERATOR(gm_left) || 
			is_fbgc_UNARY_OPERATOR(gm_left)  || 
			gm_left == UNBALANCED_EXPRESSION_LIST  || 
			gm_left == LPARA || is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
			gm_left == LBRACK || gm_left == IF_BEGIN || gm_left == BEGIN || gm_left == ELSE || gm_left == SEMICOLON ||
			is_fbgc_START(gm_left) ) )
	{
		gm->top =get_fbgc_object_type(obj);
	}
	else if(  (is_fbgc_IDENTIFIER(get_fbgc_object_type(obj))  || get_fbgc_object_type(obj) == REFERENCE)
		&& 
		( is_fbgc_BINARY_OPERATOR(gm_left) || 
			is_fbgc_UNARY_OPERATOR(gm_left)  || 
			gm_left == UNBALANCED_EXPRESSION_LIST  || 
			gm_left == LPARA || is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
			gm_left == LBRACK || 
			gm_left == IF_BEGIN || gm_left == BEGIN || gm_left == ELSE || gm_left == SEMICOLON ||
			is_fbgc_START(gm_left) ))
	{
		gm->top = IDENTIFIER;
	}	
	else if(get_fbgc_object_type(obj) == LPARA &&
			(is_fbgc_BINARY_OPERATOR(gm_left) ||
			is_fbgc_UNARY_OPERATOR(gm_left)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
		 	gm_left == UNBALANCED_EXPRESSION_LIST || 
		 	gm_left == LPARA || gm_left == IF || gm_left == ELIF || gm_left == LOAD || gm_left == SEMICOLON ||
		 	gm_left == IDENTIFIER ||
		 	is_fbgc_START(gm_left) ))
	{	
		gm->top = LPARA;
	}
	else if(get_fbgc_object_type(obj) == LBRACK &&
			(is_fbgc_BINARY_OPERATOR(gm_left) ||
			is_fbgc_UNARY_OPERATOR(gm_left)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
		 	gm_left == UNBALANCED_EXPRESSION_LIST || gm_left == LBRACK || gm_left == SEMICOLON
		 	|| gm_left == IDENTIFIER ||
		 	is_fbgc_START(gm_left)) )
	{
		gm->top = LBRACK;		
	}
	else if( (is_fbgc_UNARY_OPERATOR(get_fbgc_object_type(obj)) ||
			 get_fbgc_object_type(obj) == PLUS ||get_fbgc_object_type(obj) == MINUS) 
			&& 
			(is_fbgc_BINARY_OPERATOR(gm_left) || gm_left == LPARA || is_fbgc_START(gm_left)) )

	{
		if(get_fbgc_object_type(obj) == PLUS) obj->type = UPLUS;
		else if(get_fbgc_object_type(obj) == MINUS) obj->type = UMINUS;	
		gm->top = get_fbgc_object_type(obj);		
	}
	else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_left)){
		//cprintf(111,"binar->expr");
		gm->top = get_fbgc_object_type(obj);
	}
	else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(obj)) && (gm_left == IDENTIFIER || gm_left == RAW_TUPLE)){
		gm->top = get_fbgc_object_type(obj);
	}
	else if(get_fbgc_object_type(obj) == COMMA && is_fbgc_EXPRESSION(gm_left) ){
		gm->top = UNBALANCED_EXPRESSION_LIST;
	}	
	else if(get_fbgc_object_type(obj) == RPARA  && is_grammar_LPARA_flag_open(gm->flag) &&  (gm_left == BALANCED_EXPRESSION_LIST || gm_left == UNBALANCED_EXPRESSION_LIST) ){
		grammar_close_LPARA_flag(gm->flag);
		gm->top = RAW_TUPLE;			
	}
	else if(get_fbgc_object_type(obj) == RPARA && is_grammar_LPARA_flag_open(gm->flag) && is_fbgc_EXPRESSION(gm_left)){	
		grammar_close_LPARA_flag(gm->flag);
		gm->top = MONUPLE;
	}	
	else if(get_fbgc_object_type(obj) == RPARA && is_grammar_LPARA_flag_open(gm->flag) ){
		grammar_close_LPARA_flag(gm->flag);
		gm->top = NUPLE;
	}	
	else if(get_fbgc_object_type(obj) == RBRACK && is_grammar_LBRACK_flag_open(gm->flag) && (gm_left == BALANCED_EXPRESSION_LIST || is_fbgc_EXPRESSION(gm_left)) ){
		grammar_close_LBRACK_flag(gm->flag);
		gm->top = (RAW_MATRIX);		
	}
	/*else if(get_fbgc_object_type(obj) == SEMICOLON && gm_left == BALANCED_EXPRESSION_LIST){
		gm->top = ROW;
	}*/
	else if(get_fbgc_object_type(obj) == SEMICOLON){
		gm->top = SEMICOLON;
	}
	else if(get_fbgc_object_type(obj) == IF_BEGIN && (gm_left == BEGIN || gm_left == ELSE_BEGIN)){
		gm->top = IF_BEGIN;
		//grammar_open_BEGIN_flag(gm->flag);
	}	
	else if(get_fbgc_object_type(obj) == IF && (is_fbgc_STATEMENT(gm_left) ||gm_left == ELSE || gm_left == ELIF ||gm_left == SEMICOLON|| is_fbgc_START(gm_left))){
		gm->top = (IF);		
	}
	else if(get_fbgc_object_type(obj) == ELIF &&  is_grammar_BEGIN_flag_open(gm->flag) && gm_left == IF_BEGIN ){
		gm->top = (ELIF);		
	}	
	else if(get_fbgc_object_type(obj) == ELSE &&  is_grammar_BEGIN_flag_open(gm->flag) && gm_left == IF_BEGIN ){
		gm->top = (ELSE);		
	}		
	else if(get_fbgc_object_type(obj) == END && is_grammar_BEGIN_flag_open(gm->flag) && (gm_left == IF_BEGIN || gm_left == ELSE_BEGIN)){
		grammar_close_BEGIN_flag(gm->flag);
		gm->top = START;
	}
	else if(get_fbgc_object_type(obj) == LOAD && gm_left == START){
		gm->top = LOAD;
	}
	/*else if(get_fbgc_object_type(obj) == STRING && gm_left == LOAD){
		gm->top = PREPROCESSOR; //is it ?
	}*/
	else{
		cprintf(110,"ERROR\t");
		cprintf(100,"Unexpected grammar for L:[%s], Ob:[%s] flag{0x%X}\n",object_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)],gm->flag);
		return 0;
	}

	#undef gm_left 
	return 1;
}


uint8_t gm_seek_right(struct fbgc_grammar * gm, struct fbgc_object * obj){

	#define gm_right (gm->top)
	#ifdef GRAMMAR_DEBUG
	cprintf(100,"Seek right: OBJ:[%s] Right:[%s] flag{0x%X}\n",object_name_array[get_fbgc_object_type(obj)],object_name_array[gm_right],gm->flag);
	#endif	


	if(is_fbgc_UNARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_right)){
		gm->top = (UNARY_EXPRESSION);
	}
	else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_right)){
		gm->top = (BINARY_EXPRESSION);
	}
	else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_STATEMENT(gm_right)){
		gm->top = (ASSIGNMENT_EXPRESSION);
	}
	else if(get_fbgc_object_type(obj) == COMMA && is_fbgc_EXPRESSION(gm_right) ){
		gm->top = (BALANCED_EXPRESSION_LIST);
	}
	else if(get_fbgc_object_type(obj) == COMMA && gm_right == UNBALANCED_EXPRESSION_LIST ){
		;
	}	
	else if(
		(get_fbgc_object_type(obj) == IF ||  
		get_fbgc_object_type(obj) == ELIF ||
		get_fbgc_object_type(obj) == WHILE ||
		get_fbgc_object_type(obj) == UNTIL )
		&& gm_right == MONUPLE)
	{
		gm->top = BEGIN;
	}
	else if(get_fbgc_object_type(obj) == LOAD && is_fbgc_TUPLE(gm_right)){
		gm->top = LOAD;
	}
	else if(get_fbgc_object_type(obj) == CFUN && is_fbgc_TUPLE(gm_right)){
		gm->top = CFUN;
	}	
	else if(get_fbgc_object_type(obj) == LPARA && gm_right == LPARA){
		grammar_open_LPARA_flag(gm->flag);
	}
	else if(get_fbgc_object_type(obj) == LPARA && (is_fbgc_EXPRESSION(gm_right) || gm_right == BALANCED_EXPRESSION_LIST || gm_right == UNBALANCED_EXPRESSION_LIST) ){
		grammar_open_LPARA_flag(gm->flag);		
	}
	else if(get_fbgc_object_type(obj) == LBRACK && (is_fbgc_EXPRESSION(gm_right) || gm_right == BALANCED_EXPRESSION_LIST)){
		grammar_open_LBRACK_flag(gm->flag);
	}
	/*else if(
		(get_fbgc_object_type(obj) == IF ||  
		get_fbgc_object_type(obj) == ELIF ||
		get_fbgc_object_type(obj) == WHILE ||
		get_fbgc_object_type(obj) == UNTIL )
		&& is_fbgc_PARA_EXPRESSION(gm_right))
	{
		gm->top = (get_fbgc_object_type(obj));
	}*/
	else if(get_fbgc_object_type(obj) == IF_BEGIN){
		grammar_open_BEGIN_flag(gm->flag);
		gm->top = IF_BEGIN;
	}
	else if(get_fbgc_object_type(obj) == ELSE_BEGIN){
		grammar_open_BEGIN_flag(gm->flag);
		gm->top = ELSE_BEGIN;
	}		
	else {
		cprintf(110,"ERROR\t");
		cprintf(100,"Unexpected grammar Ob:[%s] R:[%s] flag{0x%X}\n",object_name_array[get_fbgc_object_type(obj)],object_name_array[gm_right],gm->flag);
		return 0;
	}
	#undef gm_right
	return 1;
}
