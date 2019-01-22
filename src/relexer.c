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


//#define DEBUG

static const token_table_struct lexer_table[1] = 
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
};
/*

*/

static const fbgc_lexer_rule_struct fbgc_lexer_rule_holder [] = 
{
	{SPACE,"!+!s"},
	{BIN,"0b 1|0!+"},
	{HEX,"0x !x!+"},
	{STRING,"' _!s!d!w!* '"},
	{COMPLEX,"!d!+ . !d!+ j"},
	{COMPLEX,"!d!+ j"}, 
	{DOUBLE,". !d!+"}, 
	{DOUBLE,"!d!+ . !d!+"}, 
	{INT,"!d!+"},
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"},		
	{OP,"+|-|**|^|*|/|<|>|=|=>|<=|,|;!+"},	
	{WORD,"_!w _!w!d!*"},
};

#define RULE_NUMBER sizeof(fbgc_lexer_rule_holder)/sizeof(fbgc_lexer_rule_struct)


void pretty_print_pointer(const char *buffer ,const char * ptr){
	int where = ptr-buffer;
	for (int i = 0; i < strlen(buffer); ++i){
		if(i== where) printf("[");
		cprintf(110,"%c",*(buffer+i));
		if(i== where) printf("]");
	}
	printf("\n");	
}




const char * rule_reader(rule_flag_struct * rfs,const char * rule_ptr){
	
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
			}
		}
		else{
			if(rfs->char_match_begin == NULL){
				rfs->char_match_begin = rule_ptr;
			}
			rfs->char_match_end++;
		}

		rule_ptr++;
		
		if(*rule_ptr == ' '){
			return ++rule_ptr;
		}
	}
	return rule_ptr;
}

uint8_t check_char(rule_flag_struct *rfs,const char ** buffer_ptr){

	/*#ifdef DEBUG
	cprintf(001,"Called <%s> \n",__FUNCTION__);
	#endif*/

	uint8_t check = 0;

	if(rfs->char_match_end>0){

		uint8_t i1 = 0,i2 = 0;

		while(i2 != rfs->char_match_end ){
			while(*(rfs->char_match_begin+i2) != LXR_META_OR && i2 != rfs->char_match_end){
				i2++;
			}
			if(!strncmp(*buffer_ptr,rfs->char_match_begin+i1 ,i2-i1)){
				*buffer_ptr += i2-i1;
				return 1;
			}
			if(*(rfs->char_match_begin+i2) == LXR_META_OR){
				i1 = ++i2;
			}	
		}
	}
	if(rfs->pattern_flag){
		check = (
				((rfs->pattern_flag & 0x01) && isdigit(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x02) && isalpha(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x04) && ispunct(*(*buffer_ptr))) || 
				((rfs->pattern_flag & 0x08) && isxdigit(*(*buffer_ptr)))||
				((rfs->pattern_flag & 0x10) && isprint(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x20) && isspace(*(*buffer_ptr))) ||
				((rfs->pattern_flag & 0x40) && *(*buffer_ptr) == ' ' ) ); 
		*buffer_ptr += check;
	}
	return check;
}

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

uint8_t regex_lexer(struct fbgc_object ** head_obj, const char *first_ptr){
	cprintf(111,"\n------------[relexer_begin : %s]-----------\n",first_ptr);
	rule_flag_struct rfs = {.char_match_begin = NULL,.char_match_end=0, .pattern_flag = 0, .metachar_flag = 0, .table_index = 0};

	uint8_t current_rule_index = 0;
	uint8_t current_token = fbgc_lexer_rule_holder->token;
	const char * rule_ptr = (fbgc_lexer_rule_holder)->rule;

	const char * mobile_ptr = first_ptr;

	uint8_t satisfied_rule_section = 0;
	uint8_t rule_section = 0;

	uint8_t check;

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll(*head_obj);

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

		check = check_char(&rfs,&mobile_ptr);
		#ifdef DEBUG
			cprintf(100,"Called check char, mobile_ptr:%s, check:%d\n",mobile_ptr,check);
		#endif		
		if(check ||  IS_METACHAR_STAR_OPEN(rfs.metachar_flag)) satisfied_rule_section = rule_section;
		//(check && *rule_ptr == '\0')
		if(!check || *mobile_ptr == '\0' || (check && *rule_ptr == '\0' &&!IS_METACHAR_PLUS_OPEN(rfs.metachar_flag) &&!IS_METACHAR_STAR_OPEN(rfs.metachar_flag))){
			#ifdef DEBUG
				cprintf(010,"-------------{CHECK = %d}---------------\n",check);
				cprintf(010,"Sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
			#endif
			//why rule_ptr != 0 WHYYYYYY
			if(satisfied_rule_section == rule_section && *rule_ptr != '\0'){
				#ifdef DEBUG
				cprintf(111,"-------------{MOVING IN THE RULE, NEXT}---------------\n");
				#endif
				rfs.metachar_flag = 0;
			}
			else{
				if(first_ptr != mobile_ptr && satisfied_rule_section == rule_section && *rule_ptr == '\0'){
					if(current_token != SPACE ){
					//#ifdef DEBUG
						char * tempstr = (char *) malloc(sizeof(char) * ((mobile_ptr - first_ptr)+1) );
						strncpy(tempstr,first_ptr,(mobile_ptr - first_ptr));
						tempstr[(mobile_ptr - first_ptr)] = '\0';
						//cprintf(111,"--------------[TOKEN IS FOUND]-------\n");
						cprintf(101,"['%s' : %s]",tempstr, object_name_array[current_token] );
						free(tempstr);
					
						head = push_back_fbgc_ll_object(head,new_fbgc_object_from_substr(first_ptr,mobile_ptr,current_token));
						
					//#endif
					}
						current_rule_index = 0;
						if(*mobile_ptr == '\0') break;
						//while(*mobile_ptr == ' ') mobile_ptr++; //eat space! 
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
						cprintf(111,"last rule but token couldn't find\n");
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

	cprintf(111,"\n---------[relexer_end]--------\n");
	return 1;
}


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

