#include "fbgc.h"

/*
	-------------------[fbgc_GRAMMAR]-------------------------
	
	GRAMMAR and PARSER algorithms work simultaneously. Sometimes grammar determines what is the current token and it can change the current token
	as well. Grammar has two main function left_seek and right_seek. Parser algorithm pushes and pops objects between main linked list and object
	stack. Therefore, whenever a push or pop happens grammar has to be called. 

	*What is right_seek and left_seek ?
	
	Starting with a basic example
	
	This is correct typing for many languages, in fbgc it is also correct.
	----------------------
		x = 5;
		y = 10;
	----------------------

	Without grammar, parser is able to read and convert this kind of input
	Lexer reads file and converts tokens into meaningful objects.
	This is the main linked list output
	[Main]->{REF:[x:NULL]}{=}{5}{;}{REF:[y:NULL]}{=}{10}{;}<->[T]
	Here x and y were assigned as reference objects in our symbol table.

	Parser converts this as following
	
	1-)Do not change x, take next
	2-)This is an operator, push into operator stack
	3-)This is a number do not change it
	4-)Semicolon is an operator and its precedence is lower than the assignemnt operator, operator stack
		must be popped into main stack,(in our case inserting before semicolon objects)
		and get rid of the semicolon, we do not need it

		Finally x = 5 has been converted to x,5,=
		so interpreter will find = operator and pop two objects from main linked list then will make the assignment.
	
	What about grammar while these are happening ?
	
	1-) Reading 'x'
	Grammar starts with NEWLINE token for the beginning. It always looks a one fbgc_token type variable. 
	Each iteration can change this variable. Grammar struct holds two variables, first is paranthesis flags, the other grammar token.
	When x is pushed into main stack(main linked list is the same thing, we don't change its location but it can be assumed as pushing operation)
	parser calls seek_left because it will push into main stack. We are looking what is there at the left side of the 'x'
	
	See below BNF notation, x is an 'REF' and its left NEWLINE is allowed, this condition is okay for our grammar
	Therefore it will make grammar ATOM, which is the object type. Since x is a ref object, grammar token will be reference.	
	
			This part is the allowed list for the tokens at the right hand side
			____________________________________________________________________
			v																   v						Our object is REF
	ATOM: 																										v
		(NEWLINE | &BINARY_OPERATOR | ASSIGNEMNT_OPERATOR | UNBALANCED_EXPR_LIST) ('INT' | 'STRING' | 'DOUBLE' | 'REF')
			^
		For REF, 'NEWLINE' is an allowed token for the LEFT_TABLE

	Now grammar token(i will call it gm) is REF
	
	2-) Reading '='
	Assignment operator will be pushed in op stack. It will seek its left. 

	&ASSIGNMENT_OPERATOR:
		REF ('=' | '+=' | '-=' | '*=' | '/=' | '<<=' | '>>=') 
	
	gm is REF, so it is allowed. '='  can be pushed in the op stack.
	Now gm is ASSIGN.

	3-) Reading '5'
	
	it is an INT object, its left gm is ASSIGN, it is allowed. gm is INT now.

	ATOM: 
		(NEWLINE | &BINARY_OPERATOR | ASSIGNMENT_OPERATOR | UNBALANCED_EXPR_LIST) ('INT' | 'STRING' | 'DOUBLE' | 'REF')
	
	4-) Reading ';'

		Pop '=' from op stack and push into the main stack. Now seek_right will be called.
		If there is a transfer between op stack to main stack, basically we are looking what was our right token.
		gm was INT and it is true since x = 5, 5 is at the right hand side of the assignment operator.
		It is allowed and its BNF notation is

		ASSIGNMENT_EXPRESSION: 
			&ASSIGNMENT_OPERATOR STATEMENT	

		Here Assignment operator will seek its right, an atom is a statement also. So it is allowed !
		Now, gm is assignement_expression.
	
	SEMICOLON: 
		* SEMICOLON

	Everything is allowed at the left of the semicolon.  
	
	And we finished our grammar checking and parsing in just one pass.

	Syntax:
	left -> return-token: left-side-token operator
	right-> return-token: operator right-side-token
	&binary_operator means '+' is a binary operator and it returns plus, * returns star, - return minus etc. 
	
	
	----------------------------------------------------[LEFT TABLE]----------------------------------------------------

	ASSIGNMENT_OPERATOR:
		ID ('=' | '+=' | '-=' | '*=' | '/=' | '<<=' | '>>=') 

	UNARY_OPERATOR: 
		(NEWLINE | LPARA | ASSIGNMENT_OPERATOR | BINARY_OPERATOR) ( + | - | ! | ++ | -- )

	BINARY_OPERATOR:
		EXPRESSION ('+' | '-' | '*' | '/' | '&' | '|' | '==') 
	
	ATOM: 
		(NEWLINE | BINARY_OPERATOR | ASSIGNMENT_OPERATOR | UNBALANCED_EXPR_LIST | LPARA | LBRACK  | LBRACE) (INT | STRING | DOUBLE | ID)	

	LPARA: 
		(NEWLINE | OPERATOR | UNBALANCED_EXPR_LIST | IF_BEGIN | ELIF_BEGIN | WHILE_BEGIN | LPARA) LPARA

	FUN_ARG_LPARA:
		ID LPARA


	LBRACK:
		(NEWLINE | ID | OPERATOR | UNBALANCED_LIST | SEMICOLON) LBRACK
	

	UNBALANCED_ID_LIST:
		ID COMMA 	

	UNBALANCED_EXPR_LIST:
		(EXPRESSION) COMMA 
	
	IF_BEGIN:
		NEWLINE IF		
	
	ELIF_BEGIN:
		IF STATEMENT 'ELIF'
   
	ELSE_BEGIN: 
		(IF | ELIF) STATEMENT 'ELSE'

	FUN:
		ASSIGNMENT FUN

	PARA_BALANCED_EXPR_LIST:
		( (BALANCED_EXPR_LIST | EXPRESSION) & LPARA_FLAG == 1)  RPARA
	
	EXPRESSION
		:ATOM
	 	|UNARY_EXPRESSION
	 	|BINARY_EXPRESSION
	 	|PARA_EXPRESSION
	 	|BALANCED_EXPRESSION_LIST
	 	|EXPRESSION

	OPERATOR
		:BINARY_OPERATOR
		|UNARY_OPERATOR
		|ASSIGNMENT_OPERATOR
	
	ASSIGNMENT
		:ASSIGNMENT_EXPRESSION
		|PARA_ASSIGNMENT_EXPRESSION

	STATEMENT
		:EXPRESSION
		|ASSIGNMENT

	:>---------------------------------------[RIGHT_TABLE]-----------------------------------------------
	

 	UNARY_EXPRESSION: 
		UNARY_OP EXPRESSION 

	BINARY_EXPRESSION: 
		('+' | '-' | '*' | '/' | '&' | '|' | '==' | '>' | '<' | '>=' | '<=' | '&' | '|') EXPRESSION		

	ASSIGNMENT_EXPRESSION: 
		&ASSIGNMENT_OPERATOR STATEMENT		

	IF:	
		IF_BEGIN PARA_EXPRESSION
	ELIF:
		ELIF PARA_EXPRESSION 
	
	BALANCED_EXPR_LIST: 
		',' EXPRESSION	
	BALANCED_IDENTIFIER_LIST:
		',' ID

	LPARA_FLAG_1:
		LPARA EXPRESSION
	
	LBRACK_FLAG_1:
		LBRACK EXPRESSION

	
*/



