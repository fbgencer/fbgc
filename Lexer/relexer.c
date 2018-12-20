#include "relexer.h"
#include <stdarg.h>

void cprintf(char color,const char *format, ...) {


#define ANSI_COLOR_BLACK   "\x1b[30m" //000
#define ANSI_COLOR_RED     "\x1b[31m" //100
#define ANSI_COLOR_GREEN   "\x1b[32m" //010
#define ANSI_COLOR_YELLOW  "\033[1;33m" //011
#define ANSI_COLOR_BLUE    "\x1b[34m" //001
#define ANSI_COLOR_MAGENTA "\x1b[35m" //101
#define ANSI_COLOR_CYAN    "\x1b[36m" //011
#define ANSI_COLOR_WHITE   "\033[1;37m" //111
#define ANSI_COLOR_RESET   "\x1b[0m" //0

	va_list args;
	  
	va_start(args, format);
	if(color == 100) printf(ANSI_COLOR_RED); 
	else if(color == 010) printf(ANSI_COLOR_GREEN);
	else if(color == 001) printf(ANSI_COLOR_BLUE);
	else if(color == 110) printf(ANSI_COLOR_YELLOW);
	else if(color == 101) printf(ANSI_COLOR_MAGENTA);
	else if(color == 011) printf(ANSI_COLOR_CYAN);
	else if(color == 000) printf(ANSI_COLOR_BLACK);
	else if(color == 111) printf(ANSI_COLOR_WHITE);
	
	vprintf(format, args); printf("\033[0m");
	va_end(args);
}

void pretty_print_pointer(const char *buffer ,const char * ptr){
	long int where = ptr-buffer;
	//printf("%s |ptr-> ",buffer);
	for (int i = 0; i < strlen(buffer); ++i){
		if(i== where) printf("[");
		cprintf(110,"%c",*(buffer+i));
		if(i== where) printf("]");
	}
	printf("\n");	
}

//#define DEBUG 


typedef struct{
	const char * char_match_begin;
	uint8_t char_match_e;
	fbgc_bool pattern_flag;
	fbgc_bool metachar_flag;
	fbgc_bool check;
	
	fbgc_token current_token; 

	uint8_t rule_section;
	const char *rule_ptr, *mobile_ptr;

	uint8_t table_index;
	uint8_t table_state;
	
}rule_arrange_struct;


typedef struct{
	const char * char_match_begin;
	uint8_t char_match_end;
	uint8_t pattern_flag;
	uint8_t metachar_flag;
	uint8_t table_index;
}rule_flag_struct;


void rule_reader(rule_arrange_struct * ras);
void read_rule_table(rule_arrange_struct * ras);
void check_char(rule_arrange_struct * ras);


