#include "fbgc.h"
/*
	d: digit [0-9]
	w: letter[a-zA-Z]
	o : operator
	h: hexadecimal [0-9a-fA-F]
	  
*/
/*

			d	.	j	\E \+\-*
Begin 	0	1	2	0	0
Int 	1	1	2	4	3
Double 	2	2	0	4	3
EXP_DB  3	3	0	4	0
Complex 4	0	0	0	0

*/



#ifdef LEXER_DEBUG
const char * lexer_token_list_as_strings[]={
 LEXER_TOK_AS_STRINGS()
};
#endif

#ifdef LEXER_DETAILED_DEBUG
#define DEBUG
#endif

 /*
const token_table_struct lexer_table[1] = 
{
	{
		.input_size = 4,
		.token_size = 4,
		.inputs = (const char*[]){"!d",".","j","E !+!-!*"},
		.states = (const uint8_t[])
		{ 1,2,5,5, //begin  
		  1,2,4,3, //int
		  2,0,4,3, //double
		  3,0,4,0, //exponential double
		  0,0,0,0, //complex
		},
		.tokens = (const fbgc_token[]){INT,DOUBLE,DOUBLE,COMPLEX}
	}	
};	*/

//1.2-E2

/*
	{SPACE,"! |\t!+"},
	{INT2,"0b 1|0!+"},
	{INT16,"0x !x!+"},
	{STRING,"' _|\\'!s!d!w!o!* '"},
	//{DOUBLE,"!d!+ . !d!+ E +|-!* !d!+"}, 	//just experimental, it works..
	{COMPLEX,"!d!+ . !d!+ j"},
	{COMPLEX,"!d!+ j"}, 
	{DOUBLE,". !d!+"}, 
	{DOUBLE,"!d!+ . !d!+"}, 
	{INT,"!d!+"},
*/

/*
	[Some important points about rules]

	1-) 
		White space without escape metachar means you wanna catch a sequence of strings
		For example, if you wanna catch fbgc word you can write either fbgc or f b g c  
		But if you wanna catch a digit after word you need to put space
		Rule: "fbgc !d" will catch enterances  like fbgc1 fbgc0 fbgc8 etc..
		If you wanna catch at least one digit try this "fbgc !d!+" or "fbgc !+!d" 
	2-)
		Do not put OR(|) operator if a special character and string are tried to be match
		For instance, consider following rule for white spaces and tabs
		Rule: "!+! \t"
		Here there is already an OR operator between white space and tab
		!+ means we will accept following rule if number > 1
		! means white space is a metacharacter so we need to put escape character
		\t is catching tabs as chars
		If you wanna catch, lets say white space and space word consider following
		Rule: "!+! space" 
	3-) 
		Each time put escape sequences and strings seperately
		Assume rule is catching digits and numbers as strings
		"!+!dzero|one|two" etc.. there is already an | operator between !d and zero 
		This rule catches digits and zero,one like entered strings as numbers 


 
*/
const fbgc_lexer_rule_struct fbgc_lexer_rule_holder [] = 
{
	{LEXER_TOK_NEWLINE,"\n"},
	{LEXER_TOK_SPACE,"!+! \t"},
	{LEXER_TOK_BASE2_INT,"0b 1|0!+"},
	{LEXER_TOK_BASE16_INT,"0x !x!+"},
	{LEXER_TOK_STRING,"' _|\\'!s!d!w!o!* '"},
	{LEXER_TOK_DOUBLE,"!d!+ .!* !d!* E +|-!* !d!+"}, 
	{LEXER_TOK_DOUBLE,". !d!+"}, 
	{LEXER_TOK_DOUBLE,"!d!+ . !d!+"}, 	
	{LEXER_TOK_BASE10_INT,"!d!+"},
	{LEXER_TOK_PARA,"(|)|[|]|{|}"},
	{LEXER_TOK_KEYWORDS,"end|fun|if|elif|else|for|while|break|cont|return|load|true|false"},		
	{LEXER_TOK_NAME,"_!w _!w!d!*"},		
	{LEXER_TOK_OP,"...|**|++|--|//|<=|>=|==|!=|+=|-=|*=|/=|:=|=>|>>|<<|->|<-|~>|<~|::|=|;|,|.|:|^|~|%|!|<|>|||&|/|*|-|+"},
};
	





