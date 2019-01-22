#include "fbgc.h"
/*
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Left(Top)\Input  | Number     | Unary(+,-,!)  | Binary(+,-,*,/,&)     | LeftP         | RightP                |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Start Symbol(LL) | Push Atom  | Push unary op | if(+ or -) make       | Push LP       | Error                 |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
|                  |            |               | it unary, send unary  |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Unary            | Error(?)   |               | Error                 | Pop & Push LP |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Binary           | Pop & push | Pop and Push  | Error                 | Pop & Push LP |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| LeftP            | push Atom  | Push Unary    | Check unary, if not   | Push LP       |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
|                  |            |               | Error                 |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| RightP           | Error      | Error(?)      | Error                 | Error         |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Atom             | Error      |               | Pop & Push binary op  | Error         |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Expression       | Error      | Error(?)      | Pop & Push binary op  | Error         | Pop & Read until      |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
|                  |            |               |                       |               | Left Para, then push  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
|                  |            |               |                       |               | Closed Expr           |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                  |            |               |                       |               |                       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
+------------------+------------+---------------+-----------------------+---------------+-----------------------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

*/
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



	[1,2,3,4,5]
	M:1,2,3,4,5
	O:[ , , , , ]
	G:expr
	
	1,2,+,3,4,*,2,build_matrix
		

*/




