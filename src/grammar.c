#include "fbgc.h"

//MATRIX_BEGIN
//Autogenerated by helper/grammar_gen2.py
uint8_t left_matrix[93][15] = {
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{2,0,0,2,2,2,0,0,0,2,0,2,2,2,0},
{2,0,0,2,2,2,0,0,0,2,0,2,2,2,0},
{2,0,0,2,2,2,0,0,0,2,0,2,2,2,0},
{2,0,0,2,2,2,0,0,0,2,0,2,2,2,0},
{2,0,0,2,2,2,0,0,0,2,0,2,2,2,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{3,0,0,3,3,3,0,0,0,3,0,3,3,3,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{3,0,0,3,3,3,0,0,0,3,0,3,3,3,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,15,0,0,0,0,0,0,0,0,0},
{15,0,0,0,0,0,0,0,15,0,0,0,0,0,0},
{15,0,0,0,0,0,0,0,15,0,0,0,0,0,0},
{15,0,0,0,0,0,0,0,15,0,0,0,0,0,0},
{15,0,0,0,0,0,0,0,15,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{15,0,0,0,0,0,0,0,15,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,9,9,0,0,0,0,0,9,9,9,0,0,0,0},
{12,0,12,12,12,12,0,0,0,12,0,12,12,12,12},
{0,9,9,0,0,0,0,0,9,0,9,9,0,0,0},
{13,0,13,13,13,13,0,0,0,13,0,13,13,13,0},
{0,9,9,0,0,0,0,0,9,0,9,9,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,10,10,0,0,0,0,0,10,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,1,0,0,0,0,0,1,0,1,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{-5,4,4,-5,-5,0,0,0,4,0,0,-5,-5,0,0},
{-5,4,4,-5,-5,0,0,0,4,0,0,-5,-5,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{-14,4,4,-14,-14,-14,0,0,4,0,0,-14,-14,0,0},
{0,4,4,0,0,0,0,0,4,0,0,0,0,0,0},
{5,0,0,5,5,0,0,0,0,0,0,0,0,0,0},
{5,0,0,5,5,0,0,0,0,0,0,0,0,0,0},
{5,0,0,5,5,0,0,0,0,0,0,0,0,0,0},
{5,0,0,5,5,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,6,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
uint8_t right_matrix[93][15] = {
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{9,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,0,0,9,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,11,11,0,0,0,0,0,11,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,9,9,0,0,0,0,0,9,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
//MATRIX_END


#ifndef OLD_GRAMMAR_MODEL

uint8_t gm_seek_left(struct fbgc_grammar * gm, struct fbgc_object * obj){

	#define gm_left (gm->top)
	#ifdef GRAMMAR_DEBUG
		cprintf(110,"Seek LEFT:[%s], OBJ:[%s]\n",gm_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)]);
	#endif

	int8_t new_left = left_matrix[get_fbgc_object_type(obj)][gm_left-1];
	
	if(new_left < 0){
		cprintf(100,"Special function call new_left :%s\n",gm_name_array[-new_left]);
		switch(get_fbgc_object_type(obj)){
			case PLUS:
			{
				obj->type = UPLUS;
				break;
			}
			case MINUS:
			{
				obj->type = UMINUS;
				break;
			}
			case PIPE:
			{
				obj->type = LEN;
				break;
			}
			default:
			{
				new_left = GM_ERROR;
				break;
			}	
		}
		new_left = -new_left;	
	}
	
	#ifdef GRAMMAR_DEBUG
	if(new_left == GM_ERROR){
		cprintf(100,"Unexpected grammar! LEFT:(%s), OBJ:(%s)\n",gm_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)]);
		
	}
	#endif	
	gm_left = new_left;

	return (new_left == GM_ERROR) ? _FBGC_SYNTAX_ERROR : _FBGC_NO_ERROR;

	#undef gm_left

	/*
	if(new_left == GM_ERROR){
		#ifdef GRAMMAR_DEBUG
		cprintf(100,"Unexpected grammar! LEFT:(%s), OBJ:(%s)\n",gm_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)]);
		#endif	
		return 0;
	}


	return 1;
	*/
}
uint8_t gm_seek_right(struct fbgc_grammar * gm, struct fbgc_object * obj){

	#define gm_right (gm->top)
	#ifdef GRAMMAR_DEBUG
	cprintf(110,"Seek RIGHT: OBJ:[%s] Right:[%s]\n",object_name_array[get_fbgc_object_type(obj)],gm_name_array[gm_right]);
	#endif	
	

	int8_t new_right = right_matrix[get_fbgc_object_type(obj)][gm_right-1];


	#ifdef GRAMMAR_DEBUG
	if(new_right == GM_ERROR){
		cprintf(100,"Unexpected grammar! OBJ:(%s) RIGHT:(%s)\n",object_name_array[get_fbgc_object_type(obj)],gm_name_array[gm_right]);
	}
	#endif
	gm_right = new_right;
	return (new_right == GM_ERROR) ? _FBGC_SYNTAX_ERROR : _FBGC_NO_ERROR;


	#undef gm_right

/*

	if(new_right == GM_ERROR){
		#ifdef GRAMMAR_DEBUG
		cprintf(100,"Unexpected grammar! OBJ:(%s) RIGHT:(%s)\n",object_name_array[get_fbgc_object_type(obj)],gm_name_array[gm_right]);
		#endif	
		return 0;
	}
	gm_right = new_right;

	return 1;*/
}


#else
uint8_t gm_seek_left(struct fbgc_grammar * gm, struct fbgc_object * obj){

	#define gm_left (gm->top)
	#ifdef GRAMMAR_DEBUG
		cprintf(110,"Seek LEFT:[%s], OBJ:[%s] flag{0x%X}\n",object_name_array[gm_left],object_name_array[get_fbgc_object_type(obj)],gm->flag);
	#endif

	if(gm == NULL || obj == NULL) return 0;

	if( (is_fbgc_ATOM(get_fbgc_object_type(obj))  ) 
		&& 
		(   is_fbgc_BINARY_OPERATOR(gm_left) || 
			is_fbgc_UNARY_OPERATOR(gm_left)  || 
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
			gm_left == UNBALANCED_EXPRESSION_LIST  || 
			gm_left == LPARA || gm_left == LBRACK || gm_left == LBRACE ||
			gm_left == IF_BEGIN || gm_left == ELIF_BEGIN || gm_left == WHILE_BEGIN ||
			gm_left == ELSE || gm_left == SEMICOLON || gm_left == COMMA ||
			gm_left == NEWLINE
		) 
	)
	{
		gm->top = get_fbgc_object_type(obj);
	}
	else if( get_fbgc_object_type(obj) == IDENTIFIER
		&& 
		( is_fbgc_BINARY_OPERATOR(gm_left) || 
			is_fbgc_UNARY_OPERATOR(gm_left)  || 
			gm_left == UNBALANCED_EXPRESSION_LIST  || 
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
			gm_left == LBRACK ||  gm_left == LPARA || gm_left == LBRACE ||
			gm_left == IF_BEGIN || gm_left == ELIF_BEGIN || gm_left == WHILE_BEGIN ||
			gm_left == ELSE || gm_left == SEMICOLON || gm_left == DOT || gm_left == COMMA ||
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
	else if(get_fbgc_object_type(obj) == LBRACK &&
			(
			gm_left == IDENTIFIER ||
			is_fbgc_BINARY_OPERATOR(gm_left) ||
			is_fbgc_UNARY_OPERATOR(gm_left)  ||
			is_fbgc_ASSIGNMENT_OPERATOR(gm_left) ||
		 	gm_left == UNBALANCED_EXPRESSION_LIST || gm_left == LBRACK || gm_left == SEMICOLON
		 	|| gm_left == NEWLINE) )
	{
		gm->top = LBRACK;		
	}
	else if( ( is_fbgc_UNARY_OPERATOR(get_fbgc_object_type(obj)) || get_fbgc_object_type(obj) == PLUS || get_fbgc_object_type(obj) == MINUS) 
			&& 
			(is_fbgc_BINARY_OPERATOR(gm_left) || is_fbgc_ASSIGNMENT_OPERATOR(gm_left)|| gm_left == LPARA || gm_left == NEWLINE) )
	{
		if(get_fbgc_object_type(obj) == PLUS) obj->type = UPLUS;
		else if(get_fbgc_object_type(obj) == MINUS) obj->type = UMINUS;
			
		gm->top = get_fbgc_object_type(obj);		
	}
	else if(get_fbgc_object_type(obj) == PIPE && gm_left == LEN){
		gm->top = LEN;
	}
	else if(get_fbgc_object_type(obj) == PIPE && (is_fbgc_EXPRESSION(gm_left) == 0 || is_fbgc_OPERATOR(gm_left) ) ){
		gm->top = obj->type = LEN;
	}
	else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_left)){
		gm->top = get_fbgc_object_type(obj);
	}
	//BURADA NIYE BUILD TUPLE KOYMUSUZ
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
	else if(get_fbgc_object_type(obj) == NEWLINE && (gm_left == BALANCED_EXPRESSION_LIST)){
		gm->top = BUILD_TUPLE;
	}
	else if(get_fbgc_object_type(obj) == NEWLINE && (is_fbgc_EXPRESSION(gm_left) || gm_left == ASSIGNMENT_EXPRESSION || gm_left == NEWLINE || gm_left == FOR_BEGIN || gm_left == IF_BEGIN || 
		gm_left == ELIF_BEGIN)  )
	{
		gm->top = EXPRESSION;
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
	else if(get_fbgc_object_type(obj) == LEN && (is_fbgc_EXPRESSION(gm_right) || gm_right == PIPE) ){
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
		gm->top = BALANCED_EXPRESSION_LIST;
		//cprintf(111,"burada=!\n");
	}
	else if(is_fbgc_OPERATOR(get_fbgc_object_type(obj)) && is_fbgc_EXPRESSION(gm_right)){
		gm->top = EXPRESSION;
	}
	//&& (gm_right == MONUPLE || gm_right == BUILD_TUPLE) 
	else if( get_fbgc_object_type(obj) == IF ){
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


#endif