const char * rule_reader_v2(rule_flag_struct * rfs,const char * rule_ptr){
	
	#ifdef DEBUG
		cprintf(001,"rule_reader , rule_ptr:%s\n",rule_ptr);
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

uint8_t check_char_v2(rule_flag_struct *rfs,const char ** buffer_ptr){

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


void rule_reader(rule_arrange_struct * ras){
	
	ras->metachar_flag = ras->pattern_flag = ras->char_match_e = 0;
	ras->char_match_begin = NULL;

	if(*(ras->rule_ptr) !='\0') ras->rule_section++;

	while(*ras->rule_ptr != '\0'){
		if(*(ras->rule_ptr) == LXR_META_ESCAPE){
			
			ras->rule_ptr++; 

			if( IS_METACHAR(*(ras->rule_ptr))){
				ras->metachar_flag |= SET_METACHARACTER_FLAG_FROM_RULE(*(ras->rule_ptr));

				if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
					//first assume just one char number
					(ras->rule_ptr)++;
					ras->table_index = (*((ras->rule_ptr++)) - '0');
					ras->table_state = 0;
				}					
			}
			else if( IS_SPECIAL_CHAR(*(ras->rule_ptr)) ){
				ras->pattern_flag |= SET_PATTERN_FLAG_FROM_RULE(*(ras->rule_ptr));
				if(ras->pattern_flag == 0)
					printf("Undefined rule ras->rule_ptr c:%c d:%d\n",*(ras->rule_ptr),*(ras->rule_ptr));
			}
		}
		else{
			if(ras->char_match_begin == NULL){
				ras->char_match_begin = ras->rule_ptr;
			}
			ras->char_match_e++;
		}

		if(*(ras->rule_ptr) != '\0'){
			ras->rule_ptr++;
		}

		if(*(ras->rule_ptr) == ' '){
			ras->rule_ptr++;
			break;
		}
	}
}

void check_char(rule_arrange_struct * ras){

	#ifdef DEBUG
	printf("<%s> pattern_flag : [0x%x]\n",__FUNCTION__,ras->pattern_flag);
	#endif
	ras->check = 0;

	if(!IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
		//ras->check = check_character_with_pattern(*(ras->mobile_ptr),ras->pattern_flag);
		ras->check = (
			((ras->pattern_flag & 0x01) && isdigit(*(ras->mobile_ptr))) ||
			((ras->pattern_flag & 0x02) && isalpha(*(ras->mobile_ptr))) ||
			((ras->pattern_flag & 0x04) && ispunct(*(ras->mobile_ptr))) ||
			((ras->pattern_flag & 0x04) && ispunct(*(ras->mobile_ptr))) ||
			((ras->pattern_flag & 0x08) && isxdigit(*(ras->mobile_ptr)))||
			((ras->pattern_flag & 0x10) && isprint(*(ras->mobile_ptr))) ||
			((ras->pattern_flag & 0x20) && isspace(*(ras->mobile_ptr))) ||
			((ras->pattern_flag & 0x20) && *(ras->mobile_ptr) == ' ' ) );

		
	 
	}
	if(!ras->check){
		if(ras->char_match_e>0){
			#ifdef DEBUG
			cprintf(101,"char matchers are not equal:pretty:");
			pretty_print_pointer(ras->char_match_begin,ras->char_match_begin+ras->char_match_e);
			#endif

			uint8_t i1 = 0,i2 = 0;

			
			while(ras->char_match_begin+i2 != ras->char_match_begin+ras->char_match_e){
				while(*(ras->char_match_begin+i2) != '|' && i2 != ras->char_match_e){
					i2++;
				}
				if(!strncmp(ras->mobile_ptr,ras->char_match_begin+i1 ,i2-i1)){
					ras->check = 1;
					ras->mobile_ptr += i2-i1-1;
					break;
				}
				//cprintf(101,"mobile : %c,str comp:%d and p2:[%c]\n",*ras->mobile_ptr,ras->check,*(ras->char_match_begin+i2));
				if(*(ras->char_match_begin+i2) == '|'){
					//cprintf(111,"p2 is or moving \n");
					i1 = ++i2;
				}	
			}
		}else if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
			read_rule_table(ras);
		}
	}


	#ifdef DEBUG
	printf("<%s> ending ras check : %d\n",__FUNCTION__,ras->check);
	#endif

}

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
}