#ifdef DEBUG
 
void pretty_print_pointer(const char *buffer ,const char * ptr){
	unsigned int where = ptr-buffer;
	for (unsigned int i = 0; i < strlen(buffer); ++i){
		if(i== where) printf("[");
		cprintf(110,"%c",*(buffer+i));
		if(i== where) printf("]");
	}
	printf("\n");	
}

#endif

char * fbgc_getline_from_file(char * s, int n, FILE *fp){
   	int c;
    char* cs = s;
    while(--n > 0 && (c = getc(fp)) != EOF){
    // put the input char into the current pointer position, then increment it
    // if a newline entered, break
    	switch((*cs++ = c)){
    		case '\n': goto end_of_getline;
    		case '\\': 
    			switch( c = getc(fp)){
					case 'a': *(cs-1) = '\a'; break;
					case 'b': *(cs-1) = '\b'; break;
					case 'e': *(cs-1) = '\e'; break;
					case 'f': *(cs-1) = '\f'; break;
					case 'n': *(cs-1) = '\n'; break;
					case 'r': *(cs-1) = '\r'; break;
					case 't': *(cs-1) = '\t'; break;
					case 'v': *(cs-1) = '\v'; break;
					//case '\'': *(cs) = '\''; break;
					case '\\': *(cs-1) = '\\'; break;     				
    				case EOF: 
    					*(cs++) = '\n';
    					goto end_of_getline;	
    				default:
    					*cs++ = c;
    						
    			} 
    		break;	
    	}          
    }
    end_of_getline:
    *cs = (c == EOF) ? '\n':'\0'; //handle this, i added because of comment understanding...
    *(cs+1)='\0';
    return (c == EOF && cs == s) ? NULL : s;
}



static const char * rule_reader(rule_flag_struct * rfs,const char * rule_ptr){
	
	#ifdef DEBUG
		cprintf(001,"inside rule_reader , rule_ptr:%s\n",rule_ptr);
	#endif

	rfs->metachar_flag = rfs->pattern_flag = rfs->char_match_end = 0;
	rfs->char_match_begin = NULL;

	while(*rule_ptr != '\0'){
		if(*rule_ptr == LXR_META_ESCAPE){
			rule_ptr++; //eat escape char

			if( IS_METACHAR(*rule_ptr) ){
				rfs->metachar_flag |= SET_METACHARACTER_FLAG_FROM_RULE(*rule_ptr);
				//if(rfs->metachar_flag == 0) cprintf(100,"Undefined metacharacter at rule section!\n");

				if(IS_METACHAR_TABLE_OPEN(rfs->metachar_flag)){
					//first assume just one char number is entered
					rule_ptr++;
					rfs->table_index = (*rule_ptr++ - '0');
					return rule_ptr;
					//here we are assuming that there is no rule other than this!!!!
				}					
			}
			else if( IS_SPECIAL_CHAR(tolower(*rule_ptr)) ){
				rfs->pattern_flag |= SET_PATTERN_FLAG_FROM_RULE(tolower(*rule_ptr));
				if(rfs->pattern_flag == 0)
					printf("Undefined rule rfs->rule_ptr c:%c d:%d\n",*rule_ptr,*rule_ptr);
				else if(isupper(*rule_ptr)){
					rfs->pattern_flag = ~rfs->pattern_flag;
				}
			}else{
				//cprintf(100,"\n\t\tUndefined metachar at %c\n\n",*rule_ptr);
				//move anyway to catch ! only 
				//cprintf(111,"oky i do it\n");
				rfs->char_match_end++;
				rule_ptr--;
			}
		}
		else{
			if(rfs->char_match_begin == NULL){
				rfs->char_match_begin = rule_ptr;
			}
			//cprintf(111,"increase for %c\t%d\n",*rule_ptr,rfs->char_match_end);
			rfs->char_match_end++;
		}

		rule_ptr++;
		
		if(*rule_ptr == ' '){
			return ++rule_ptr;
		}
	}
	return rule_ptr;
}