uint8_t gm_seek_left(struct fbgc_grammar * gm, struct fbgc_object * obj){

	#define gm_left (gm->top)
	#ifdef GRAMMAR_DEBUG
		cprintf(100,"Seek LEFT:[%s], OBJ:[%s] flag{0x%X}\n",object_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)],gm->flag);
	#endif

	if(gm == NULL || obj == NULL) return 0;

	if( (is_fbgc_ATOM(get_fbgc_object_type(obj))  ) 
		&& 
		(   is_fbgc_BINARY_OPERATOR(gm_left) || 
			is_fbgc_UNARY_OPERATOR(gm_left)  || 
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
			gm_left == UNBALANCED_EXPRESSION_LIST  || 
			gm_left == LPARA || gm_left == LBRACK || gm_left == LBRACE ||
			gm_left == IF_BEGIN || gm_left == ELIF_BEGIN ||
			gm_left == ELSE || gm_left == SEMICOLON ||
			gm_left == NEWLINE
		) 
	)
	{
		gm->top =get_fbgc_object_type(obj);
	}
	else if(  (is_fbgc_IDENTIFIER(get_fbgc_object_type(obj))  || get_fbgc_object_type(obj) == IDENTIFIER)
		&& 
		( is_fbgc_BINARY_OPERATOR(gm_left) || 
			is_fbgc_UNARY_OPERATOR(gm_left)  || 
			gm_left == UNBALANCED_EXPRESSION_LIST  || 
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
			gm_left == LBRACK ||  gm_left == LPARA || gm_left == LBRACE ||
			gm_left == IF_BEGIN || gm_left == ELIF_BEGIN ||
			gm_left == ELSE || gm_left == SEMICOLON ||
			gm_left == NEWLINE ))
	{
		gm->top = get_fbgc_object_type(obj);
	}	
	else if(get_fbgc_object_type(obj) == LPARA &&
			(is_fbgc_BINARY_OPERATOR(gm_left) ||
			is_fbgc_UNARY_OPERATOR(gm_left)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
		 	gm_left == UNBALANCED_EXPRESSION_LIST || 
		 	gm_left == LPARA || gm_left == IF || gm_left == ELIF || gm_left == WHILE  || 
		 	gm_left == LOAD || gm_left == SEMICOLON ||
		 	is_fbgc_IDENTIFIER(gm_left) || gm_left == COMMA ||
		 	gm_left == NEWLINE ))
	{	
		gm->top = LPARA;
	}
	else if(get_fbgc_object_type(obj) == LBRACK && gm_left == IDENTIFIER)
	{
		gm->top = LBRACK;

	}	
	else if(get_fbgc_object_type(obj) == LBRACK &&
			(is_fbgc_BINARY_OPERATOR(gm_left) ||
			is_fbgc_UNARY_OPERATOR(gm_left)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
		 	gm_left == UNBALANCED_EXPRESSION_LIST || gm_left == LBRACK || gm_left == SEMICOLON
		 	|| gm_left == NEWLINE) )
	{
		gm->top = LBRACK;		
	}
	else if(get_fbgc_object_type(obj) == LBRACE && gm_left == START)
	{
		gm->top = LBRACE;		
	}	
	else if( (is_fbgc_UNARY_OPERATOR(get_fbgc_object_type(obj)) ||
			 get_fbgc_object_type(obj) == PLUS ||get_fbgc_object_type(obj) == MINUS) 
			&& 
			(is_fbgc_BINARY_OPERATOR(gm_left) || is_fbgc_ASSIGNMENT_OPERATOR(gm_left) || gm_left == LPARA || gm_left == NEWLINE) )

	{
		if(get_fbgc_object_type(obj) == PLUS) obj->type = UPLUS;
		else if(get_fbgc_object_type(obj) == MINUS) obj->type = UMINUS;
			
		gm->top = get_fbgc_object_type(obj);		
	}
	else if(get_fbgc_object_type(obj) == PIPE && gm_left == LEN){
		gm->top = LEN;
	}
	else if(get_fbgc_object_type(obj) == PIPE && is_fbgc_EXPRESSION(gm_left) == 0){
		gm->top = obj->type = LEN;
	}
	else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_left)){
		gm->top = get_fbgc_object_type(obj);
	}
	else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(obj)) && (gm_left == IDENTIFIER ||  gm_left == BUILD_TUPLE)){
		gm->top = get_fbgc_object_type(obj);
	}
	else if(is_fbgc_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_left)){
		gm->top = get_fbgc_object_type(obj);
	}
	else if(get_fbgc_object_type(obj) == COMMA && (is_fbgc_EXPRESSION(gm_left) || gm_left == ASSIGNMENT_EXPRESSION) ){
		gm->top = UNBALANCED_EXPRESSION_LIST;
	}	
	else if(get_fbgc_object_type(obj) == RPARA  && is_grammar_LPARA_flag_open(gm->flag) &&  (gm_left == BALANCED_EXPRESSION_LIST || gm_left == UNBALANCED_EXPRESSION_LIST) ){
		grammar_close_LPARA_flag(gm->flag);
		gm->top = BUILD_TUPLE;			
	}
	else if(get_fbgc_object_type(obj) == RPARA && is_grammar_LPARA_flag_open(gm->flag) && (is_fbgc_EXPRESSION(gm_left) || gm_left == ASSIGNMENT_EXPRESSION) ){	
		grammar_close_LPARA_flag(gm->flag);
		gm->top = MONUPLE;
	}	
	else if(get_fbgc_object_type(obj) == RPARA && is_grammar_LPARA_flag_open(gm->flag) ){
		grammar_close_LPARA_flag(gm->flag);
		gm->top = NUPLE;
	}	
	else if(get_fbgc_object_type(obj) == RBRACK && is_grammar_LBRACK_flag_open(gm->flag) && 
		(gm_left == BALANCED_EXPRESSION_LIST || is_fbgc_EXPRESSION(gm_left)) ){
		grammar_close_LBRACK_flag(gm->flag);
		gm->top = (BUILD_MATRIX);		
	}
	else if(get_fbgc_object_type(obj) == RBRACK && is_grammar_LBRACK_flag_open(gm->flag) ){
		grammar_close_LBRACK_flag(gm->flag);
		gm->top = MONATRIX;
	}		
	else if(get_fbgc_object_type(obj) == SEMICOLON && gm_left == LBRACK){
		gm->top = ROW;
	}
	else if(get_fbgc_object_type(obj) == SEMICOLON && (is_fbgc_STATEMENT(gm_left) || gm_left == SEMICOLON  || gm_left == NEWLINE)){
		gm->top = SEMICOLON;
	}

	else if(get_fbgc_object_type(obj) == IF && (is_fbgc_STATEMENT(gm_left) ||gm_left == ELSE || gm_left == ELIF ||
			gm_left == NEWLINE || gm_left == LBRACE)){
		gm->top = IF;		
	}
	
	else if(get_fbgc_object_type(obj) == ELIF &&  is_grammar_BEGIN_flag_open(gm->flag) && (gm_left == IF_BEGIN || gm_left == ELIF_BEGIN) ){
		gm->top = ELIF;		
	}	
	else if(get_fbgc_object_type(obj) == ELSE &&  is_grammar_BEGIN_flag_open(gm->flag) &&( gm_left == IF_BEGIN || gm_left == ELIF_BEGIN) ){
		gm->top = (ELSE);	
		grammar_close_BEGIN_flag(gm->flag);	
	}	
	else if(get_fbgc_object_type(obj) == WHILE && (is_fbgc_STATEMENT(gm_left) ||gm_left == ELSE || gm_left == ELIF ||
			gm_left == NEWLINE || gm_left == LBRACE)){
		gm->top = WHILE;		
	}
	else if(get_fbgc_object_type(obj) == FOR && (is_fbgc_STATEMENT(gm_left) ||gm_left == ELSE || gm_left == ELIF ||
			gm_left == NEWLINE || gm_left == LBRACE)){
		gm->top = WHILE;		
	}			
	else if(get_fbgc_object_type(obj) == END && (is_fbgc_EXPRESSION(gm_left)) ){

		gm->top = EXPRESSION;
	}
	else if(get_fbgc_object_type(obj) == LOAD && (gm_left == NEWLINE || gm_left == ASSIGN)){
		gm->top = LOAD;
	}
	else if(get_fbgc_object_type(obj) == BREAK && (gm_left == IF_BEGIN || gm_left == ELIF_BEGIN || gm_left == NEWLINE || gm_left == SEMICOLON)){
		gm->top = BREAK;
	}
	else if(get_fbgc_object_type(obj) == CONT && (gm_left == IF_BEGIN || gm_left == ELIF_BEGIN || gm_left == NEWLINE || gm_left == SEMICOLON)){
		gm->top = CONT;
	}
	else if(get_fbgc_object_type(obj) == NEWLINE && (is_fbgc_EXPRESSION(gm_left) || gm_left == ASSIGNMENT_EXPRESSION || gm_left == FOR_BEGIN || gm_left == IF_BEGIN || 
		gm_left == ELIF_BEGIN)  )
	{
		gm->top = NEWLINE;
	}	
	else{

		//cprintf(110,"ERROR\t");
		#ifdef GRAMMAR_DEBUG
		cprintf(100,"Unexpected grammar for L:[%s], Ob:[%s] flag{0x%X}\n",object_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)],gm->flag);
		#endif
		return 0;
	}
	#undef gm_left 
	
	return 1;
}