uint8_t regex_lexer_v2(const char *first_ptr){
	cprintf(110,"Input : %s\n================\n",first_ptr);
	rule_flag_struct rfs = {.char_match_begin = NULL,.char_match_end=0, .pattern_flag = 0, .metachar_flag = 0, .table_index = 0};

	uint8_t current_rule_index = 0;
	uint8_t current_token = fbgc_lexer_rule_holder->token;
	const char * rule_ptr = (fbgc_lexer_rule_holder)->rule;

	const char * mobile_ptr = first_ptr;

	uint8_t satisfied_rule_section = 0;
	uint8_t rule_section = 0;

	uint8_t check;

	#define BREAKERR 100
	for(int break_loop = 0;break_loop<BREAKERR && *mobile_ptr != '\0';break_loop++){

		#ifdef DEBUG
			cprintf(110,"buffer:");
			pretty_print_pointer(first_ptr,mobile_ptr);
		#endif

		if(!IS_METACHAR_STAR_OPEN(rfs.metachar_flag) && !IS_METACHAR_PLUS_OPEN(rfs.metachar_flag) && *rule_ptr != '\0'){
			#ifdef DEBUG
				cprintf(100,"Call rule_reader rule_ptr:%s\n",rule_ptr);
			#endif
			rule_ptr = rule_reader_v2(&rfs,rule_ptr);
			rule_section++;

			#ifdef DEBUG
				cprintf(100,"Sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
			#endif

		}

		check = check_char_v2(&rfs,&mobile_ptr);
		if(check ||  IS_METACHAR_STAR_OPEN(rfs.metachar_flag)) satisfied_rule_section = rule_section;

		 
		#ifdef DEBUG
			cprintf(100,"Called check char, mobile_ptr:%s, check:%d\n",mobile_ptr,check);
		#endif

		if(!check || *mobile_ptr == '\0'){
				#ifdef DEBUG
				cprintf(010,"-------------{CHECK = 0}---------------\n");
				cprintf(010,"Sts rule sec:%d, rule_sec:%d, *rule_ptr:%c\n",satisfied_rule_section,rule_section,*rule_ptr);
				#endif
			//why rule_ptr != 0 WHYYYYYY
			if(satisfied_rule_section == rule_section && *rule_ptr != '\0'){
				#ifdef DEBUG
				cprintf(010,"-------------{MOVING IN THE RULE, NEXT}---------------\n");
				#endif
				rfs.metachar_flag = 0;
			}
			else{
				if(first_ptr != mobile_ptr && satisfied_rule_section == rule_section && *rule_ptr == '\0'){
					//if(current_token != SPACE ){
						char * tempstr = (char *) malloc(sizeof(char) * ((mobile_ptr - first_ptr)+1) );
						strncpy(tempstr,first_ptr,(mobile_ptr - first_ptr));
						tempstr[(mobile_ptr - first_ptr)] = '\0';
						cprintf(111,"--------------[TOKEN IS FOUND]-------\n");
						cprintf(101,"[%s : %s]\n\n",tempstr, TYPES_ARRAY[current_token] );
						free(tempstr);
						current_rule_index = 0;
						first_ptr = mobile_ptr;
					//}
					//else {
					//	cprintf(100,"Fucking space!\n");
					//}
				}
				else if(current_rule_index < (RULE_NUMBER-1) && satisfied_rule_section != rule_section){
					#ifdef DEBUG
					cprintf(010,"-------------{CHANGING THE RULE}---------------\n");
					#endif	 
					current_rule_index++;
					mobile_ptr = first_ptr;
				}
				else {
					if(current_rule_index == RULE_NUMBER-1){
						//cprintf(111,"last rule but token couldn't find\n");
						first_ptr = ++mobile_ptr;
						
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

	return 1;
}

uint8_t regex_lexer(const char *buffer){
	
	const char *first_ptr = buffer;
	
	cprintf(110,"Input : %s\n================\n",buffer);
	

	rule_arrange_struct * ras = (rule_arrange_struct *) malloc(sizeof(rule_arrange_struct));
	ras->current_token = (fbgc_lexer_rule_holder)->token;
	ras->rule_ptr = (fbgc_lexer_rule_holder)->rule; //rule pointer travels between different token rules

	uint8_t current_rule_index = 0;
	
	ras->mobile_ptr = first_ptr;
	 
	uint8_t satisfied_rule_section = 0;

	
	
	#define BREAKER 3000

	for(int break_loop = 0;break_loop<BREAKER;break_loop++){
			
		if( !IS_METACHAR_PLUS_OPEN(ras->metachar_flag) && !IS_METACHAR_TABLE_OPEN(ras->metachar_flag) && !IS_METACHAR_STAR_OPEN(ras->metachar_flag)){
			#ifdef DEBUG
			pretty_print_pointer(((fbgc_lexer_rule_holder)+current_rule_index)->rule,ras->rule_ptr);
			#endif
			rule_reader(ras);
		}
		check_char(ras);

		#ifdef DEBUG
		cprintf(110,"Reading str:");
		pretty_print_pointer(first_ptr,ras->mobile_ptr);
		printf("Check : %d\n",ras->check );
		#endif

		if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
			SET_METACHAR_TABLE_FLAG_CLOSE(ras->metachar_flag); 
			current_rule_index = RULE_NUMBER-1;	  	//means goto the founded token state
			satisfied_rule_section = ras->rule_section;	

		}

		if(IS_METACHAR_STAR_OPEN(ras->metachar_flag) && ras->check == 0 ){
			satisfied_rule_section = ras->rule_section;
		}

		if(ras->check){
			(ras->mobile_ptr)++;
			if(!IS_METACHAR_TABLE_OPEN(ras->metachar_flag) && satisfied_rule_section != ras->rule_section)
				satisfied_rule_section = ras->rule_section;	
		} 
		
		if(!(ras->check) || *(ras->mobile_ptr) == '\0'){
			#ifdef DEBUG 
			printf("++++++++++++++++++[CHECK = 0]+++++++++++++++\n");
			#endif
			// 3 durum : rule içinde ilerlemek, rule değiştirmek ve token kabul edip baştaki rule'a dönmek
			//move into the current rule

			if(satisfied_rule_section == ras->rule_section && *(ras->rule_ptr) != '\0'){
				#ifdef DEBUG
				printf("-------------{Moving into rule}---------------\n");
				#endif
				ras->metachar_flag = 0;
			}
			else if(current_rule_index < (RULE_NUMBER-1) && satisfied_rule_section != ras->rule_section){
				#ifdef DEBUG 
				printf("-------------{Change the rule}---------------\n");
				#endif
				//we finished the rule but we couldn't catch a sequence, take next rule
				
				current_rule_index++;
				ras->rule_ptr = ((fbgc_lexer_rule_holder)+current_rule_index)->rule;
				ras->current_token = ((fbgc_lexer_rule_holder)+current_rule_index)->token;
				satisfied_rule_section = ras->rule_section = 0;
				
				ras->metachar_flag = 0;
			}
			else {
				
					if(first_ptr != (ras->mobile_ptr) && satisfied_rule_section == ras->rule_section && *(ras->rule_ptr) == '\0'){
						
						if(ras->current_token != SPACE ){
							char * tempstr = (char *) malloc(sizeof(char) * ((ras->mobile_ptr - first_ptr)+1) );
							strncpy(tempstr,first_ptr,(ras->mobile_ptr - first_ptr));
							tempstr[(ras->mobile_ptr - first_ptr)] = '\0';
							cprintf(111,"--------------[TOKEN IS FOUND]-------\n");
							cprintf(101,"[%s : %s]\n\n",tempstr, TYPES_ARRAY[ras->current_token] );
							free(tempstr);
						}
						
						current_rule_index = 0;
						ras->rule_ptr = (fbgc_lexer_rule_holder)->rule;
						ras->current_token = (fbgc_lexer_rule_holder)->token;

						satisfied_rule_section = ras->rule_section = 0;
						ras->metachar_flag = 0;


						if(*(ras->mobile_ptr) < 5) break;
						
						first_ptr = (ras->mobile_ptr);
					}
					else {
						if(current_rule_index == RULE_NUMBER-1){
							cprintf(100,"last rule but token couldn't find\n");

							break;
						}
					}
			}
		}
		
		ras->check = 0;  		
	}

	free(ras);
	return 1;
}


int main(){
	//const char * buffer = "0b01";
	//"1.2312312312312312312312312454354343545647587768 1j*2.123E23j+2j x = 23 'lolll\tdnasd' abcdef x=5.123123; 1.2E+3j wow _x_f alper alam ";
	//regex_lexer(buffer);

	const char *str = "The quick brown fox jumps over the lazy wolf";
	regex_lexer_v2(str);

	/*rule_flag_struct rfs;
	const char *rule = "0b 1|0!+";
	const char *buf = "0b11";

	//rule = rule_reader_v2(&rfs,rule);
	//uint8_t check = check_char_v2(&rfs,&buf);
	*/

	//printf("%c\n",tolower('.') );
	//printf("size : %ld %ld\n",sizeof(rule_arrange_struct),sizeof(rule_flag_struct) );

	return 0;
}
