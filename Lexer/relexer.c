#include "relexer.h"
#include <stdarg.h>

void rgb_printf(char color,const char *format, ...) {


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
		rgb_printf(100,"%c",*(buffer+i));
		if(i== where) printf("]");
	}
	printf("\n");	
}

#define DEBUG 


typedef struct{
	const char * char_match_begin, * char_match_end;
	fbgc_bool pattern_flag;
	fbgc_bool metachar_flag;
	fbgc_bool check;
	uint8_t rule_section;
	const char *rule_ptr, *mobile_ptr;
	const token_table_struct *table_ptr;
	uint8_t table_state;
	fbgc_token current_token; 
}rule_arrange_struct;


void rule_reader(rule_arrange_struct * ras);
void read_rule_table(rule_arrange_struct * ras);
void check_char(rule_arrange_struct * ras);

void rule_reader(rule_arrange_struct * ras){
	
	
	ras->metachar_flag = 0;
  	ras->char_match_begin = ras->char_match_end = NULL;
  	ras->pattern_flag = 0;


	if(*(ras->rule_ptr) !='\0') ras->rule_section++;
	while(*(ras->rule_ptr) != ' ' && *(ras->rule_ptr) != '\0'){
	
		if(*(ras->rule_ptr) == LXR_META_BACKSLASH){
			SET_METACHAR_BACKSLASH_FLAG_CLOSE(ras->metachar_flag);
			//rgb_printf(110,"backslash flag\n");
			ras->rule_ptr++; 

			if( IS_METACHAR(*(ras->rule_ptr))){
				ras->metachar_flag |= SET_METACHARACTER_FLAG_FROM_RULE(*(ras->rule_ptr));
				#ifdef DEBUG
				rgb_printf(110,"metachar_flag : 0x%X\n",ras->metachar_flag);
				#endif
				if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
					//first assume just one char number
					(ras->rule_ptr)++;
					//burayi daha flexible yap
					ras->table_ptr = lexer_table+(*((ras->rule_ptr)) - '0');
					(ras->rule_ptr) ++; // sweep to the end [i]
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

			rgb_printf(010,"<Character sequence catching> rule: %c\n",*ras->rule_ptr);
			if(ras->char_match_begin == NULL)
				ras->char_match_begin = ras->rule_ptr;

			ras->char_match_end = ras->rule_ptr+1;
			
		}


		if(*(ras->rule_ptr) != '\0')(ras->rule_ptr)++;
	

	}
	
	if(*(ras->rule_ptr) == ' ') ras->rule_ptr++;
}

void check_char(rule_arrange_struct * ras){

	#ifdef DEBUG
	//printf("<%s> pattern_flag : [0x%x]\n",__FUNCTION__,ras->pattern_flag);
	#endif
	ras->check = 0;
	ras->check = check_character_with_pattern(*(ras->mobile_ptr),ras->pattern_flag);

  	if(!ras->check && ras->char_match_begin != ras->char_match_end){
  		//rgb_printf(101,"char matchers are not equal:pretty:");
  		//pretty_print_pointer(ras->char_match_begin,ras->char_match_end);
  		const char * p1 = ras->char_match_begin;
  		const char * p2 = p1; 
  		//rgb_printf(111,"============================\n");
  		while(p2 != ras->char_match_end){
			while(*p2 != '|' && p2 != ras->char_match_end){
				//rgb_printf(111,"p2:[%c]",*p2);
	  			p2++;
	  		}
		  	//printf("\n");
		  	//printf("mb: %c,p1 :%c, p2: %c, p2-p1 : %ld\n",*ras->mobile_ptr,*p1,*p2,p2-p1 );
		  	if(!strncmp(ras->mobile_ptr,p1,p2-p1)){
		  		ras->check = 1;
		  		if(p2-p1 > 1) ras->mobile_ptr += p2-p1-1;
		  		//printf("check:1 after comp ras {%s}breaking..\n",ras->mobile_ptr );
		  		break;
		  	}
		  	//rgb_printf(101,"mobile : %c,str comp:%d and p2:[%c]\n",*ras->mobile_ptr,ras->check,*p2);
		  	if(*p2 == '|'){
		  		//rgb_printf(111,"p2 is or moving \n");
		  		p1 = ++p2;
			}	
  		}

  		//rgb_printf(111,"=============[while finished]==============\n");
  	}
  	
  	if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
  		read_rule_table(ras);
  	}
  	#ifdef DEBUG
  	//printf("<%s> ending ras check : %d\n",__FUNCTION__,ras->check);
  	#endif

}