uint8_t grammar_seek_left(struct fbgc_object * gm_head,struct fbgc_object * obj ){

	#define GM_LEFT get_fbgc_object_type(top_fbgc_ll_object(gm_head))
	#define change_top(x)(top_fbgc_ll_object(gm_head)->type = x)
	
	cprintf(111,"Left :[%s], obj_type:[%s]\n",object_name_array[GM_LEFT],object_name_array[get_fbgc_object_type(obj)]);


	if(is_fbgc_ATOM(get_fbgc_object_type(obj)) || is_fbgc_IDENTIFIER(get_fbgc_object_type(obj))){
		if(  is_fbgc_BINARY_OPERATOR(GM_LEFT)|| is_fbgc_UNARY_OPERATOR(GM_LEFT) || GM_LEFT == UNBALANCED_EXPR_LIST || is_fbgc_START(GM_LEFT) ){
			change_top(ATOM);
		}
		//else if(GM_LEFT == LPARA || GM_LEFT == LBRACE || GM_LEFT == LBRACK) 
		//	gm_head = push_front_fbgc_ll_object(gm_head,new_fbgc_object(ATOM));
	}
	/*else if(get_fbgc_object_type(obj) == LPARA &&
			(is_fbgc_BINARY_OPERATOR(GM_LEFT) ||
			is_fbgc_UNARY_OPERATOR(GM_LEFT)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(GM_LEFT) ||
		 	GM_LEFT == UNBALANCED_EXPR_LIST || 
		 	is_fbgc_START(GM_LEFT) ))
	{
			change_top(LPARA);		
	}*/
	/*else if(get_fbgc_object_type(obj) == LBRACK &&
			(is_fbgc_BINARY_OPERATOR(GM_LEFT) ||
			is_fbgc_UNARY_OPERATOR(GM_LEFT)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(GM_LEFT) ||
		 	GM_LEFT == UNBALANCED_EXPR_LIST || 
		 	is_fbgc_START(GM_LEFT) ))
	{
			change_top(LBRACK);		
	}*/
	else if(get_fbgc_object_type(obj) == LBRACE &&
			(is_fbgc_BINARY_OPERATOR(GM_LEFT) ||
			is_fbgc_UNARY_OPERATOR(GM_LEFT)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(GM_LEFT) ||
		 	GM_LEFT == UNBALANCED_EXPR_LIST || 
		 	is_fbgc_START(GM_LEFT) ))
	{
			change_top(LBRACE);		
	}	
	else if( (is_fbgc_UNARY_OPERATOR(get_fbgc_object_type(obj)) || get_fbgc_object_type(obj) == PLUS ||get_fbgc_object_type(obj) == MINUS) && 
		(is_fbgc_BINARY_OPERATOR(GM_LEFT) || GM_LEFT == LPARA || is_fbgc_START(GM_LEFT)) ){
		if(get_fbgc_object_type(obj) == PLUS) obj->type = UPLUS;
		else if(get_fbgc_object_type(obj) == MINUS) obj->type = UMINUS;	
		//if(GM_LEFT == LPARA || GM_LEFT == LBRACE || GM_LEFT == LBRACK) 
		//	gm_head = push_front_fbgc_ll_object(gm_head,new_fbgc_object(obj->type));
		//else 
			change_top(get_fbgc_object_type(obj));		
	}
	else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(GM_LEFT)){
		//cprintf(111,"binar->expr");
		change_top(get_fbgc_object_type(obj));
	}
	else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(obj)) && GM_LEFT == IDENTIFIER){
		change_top(get_fbgc_object_type(obj));
	}
	else if(get_fbgc_object_type(obj) == COMMA && is_fbgc_EXPRESSION(GM_LEFT) ){
		change_top(UNBALANCED_EXPR_LIST);
	}	
	else if(get_fbgc_object_type(obj) == COMMA && GM_LEFT == IDENTIFIER ){
		change_top(UNBALANCED_IDENTIFIER_LIST);
	}
	else if(get_fbgc_object_type(obj) == RBRACK && GM_LEFT == BALANCED_EXPR_LIST){
		delete_front_fbgc_ll_object(gm_head);
		if(GM_LEFT == LBRACK) change_top(MATRIX);
		else cprintf(111,"Error occured for rbrack->matrix\n");			
	}
	else if(get_fbgc_object_type(obj) == RBRACE && GM_LEFT == BALANCED_EXPR_LIST){
		delete_front_fbgc_ll_object(gm_head);
		if(GM_LEFT == LBRACE) change_top(LIST);
		else cprintf(111,"Error occured for rbrace->list\n");			
	}
	else if(get_fbgc_object_type(obj) == ELIF && is_fbgc_STATEMENT(GM_LEFT)){
		delete_front_fbgc_ll_object(gm_head);
		if(GM_LEFT == IF) change_top(ELIF);
		else cprintf(111,"error at elif\n");		
	}
	else if(get_fbgc_object_type(obj) == ELSE && is_fbgc_STATEMENT(GM_LEFT)){
		delete_front_fbgc_ll_object(gm_head);
		if(GM_LEFT == IF || GM_LEFT == ELIF) change_top(ELSE);
		else cprintf(111,"error at else\n");		
	}	
	else if(get_fbgc_object_type(obj) == END && is_fbgc_STATEMENT(GM_LEFT)){
		delete_front_fbgc_ll_object(gm_head);
		if(GM_LEFT == IF || GM_LEFT == ELIF || GM_LEFT == ELSE || GM_LEFT == WHILE || GM_LEFT == UNTIL)
			change_top(ELSE);
		else cprintf(111,"error at end\n");		
	}
	else if(get_fbgc_object_type(obj) == RPARA && GM_LEFT == LPARA_EMPTY){
		change_top(PARA_EMPTY);
	}	
	else if(get_fbgc_object_type(obj) == RPARA && GM_LEFT == LPARA_EXPRESSION){	
		change_top(PARA_EXPRESSION);
	}
	else if(get_fbgc_object_type(obj) == RPARA && GM_LEFT == LPARA_ASSIGNMENT_EXPRESSION){
		change_top(PARA_ASSIGNMENT_EXPRESSION);		
	}
	else if(get_fbgc_object_type(obj) == RPARA && GM_LEFT == LPARA_BALANCED_EXPR_LIST ){
		change_top(PARA_BALANCED_EXPR_LIST);;			
	}
	else if(get_fbgc_object_type(obj) == RPARA && GM_LEFT == BALANCED_IDENTIFIER_LIST){
		change_top(PARA_IDENTIFIER_LIST);
	}	
	else {
		cprintf(111,"ERROR at seek left\n");
		return 0;
	}	


	return 1;
}