uint8_t gm_seek_right(struct fbgc_grammar * gm, struct fbgc_object * obj){

	#define gm_right (gm->top)
	#ifdef GRAMMAR_DEBUG
	cprintf(001,"Seek right: OBJ:[%s] Right:[%s] flag{0x%X}\n",object_name_array[get_fbgc_object_type(obj)],object_name_array[gm_right],gm->flag);
	#endif	

	if(gm == NULL || obj == NULL) return 0;

	if(get_fbgc_object_type(obj) == IDENTIFIER && (is_fbgc_EXPRESSION(gm_right) || is_fbgc_TUPLE(gm_right)) ){
		gm->top = EXPRESSION;
	}
	else if(get_fbgc_object_type(obj) == PIPE){
		if(gm_right == PIPE) gm->top = obj->type = LEN;
		else if(gm_right == LEN) gm->top = LEN; 
	}
	else if(get_fbgc_object_type(obj) == LEN && is_fbgc_EXPRESSION(gm_right)){
		gm->top = LEN;
	}	
	else if(is_fbgc_UNARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_right)){
		gm->top = (UNARY_EXPRESSION);
	}
	else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_right)){
		gm->top = (BINARY_EXPRESSION);
	}
	else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_STATEMENT(gm_right)){
		gm->top = (ASSIGNMENT_EXPRESSION);
	}
	else if(get_fbgc_object_type(obj) == COMMA && (is_fbgc_EXPRESSION(gm_right) || gm_right == ASSIGNMENT_EXPRESSION) ){
		gm->top = (BALANCED_EXPRESSION_LIST);
		//cprintf(111,"burada=!\n");
	}
	else if(is_fbgc_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_right)){
		gm->top = EXPRESSION;
	}

	//Why do we have this ?
	//else if(get_fbgc_object_type(obj) == COMMA && gm_right == UNBALANCED_EXPRESSION_LIST ){
	//	;
	//}	
	else if( get_fbgc_object_type(obj) == IF && (gm_right == MONUPLE || gm_right == BUILD_TUPLE) ){
		gm->top  = obj->type = IF_BEGIN;
	}
	else if( get_fbgc_object_type(obj) == ELIF && (gm_right == MONUPLE || gm_right == BUILD_TUPLE)){
		gm->top  = obj->type = ELIF_BEGIN;
	}
	else if( get_fbgc_object_type(obj) == WHILE && (gm_right == MONUPLE || gm_right == BUILD_TUPLE)){
		gm->top  = obj->type = WHILE_BEGIN;
	}
	else if( get_fbgc_object_type(obj) == FOR && (gm_right == MONUPLE || gm_right == BUILD_TUPLE)){
		gm->top  = obj->type = FOR_BEGIN;
	}							
	else if( get_fbgc_object_type(obj) == IF_BEGIN && is_fbgc_STATEMENT(gm_right) ){
		gm->top = IF_BEGIN;
		grammar_open_BEGIN_flag(gm->flag);		
	}
	else if( get_fbgc_object_type(obj) == ELIF_BEGIN && is_fbgc_STATEMENT(gm_right) ){
		gm->top = ELIF_BEGIN;
		grammar_open_BEGIN_flag(gm->flag);		
	}
	else if( get_fbgc_object_type(obj) == WHILE_BEGIN && is_fbgc_STATEMENT(gm_right) ){
		gm->top = WHILE_BEGIN;
		grammar_open_BEGIN_flag(gm->flag);		
	}
	else if( get_fbgc_object_type(obj) == FOR_BEGIN && is_fbgc_STATEMENT(gm_right) ){
		gm->top = FOR_BEGIN;
		grammar_open_BEGIN_flag(gm->flag);		
	}			
	else if(get_fbgc_object_type(obj) == LOAD && gm_right == MONUPLE){
		gm->top = LOAD;
	}
	else if(get_fbgc_object_type(obj) == CFUN && is_fbgc_TUPLE(gm_right)){
		gm->top = CFUN;
	}	
	else if(get_fbgc_object_type(obj) == LPARA && gm_right == LPARA){
		grammar_open_LPARA_flag(gm->flag);
	}
	else if(get_fbgc_object_type(obj) == LPARA && (is_fbgc_EXPRESSION(gm_right) || gm_right == ASSIGNMENT_EXPRESSION|| 
			gm_right == BALANCED_EXPRESSION_LIST || gm_right == UNBALANCED_EXPRESSION_LIST) ){
		grammar_open_LPARA_flag(gm->flag);		
	}
	else if(get_fbgc_object_type(obj) == LBRACK && gm_right == LBRACK){
		grammar_open_LBRACK_flag(gm->flag);
	}	
	else if(get_fbgc_object_type(obj) == LBRACK && (is_fbgc_EXPRESSION(gm_right) || gm_right == BALANCED_EXPRESSION_LIST)){
		grammar_open_LBRACK_flag(gm->flag);
	}
	else if(get_fbgc_object_type(obj) == LBRACE && (is_fbgc_EXPRESSION(gm_right) || gm_right == ASSIGNMENT_EXPRESSION || gm_right == NEWLINE)){
		grammar_open_LBRACE_flag(gm->flag);
	}

	else {
		//cprintf(110,"ERROR\t");
		#ifdef GRAMMAR_DEBUG
		cprintf(100,"Unexpected grammar Ob:[%s] R:[%s] flag{0x%X}\n",object_name_array[get_fbgc_object_type(obj)],object_name_array[gm_right],gm->flag);
		#endif
		return 0;
	}
	#undef gm_right

	return 1;
}