void read_rule_table(rule_arrange_struct *ras){

	uint8_t const input_size = ras->table_ptr->input_size;
	uint8_t const token_size = ras->table_ptr->token_size;
	const char * hold_rule_ptr = ras->rule_ptr;
	const char * hold_mobile_ptr = ras->mobile_ptr;
	
	fbgc_bool match_occured = 0;

	int koruyucu = 0;

	while(*(ras->mobile_ptr) != '\0' ) {

		for(uint8_t i = 0; i<input_size; i++){
			ras->rule_section = 0;
			ras->rule_ptr = ras->table_ptr->inputs[i]; // okunacak yeni kurallar tablodan alınır

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
					else if(ras->table_ptr->states[i + (token_size * ras->table_state)] > token_size){
						//rgb_printf(011,"there must be an error\n");
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
				uint8_t dummy_state = ras->table_ptr->states[i + (token_size * ras->table_state)];
				
				if(dummy_state == 0){
					//state returned to beginning, do not change the state
					
					ras->current_token = ras->table_ptr->tokens[ras->table_state-1];
					ras->check = 0;
					
					SET_METACHAR_TABLE_FLAG_OPEN(ras->metachar_flag);
				}
				else{
					//succesfully moving
					
					ras->table_state = ras->table_ptr->states[i + (token_size * ras->table_state)];
					ras->current_token = ras->table_ptr->tokens[ras->table_state-1];
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




uint8_t regex_lexer(const char *buffer){
	
	const char *first_ptr = buffer;
	
	rgb_printf(110,"Input : %s\n================\n",buffer);
	

  	const fbgc_lexer_rule_struct * rule_struct_ptr = fbgc_lexer_rule_holder;
  	rule_arrange_struct * ras = (rule_arrange_struct *) malloc(sizeof(rule_arrange_struct));
  	ras->rule_ptr = rule_struct_ptr->rule; //rule pointer travels between different token rules
	ras->current_token = rule_struct_ptr->token;
	uint8_t current_rule_index = 0;
	ras->mobile_ptr = buffer;
	 

  	ras->char_match_begin = ras->char_match_end = NULL;

  	uint8_t satisfied_rule_section = 0;

  	
  	int break_loop = 0;
  	#define BREAKER 3000

  	while(*(ras->mobile_ptr) != '\0'){	break_loop++;
  		
  		if( !IS_METACHAR_PLUS_OPEN(ras->metachar_flag) && !IS_METACHAR_TABLE_OPEN(ras->metachar_flag) && !IS_METACHAR_STAR_OPEN(ras->metachar_flag)){
  			#ifdef DEBUG
  			pretty_print_pointer((rule_struct_ptr+current_rule_index)->rule,ras->rule_ptr);
  			#endif
  			rule_reader(ras);
	  	}
	  	check_char(ras);

	  	#ifdef DEBUG
	  	rgb_printf(110,"Reading str:");
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
	  			ras->rule_ptr = (rule_struct_ptr+current_rule_index)->rule;
	  			ras->current_token = (rule_struct_ptr+current_rule_index)->token;
	  			satisfied_rule_section = ras->rule_section = 0;
	  			
	  			ras->metachar_flag = 0;
	  		}
	  		else {
	  			
					if(first_ptr != (ras->mobile_ptr) && satisfied_rule_section == ras->rule_section &&*(ras->rule_ptr) == '\0'){
						
						char * tempstr = (char *) malloc(sizeof(char) * ((ras->mobile_ptr - first_ptr)+1) );
						strncpy(tempstr,first_ptr,(ras->mobile_ptr - first_ptr));
						tempstr[(ras->mobile_ptr - first_ptr)] = '\0';
						rgb_printf(111,"--------------[TOKEN IS FOUND]-------\n");
						rgb_printf(101,"[%s : %s]\n\n",tempstr, TYPES_ARRAY[ras->current_token] );
						
						
						free(tempstr);
						
						current_rule_index = 0;
						ras->rule_ptr = rule_struct_ptr->rule;
						ras->current_token = rule_struct_ptr->token;

						satisfied_rule_section = ras->rule_section = 0;
						ras->metachar_flag = 0;

						
						while(	*((ras->mobile_ptr)) == ' '  || 
								*((ras->mobile_ptr)) == '\n' || 
								*((ras->mobile_ptr)) == '\t' )
							ras->mobile_ptr++;

						if(*(ras->mobile_ptr) < 5) break;
						rgb_printf(011,"after match : %d\n",*(ras->mobile_ptr));
						first_ptr = (ras->mobile_ptr);
					}
					else {
						if(current_rule_index == RULE_NUMBER-1){
							rgb_printf(100,"last rule but token couldn't find\n");

							break;
						}
					}
	  		}
	  	}
	  	

  		if(break_loop > BREAKER){
  			
  			break;
  		}
  		ras->check = 0;

  		
  		
	}

	free(ras);
	return 1;
}





int main(){
	const char * buffer = " 1 2 j    + 2 5 ";
	//"1.2312312312312312312312312454354343545647587768 1j*2.123E23j+2j x = 23 'lolll\tdnasd' abcdef x=5.123123; 1.2E+3j wow _x_f alper alam ";
  	regex_lexer(buffer);

	return 0;
}