uint8_t grammar_seek_right(struct fbgc_object * gm_head,struct fbgc_object * obj){

	#define GM_RIGHT get_fbgc_object_type(top_fbgc_ll_object(gm_head))
	#define change_top(x)(top_fbgc_ll_object(gm_head)->type = x)
	
	cprintf(111,"Obj_type:[%s] Right :[%s]\n",object_name_array[get_fbgc_object_type(obj)],object_name_array[GM_RIGHT]);

	if(is_fbgc_UNARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(GM_RIGHT)){
		change_top(UNARY_EXPRESSION);
	}
	else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(GM_RIGHT)){
		change_top(BINARY_EXPRESSION);
	}
	else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_STATEMENT(GM_RIGHT)){
		change_top(ASSIGNMENT_EXPRESSION);
	}
	else if(get_fbgc_object_type(obj) == COMMA && is_fbgc_EXPRESSION(GM_RIGHT) ){
		change_top(BALANCED_EXPR_LIST);
	}
	else if(get_fbgc_object_type(obj) == COMMA && GM_RIGHT == IDENTIFIER ){
		change_top(BALANCED_IDENTIFIER_LIST);
	}	
	else if(
		(get_fbgc_object_type(obj) == IF ||  
		get_fbgc_object_type(obj) == ELIF ||
		get_fbgc_object_type(obj) == WHILE ||
		get_fbgc_object_type(obj) == UNTIL )
		&& is_fbgc_PARA_EXPRESSION(GM_RIGHT)){
		change_top(get_fbgc_object_type(obj));
	}
	else if(get_fbgc_object_type(obj) == LPARA && GM_RIGHT == LPARA){
		change_top(LPARA_EMPTY);
	}	
	else if(get_fbgc_object_type(obj) == LPARA && is_fbgc_EXPRESSION(GM_RIGHT)){	
		change_top(LPARA_EXPRESSION);
	}
	else if(get_fbgc_object_type(obj) == LPARA && GM_RIGHT == ASSIGNMENT_EXPRESSION){
		change_top(LPARA_ASSIGNMENT_EXPRESSION);		
	}
	else if(get_fbgc_object_type(obj) == LPARA && (is_fbgc_EXPRESSION(GM_RIGHT) || GM_RIGHT == BALANCED_EXPR_LIST) ){
		change_top(LPARA_BALANCED_EXPR_LIST);;			
	}
	else if(get_fbgc_object_type(obj) == LPARA && GM_RIGHT == BALANCED_IDENTIFIER_LIST){
		change_top(LPARA_IDENTIFIER_LIST);
	}
	else {
		return 0;
	}

	return 1;





	//two number cannot be in the state side by side, top is the last pushed in the array
	
	/*if((is_fbgc_BINARY_OPERATOR(obj_type) || is_fbgc_UNARY_OPERATOR(obj_type)) && (right == EXPRESSION || right == NUMBER || right == IDENTIFIER)){
			top_fbgc_ll_object(gm_head)->type = EXPRESSION;
	}*/
	/*else if( (is_fbgc_BINARY_OPERATOR(top) || top == LPARA || top == UNKNOWN) && is_fbgc_UNARY_OPERATOR(obj_type)){
		if(obj_type == PLUS) obj->type = UPLUS;
		else if(obj_type == MINUS) obj->type = UMINUS;	
		top_fbgc_ll_object(gm_head)->type = obj_type;

	}
	else if(is_fbgc_BINARY_OPERATOR(obj_type)){
		if(is_fbgc_NUMBER(top) || top == EXPRESSION || top == IDENTIFIER){
			top_fbgc_ll_object(gm_head)->type = obj_type;
		}
		else {
			cprintf(111,"\nThe left side of %s is not valid operand!\n",object_name_array[obj->type]);
		}
	}
	else if(obj_type == LPARA && top != LPARA){
		gm_head = push_front_fbgc_ll_object(gm_head,new_fbgc_object(LPARA));
	}*/
}