static uint8_t check_char(rule_flag_struct *rfs,char ** buffer_ptr){

	#ifdef DEBUG
	cprintf(001,"Called <%s> \n",__FUNCTION__);
	cprintf(100,"RFS pattern_flag 0x%x buffer_ptr[%s]\n",rfs->pattern_flag,*buffer_ptr);
	#endif

	uint8_t check = 0;
	uint8_t rule_meta_or_section = 0;

	uint8_t i1 = 0,i2 = 0;

	while(i1 < rfs->char_match_end ){

		//cprintf(111,"stuck!\n");
		//<= yanlış!
		while(i2 < rfs->char_match_end){
			i2++;
			if(*(rfs->char_match_begin+i2) == LXR_META_OR) break;
		}
		if(i2>i1 && !strncmp(*buffer_ptr,rfs->char_match_begin+i1 ,i2-i1)){
			*buffer_ptr += i2-i1;
			//cprintf(001,"\t\t\t>>>>>>>>>Catched at %d\n",rule_meta_or_section);
			return ++rule_meta_or_section; //if it's zero we have return 1 so check must be satisfied
		}
		if(*(rfs->char_match_begin+i2) == LXR_META_OR || *(rfs->char_match_begin+i2) == '\0' ){
			/*cprintf(100,"We hit lxr_meta_or at %d ",i2);
			cprintf(010,"Not found :");
			for(int qq = i1; qq<i2; qq++){
				cprintf(110,"%c",*(rfs->char_match_begin+qq));
			}*/
			//cprintf(010,"\n");
			i1 = ++i2;

			//cprintf(001,"i1=%d i2=%d\n",i1,i2);
			rule_meta_or_section++;
		}
		else{
			i1 = i2;
		}	
	}

	if(rfs->pattern_flag){
		check = (
				((rfs->pattern_flag & 0x01) && isdigit(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x02) && isalpha(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x04) && (ispunct(*(*buffer_ptr))) && (*(*buffer_ptr)) != '\'') || 
				((rfs->pattern_flag & 0x08) && isxdigit(*(*buffer_ptr)))||
				((rfs->pattern_flag & 0x10) && isprint(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x20) && isspace(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x40) && *(*buffer_ptr) == ' ' ) ); 
		*buffer_ptr += check;
	}
	return check;
/*
#define SET_PATTERN_FLAG_FROM_RULE(x)({\
x == 'd' ? 0x01 :\
x == 'w' ? 0x02 :\
x == 'o' ? 0x04 :\
x == 'x' ? 0x08 :\
x == '.' ? 0x10 :\
x == 's' ? 0x20 :\
x == ' ' ? 0x40 :0;})
*/

}


uint8_t regex_lexer(struct fbgc_object ** field_obj,char * first_ptr){
	#ifdef LEXER_DEBUG
	cprintf(111,"==========LEXER==========\nInput string : \n");
	cprintf(101,"%s",first_ptr);
	cprintf(111,"Tokenizing each part..\n");
	
	#endif


	rule_flag_struct rfs = {.char_match_begin = NULL,.char_match_end=0, .pattern_flag = 0, .metachar_flag = 0, .table_index = 0};
	if(*first_ptr == '\0') return 0;

	uint8_t current_rule_index = 0;
	uint8_t current_token = fbgc_lexer_rule_holder->token;
	const char * rule_ptr = (fbgc_lexer_rule_holder)->rule;

	char * mobile_ptr = first_ptr;

	uint8_t satisfied_rule_section = 0;
	uint8_t rule_section = 0;

	uint8_t check;

	for(;;){
		#ifdef DEBUG
			cprintf(110,"buffer:");
			pretty_print_pointer(first_ptr,mobile_ptr);
		#endif

		if(!IS_METACHAR_STAR_OPEN(rfs.metachar_flag) && !IS_METACHAR_PLUS_OPEN(rfs.metachar_flag) && *rule_ptr != '\0'){
			
			//for(;;){
				#ifdef DEBUG
					cprintf(100,"Call rule_reader rule_ptr:%s\n",rule_ptr);
				#endif
				rule_ptr = rule_reader(&rfs,rule_ptr);
				rule_section++;

				#ifdef DEBUG
					cprintf(001,"After rule reader call,sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
				#endif
			//}
		}

		//When reading from file \n is actually becomes a c string \\n, so we have to convert them to match sequence
		//Later try to implement this feature in regex module as catch_and_convert
		//we use const pointer but for this they are not constant, we have fetch the input somehow converted ones 
		/*if(*mobile_ptr == '\\')
		{
			if(*(mobile_ptr+1)  == '\'') memmove (mobile_ptr,mobile_ptr+1,strlen(mobile_ptr)); 

		}	*/
			//convert_to_escape_sequence(&mobile_ptr);
		//=====================================

		check = check_char(&rfs,&mobile_ptr);
		#ifdef DEBUG
			cprintf(100,"Called check char, mobile_ptr:<%s>, check:%d\n",mobile_ptr,check);
		#endif		
		if(check ||  IS_METACHAR_STAR_OPEN(rfs.metachar_flag)) satisfied_rule_section = rule_section;
		//(check && *rule_ptr == '\0')
		if(!check || *mobile_ptr == '\0' || (check && *rule_ptr == '\0' &&!IS_METACHAR_PLUS_OPEN(rfs.metachar_flag) &&!IS_METACHAR_STAR_OPEN(rfs.metachar_flag))){
			#ifdef DEBUG
				cprintf(010,"-------------{CHECK = %d}---------------\n",check);
				cprintf(010,"Stsfied rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
			#endif
			//why rule_ptr != 0 xxxxxxxxxx
			if(satisfied_rule_section == rule_section && *rule_ptr != '\0'){
				#ifdef DEBUG
				cprintf(111,"-------------{MOVING IN THE RULE, NEXT}---------------\n");
				#endif
				rfs.metachar_flag = 0;
			}
			else{
				if(first_ptr != mobile_ptr && satisfied_rule_section == rule_section && *rule_ptr == '\0'){
					if(current_token != LEXER_TOK_SPACE){
					//#ifdef DEBUG
						#ifdef LEXER_DEBUG
							char * tempstr = (char *) malloc(sizeof(char) * ((mobile_ptr - first_ptr)+1) );
							strncpy(tempstr,first_ptr,(mobile_ptr - first_ptr));
							tempstr[(mobile_ptr - first_ptr)] = '\0';
							cprintf(101,"['%s' : %s]\n",tempstr, lexer_token_list_as_strings[current_token-1] );
							free(tempstr);
						#endif
					

						(cast_fbgc_object_as_field(*field_obj)->head) = 
							push_back_fbgc_ll_object(
								(cast_fbgc_object_as_field(*field_obj)->head),
								 //new_fbgc_object_from_substr(*field_obj,first_ptr,mobile_ptr,current_token)
									tokenize_substr(first_ptr,mobile_ptr,current_token,check-1)
								);

						//if(current_token == WORD) print_fbgc_symbol_table((cast_fbgc_object_as_field(*field_obj)->global_table));


					//#endif
					}
						current_rule_index = 0;
						if(*mobile_ptr == '\0') break;
						first_ptr = mobile_ptr;

				}
				else if(current_rule_index < (RULE_NUMBER-1) && satisfied_rule_section != rule_section){
					#ifdef DEBUG
					cprintf(111,"-------------{CHANGING THE RULE}---------------\n");
					#endif	 
					current_rule_index++;
					mobile_ptr = first_ptr;
				}
				else {
					if(current_rule_index == RULE_NUMBER-1){
						cprintf(111,"last rule but token couldn't find RETURN 0\n");
						//first_ptr = ++mobile_ptr;
						return 0;
						
					}
					else {
						cprintf(111,"WHAT!\n");
						break;
					}
				}

				rule_ptr = (fbgc_lexer_rule_holder+current_rule_index)->rule;
				current_token = (fbgc_lexer_rule_holder+current_rule_index)->token;
				satisfied_rule_section = rule_section = 0;
				
				rfs.metachar_flag = 0;

			}
		}
	}

	#ifdef LEXER_DEBUG
		cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	return 1;
}


struct
fbgc_object * tokenize_substr(const char *str1, const char*str2, lexer_token token, uint8_t where){
	struct fbgc_object *obj = NULL;

	switch(token){
		case LEXER_TOK_NEWLINE:
		{
			return new_fbgc_object(NEWLINE);
		}
		case LEXER_TOK_BASE2_INT:
		{
			return new_fbgc_int_object_from_substr(str1+2,str2,2); //eat 0b
		} 
		case LEXER_TOK_BASE10_INT:
		{
			return new_fbgc_int_object_from_substr(str1,str2,10);
		} 
		case LEXER_TOK_BASE16_INT:
		{
			return new_fbgc_int_object_from_substr(str1+2,str2,16); //eat 0x
		} 
		case LEXER_TOK_DOUBLE:
		{
			return new_fbgc_double_object_from_substr(str1,str2); 
		}
		case LEXER_TOK_STRING:
		{
			return new_fbgc_str_object_from_substr(str1+1,str2-1);
		}
		case LEXER_TOK_OP:
		{	
			//cprintf(111,"\n\nCatched %s\n\n",object_name_array[THREE_DOT+where]);
			fbgc_token opcode = THREE_DOT+where;
			if(is_fbgc_ASSIGNMENT_OPERATOR(opcode))
			{
				return  derive_from_new_int_object(opcode,-1);
			}
			return new_fbgc_object(THREE_DOT+where);
		}					
		case LEXER_TOK_PARA:
		{
			/*
				(|)|[|]|{|}
				LPARA  : 1 - 0 
				RPARA  : 3 - 1
				LBRACK : 5 - 2
				RBRACK : 7 - 3
				LBRACE : 9 - 4
				RBRACE : 11 - 5
				"where" gives the location after LPARA, if its 3 it means RBRACK is found because
				(|)|[|]|{|} , 3rd section is starting from 0 RBRACK 
			*/
			return new_fbgc_object(LPARA+where);
		}
		case LEXER_TOK_KEYWORDS:
		{	
			fbgc_token kw_tok = END+where;

			switch(kw_tok){
				case IF:
				case ELIF:
				case WHILE:
				case BREAK:
				case CONT:
				case FUN_MAKE:
				return new_fbgc_jumper_object(kw_tok);
				default: return new_fbgc_object(END + where);
			}

		}
		case LEXER_TOK_NAME:
		{	
			return new_fbgc_symbol_from_substr(str1,str2);
			//return obj;//new_fbgc_object(NAME);
		}
		default: 
			cprintf(100,"Undefined object creation in new object creation! returning NULL\n");
			return NULL;
		break;
	}


	/*switch(token){

		case LPARA:			
		case RPARA:
		case LBRACE:
		case RBRACE:
		case LBRACK:
		case RBRACK:
		case OP:{	
					fbgc_token opcode = get_operator_code_from_substr(str1,str2);
					
					if(opcode == UNKNOWN){
						cprintf(100,"Undefined operator!\n");
						obj = NULL;
					}else if(is_fbgc_ASSIGNMENT_OPERATOR(opcode)){
						obj = new_fbgc_int_object(-1);
						obj->type = opcode;
					}
					else {
						obj = new_fbgc_object(opcode);
					}


					break;
		} 
		case WORD:
					token = get_reserved_word_code_from_substr(str1,str2);
					if(token == UNKNOWN){
						//cprintf(111,"\nThis is not a keyword! \n"); it must be something id or function or class etc.
						//is this substring refers to a c function ?

						obj = new_fbgc_symbol_from_substr(field_obj,str1,str2);
						//handle symbol table!
					}
					else {
						//this is a keyword, maybe if,while,elif etc.

						if(token == IF || token == ELIF || token == WHILE || token == BREAK || token == CONT || token == FOR || token == FUN_MAKE){
							obj = new_fbgc_jumper_object(token);
						}
						else 
							obj  = new_fbgc_object(token);
						 
					}
		break;

		
	}*/

    return obj;
}

/*
uint8_t match(match_where *mw, const char * inc_rule_ptr, const char * buffer){

	rule_flag_struct rfs = {.char_match_begin = NULL,.char_match_end=0, .pattern_flag = 0, .metachar_flag = 0, .table_index = 0};

	const char * first_ptr = buffer;
	const char * mobile_ptr = buffer;
	const char * rule_ptr = inc_rule_ptr;

	uint8_t satisfied_rule_section = 0;
	uint8_t rule_section = 0;

	uint8_t check;

	for(;*mobile_ptr != '\0';){
		#ifdef DEBUG
			cprintf(110,"buffer:");
			pretty_print_pointer(first_ptr,mobile_ptr);
		#endif

		if(satisfied_rule_section == rule_section && !IS_METACHAR_STAR_OPEN(rfs.metachar_flag) && !IS_METACHAR_PLUS_OPEN(rfs.metachar_flag) && *rule_ptr != '\0'){
			
			//for(;;){
				#ifdef DEBUG
					cprintf(100,"Call rule_reader rule_ptr:%s\n",rule_ptr);
				#endif
				rule_ptr = rule_reader(&rfs,rule_ptr);
				rule_section++;

				#ifdef DEBUG
					cprintf(100,"Sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
				#endif
			//}
		}

		check = check_char(&rfs,&mobile_ptr);
		#ifdef DEBUG
			cprintf(100,"Called check char, mobile_ptr:%s, check:%d\n",mobile_ptr,check);
		#endif		
		if(check ||  IS_METACHAR_STAR_OPEN(rfs.metachar_flag)) satisfied_rule_section = rule_section;

		if(!check || *mobile_ptr == '\0'){
			#ifdef DEBUG
				cprintf(010,"-------------{CHECK = 0}---------------\n");
				cprintf(010,"Sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
			#endif
			if(first_ptr != mobile_ptr && satisfied_rule_section == rule_section && *rule_ptr == '\0'){
					mw->start = first_ptr - buffer;
					mw->length = mobile_ptr - first_ptr; 
					return 1;
			}
			first_ptr = ++mobile_ptr;
			rule_ptr = inc_rule_ptr;
			satisfied_rule_section = rule_section = 0;
			rfs.metachar_flag = 0;
		}
	}
	return 0;
}
*/


/*
void read_rule_table(rule_arrange_struct *ras){

	uint8_t const input_size = (lexer_table+ras->table_index)->input_size;
	uint8_t const token_size = (lexer_table+ras->table_index)->token_size;
	const char * hold_rule_ptr = ras->rule_ptr;
	const char * hold_mobile_ptr = ras->mobile_ptr;
	
	fbgc_bool match_occured = 0;

	int koruyucu = 0;

	while(*(ras->mobile_ptr) != '\0' ) {
		for(uint8_t i = 0; i<input_size; i++){
			ras->rule_section = 0;
			ras->rule_ptr = (lexer_table+ras->table_index)->inputs[i]; // okunacak yeni kurallar tablodan alınır

			while(*(ras->rule_ptr) != '\0'){
				//alınan ilk kural için flagleri belirleyelim
				rule_reader(ras);
				//kuralı verilen char için okuyalım
				SET_METACHAR_TABLE_FLAG_CLOSE(ras->metachar_flag);
				check_char(ras);
				SET_METACHAR_TABLE_FLAG_OPEN(ras->metachar_flag);

				if(ras->check && *(ras->rule_ptr) != '\0') {
					ras->mobile_ptr++;
					if(*(ras->rule_ptr) == ' ') ras->rule_ptr++;
				}
				else {  
					if( !ras->check  && IS_METACHAR_STAR_OPEN(ras->metachar_flag)) {
						ras->check = 1;
					}
					else if((lexer_table+ras->table_index)->states[i + (token_size * ras->table_state)] > token_size){
						//cprintf(011,"there must be an error\n");
						ras->check = 0;
					}
					break;
				}
				koruyucu++;
				if(koruyucu >= 100) break;
			}

			if(ras->check){
				match_occured = 1;
				//check 1 ise tabloda yeni bir state'e gitmemiz gerekiyor.
				uint8_t dummy_state = (lexer_table+ras->table_index)->states[i + (token_size * ras->table_state)];
				
				if(dummy_state == 0){
					//state returned to beginning, do not change the state
					
					ras->current_token = (lexer_table+ras->table_index)->tokens[ras->table_state-1];

					ras->check = 0;
					
					SET_METACHAR_TABLE_FLAG_OPEN(ras->metachar_flag);
				}
				else{
					//succesfully moving
					
					ras->table_state = (lexer_table+ras->table_index)->states[i + (token_size * ras->table_state)];
					ras->current_token = (lexer_table+ras->table_index)->tokens[ras->table_state-1];
					SET_METACHAR_TABLE_FLAG_OPEN(ras->metachar_flag);
					
				}
				
				break;
			}
			
		}
		if(ras->check) ras->mobile_ptr++;
		else{

			break;
		}
	}
	ras->rule_ptr = hold_rule_ptr;

	if(match_occured == 0){
		// no input is satisfied this char
		
		SET_METACHAR_TABLE_FLAG_CLOSE(ras->metachar_flag);
	}
}*/



/*
uint8_t match_and_replace(const char * inc_rule_ptr,char buffer[],const char *replace_str){

	rule_flag_struct rfs = {.char_match_begin = NULL,.char_match_end=0, .pattern_flag = 0, .metachar_flag = 0, .table_index = 0};

	const char * first_ptr = buffer;
	const char * mobile_ptr = buffer;
	const char * rule_ptr = inc_rule_ptr;

	uint8_t satisfied_rule_section = 0;
	uint8_t rule_section = 0;

	uint8_t check;

	for(;*mobile_ptr != '\0';){
		#ifdef DEBUG
			cprintf(110,"buffer:");
			pretty_print_pointer(first_ptr,mobile_ptr);
		#endif

		if(satisfied_rule_section == rule_section && !IS_METACHAR_STAR_OPEN(rfs.metachar_flag) && !IS_METACHAR_PLUS_OPEN(rfs.metachar_flag) && *rule_ptr != '\0'){
			
			//for(;;){
				#ifdef DEBUG
					cprintf(100,"Call rule_reader rule_ptr:%s\n",rule_ptr);
				#endif
				rule_ptr = rule_reader(&rfs,rule_ptr);
				rule_section++;

				#ifdef DEBUG
					cprintf(100,"Sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
				#endif
			//}
		}

		check = check_char(&rfs,&mobile_ptr);
		#ifdef DEBUG
			cprintf(100,"Called check char, mobile_ptr:%s, check:%d\n",mobile_ptr,check);
		#endif		
		if(check ||  IS_METACHAR_STAR_OPEN(rfs.metachar_flag)) satisfied_rule_section = rule_section;

		if(!check || *mobile_ptr == '\0'){
			#ifdef DEBUG
				cprintf(010,"-------------{CHECK = 0}---------------\n");
				cprintf(010,"Sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
			#endif
			if(first_ptr != mobile_ptr && satisfied_rule_section == rule_section && *rule_ptr == '\0'){

				    memmove ((void *)first_ptr+strlen(replace_str), mobile_ptr, strlen(mobile_ptr));
				    memmove ((void *)first_ptr, replace_str, strlen(replace_str));
					
			}
			first_ptr = ++mobile_ptr;
			rule_ptr = inc_rule_ptr;
			satisfied_rule_section = rule_section = 0;
			rfs.metachar_flag = 0;
		}
	}
	return 1;
}

*/