#include "relexer.h"


void pretty_print_pointer(const char *buffer ,const char * ptr){
	long int where = ptr-buffer;
	//printf("%s |ptr-> ",buffer);
	for (int i = 0; i < strlen(buffer); ++i){
		if(i== where) printf("[");
		printf("%c",*(buffer+i));
		if(i== where) printf("]");
	}
	printf("\n");	
}

#define DEBUG 

typedef struct{
	string str;
	fbgc_token token;
}token_and_string;


typedef struct{
	string char_match;
	fbgc_bool pattern_flag;
	fbgc_bool metachar_flag;
	uint8_t rule_section;
	const char *rule_ptr, *mobile_ptr;
	fbgc_bool check;
	const token_table_struct *table_ptr;
	uint8_t table_state;
	fbgc_token current_token; 
}rule_arrange_struct;


void rule_reader(rule_arrange_struct * ras);
void read_rule_table(rule_arrange_struct * ras);
void check_char(rule_arrange_struct * ras);

void rule_reader(rule_arrange_struct * ras){
	
	
	ras->metachar_flag = 0;
  	
  	ras->char_match = string("");
  	ras->pattern_flag = 0;


	if(*(ras->rule_ptr) !='\0') ras->rule_section++;
	while(*(ras->rule_ptr) != ' ' && *(ras->rule_ptr) != '\0'){

		//printf("checking rule_ptr [%c]\n",*ras->rule_ptr );
		ras->metachar_flag |= GET_METACHARACTER_FLAG_FROM_RULE(*(ras->rule_ptr));
		//if(ras->metachar_flag) ras->rule_ptr++;

		if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
			//first assume just one char number

			(ras->rule_ptr)++;
			//burayi daha flexible yap
			ras->table_ptr = lexer_table+(*((ras->rule_ptr)) - '0');
			(ras->rule_ptr) ++; // sweep to the end [i]
			ras->table_state = 0;
		}		
		else if(IS_METACHAR_BACKSLASH_OPEN(ras->metachar_flag)){
			SET_METACHAR_BACKSLASH_FLAG_CLOSE(ras->metachar_flag);
			ras->rule_ptr++; 
			//printf("Backslash Next %c\n",*ras->rule_ptr );
			ras->pattern_flag = ras->pattern_flag | GET_PATTERN_FLAG_FROM_RULE(*(ras->rule_ptr));
	
			if(*(ras->rule_ptr) == LXR_META_PLUS || *(ras->rule_ptr) == LXR_META_STAR ||*(ras->rule_ptr) == ' ')
			{			
				//printf("Catched : [%c]\n",*(ras->rule_ptr) );
				ras->char_match.push_back(*(ras->rule_ptr));
				
			}
			else if(ras->pattern_flag == 0){
				
				printf("Undefined rule ras->rule_ptr c:%c d:%d\n",*(ras->rule_ptr),*(ras->rule_ptr));
			}
		}
		else{
			if(*(ras->rule_ptr) != LXR_META_STAR && *(ras->rule_ptr) != LXR_META_PLUS)
				ras->char_match.push_back(*(ras->rule_ptr));
		}
		if(*(ras->rule_ptr) != '\0')(ras->rule_ptr)++;
	}
	//
	if(*(ras->rule_ptr) == ' ') ras->rule_ptr++;
}

void check_char(rule_arrange_struct * ras){

	#ifdef DEBUG
	printf("<%s> pattern_flag : [0x%x] char_match :[%s] \n",__FUNCTION__,ras->pattern_flag,ras->char_match.c_str() );
	#endif
	ras->check = 0;
	ras->check = (ras->check) || check_character_with_pattern(*(ras->mobile_ptr),ras->pattern_flag);

  	for(uint8_t i = 0; i<ras->char_match.size(); i++){
  		ras->check = (ras->check)  || (ras->char_match[i] == *(ras->mobile_ptr));
  	}
  	
  	if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
  		#ifdef DEBUG
  		printf("Table acildi..\n");
  		#endif
  		read_rule_table(ras);
  	}

  	printf("<%s> ras check : %d\n",__FUNCTION__,ras->check);
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
				else if(dummy_state >= token_size+1){
					ras->current_token = UNKNOWN;
					#ifdef DEBUG 
						printf("LEXER ERROR\n" ); 
					#endif
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

//#undef DEBUG


uint8_t regex_lexer(){
	const char * buffer = "0xFF 0x10 \nlol ";//= "0x01FF  0xFF 1j*2.123E23j+2j x::= = 23 'lolll\ndnasd' abcdef x=5.123123; 1.2E+3j wow _x_f alper alam ";
	const char *first_ptr = buffer;

  	const fbgc_lexer_rule_struct * rule_struct_ptr = fbgc_lexer_rule_holder;
  	rule_arrange_struct * ras = (rule_arrange_struct *) malloc(sizeof(rule_arrange_struct));
  	ras->rule_ptr = rule_struct_ptr->rule; //rule pointer travels between different token rules
	ras->current_token = rule_struct_ptr->token;
	uint8_t current_rule_index = 0;
	ras->mobile_ptr = buffer;
	 
	string satisfied_rule_section_str;
  	
  	const char * satisfied_rule_section_ptr_begin, * satisfied_rule_section_ptr_end; 
  


  	uint8_t satisfied_rule_section = 0;

  	std::vector<token_and_string> token_vector;
  	
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
	  	pretty_print_pointer(first_ptr,ras->mobile_ptr);
	  	printf("Check : %d\n",ras->check );
	  	#endif


	  	if(IS_METACHAR_TABLE_OPEN(ras->metachar_flag)){
	  		SET_METACHAR_TABLE_FLAG_CLOSE(ras->metachar_flag); 
			current_rule_index = RULE_NUMBER-1;	  	
			satisfied_rule_section = ras->rule_section;	
	  	}

	  	if(IS_METACHAR_STAR_OPEN(ras->metachar_flag) && ras->check == 0 ){
	  		
	  		satisfied_rule_section = ras->rule_section;
	  	}

	  	if(ras->check){
	  		(ras->mobile_ptr)++;
	  		//printf("check1: %c\n",*ras->mobile_ptr );
	  		if(!IS_METACHAR_TABLE_OPEN(ras->metachar_flag) && satisfied_rule_section != ras->rule_section){
	  			satisfied_rule_section = ras->rule_section;

	  			satisfied_rule_section_ptr_begin = (rule_struct_ptr+current_rule_index)->rule;
	  			satisfied_rule_section_ptr_end = ras->rule_ptr;

	  			satisfied_rule_section_str = string((rule_struct_ptr+current_rule_index)->rule, ras->rule_ptr); // begin to end 
	  			printf("<satisfied_rule_section_str> : %s\n",satisfied_rule_section_str.c_str());

	  			if(satisfied_rule_section_str[satisfied_rule_section_str.size()-1] == ' '){
	  				satisfied_rule_section_str.pop_back();
	  			}
	  			if(*satisfied_rule_section_ptr_end == ' ') satisfied_rule_section_ptr_end--;
	  			
	  		} 	
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

	  			if(satisfied_rule_section_str.size()>0){ 
	  				#ifdef DEBUG 
	  				printf("-------------{Check the previous rule}---------------\n");
	  				printf("previous Rule :[%s]\n",satisfied_rule_section_str.c_str());
	  				printf("Pointer prev rule : [%s]\n",string(satisfied_rule_section_ptr_begin,satisfied_rule_section_ptr_end).c_str());
	  				#endif
	  				
		  			if(strncmp(ras->rule_ptr,satisfied_rule_section_str.c_str(),satisfied_rule_section_str.size()) == 0){
		  				ras->rule_ptr += satisfied_rule_section_str.size();
		  				if(*(ras->rule_ptr) ==' ') ras->rule_ptr++;
		  				
		  				satisfied_rule_section = ++ras->rule_section;
		  			}	

		  		}
	  		}
	  		else {
	  			
					if(first_ptr != (ras->mobile_ptr) && satisfied_rule_section == ras->rule_section &&*(ras->rule_ptr) == '\0'){
						
						string tempstr = string(first_ptr,ras->mobile_ptr);
						#ifdef DEBUG
						printf("------------------------[TOKEN IS FOUND]--------------------------\n");
						printf("[%s : %s]\n",tempstr.c_str(), TYPES_ARRAY[ras->current_token] );
						printf("------------------------[TOKEN IS FOUND]---------------------\n\n\n\n");
						#endif
						
						token_and_string v;
						v.str = tempstr; v.token = ras->current_token;
						token_vector.push_back(v);
						
						current_rule_index = 0;
						ras->rule_ptr = rule_struct_ptr->rule;
						ras->current_token = rule_struct_ptr->token;
						
						satisfied_rule_section_str = string("");
						satisfied_rule_section = ras->rule_section = 0;
						ras->metachar_flag = 0;

						
						while(	*((ras->mobile_ptr)) == ' '  || 
								*((ras->mobile_ptr)) == '\n' || 
								*((ras->mobile_ptr)) == '\t' )
							ras->mobile_ptr++;

						if(*(ras->mobile_ptr) == '\0') break;
						first_ptr = (ras->mobile_ptr);
					}
					else {
						if(current_rule_index == RULE_NUMBER-1){
							printf("last rule but token couldn't find\n");
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

	///*FILE *fl = fopen("relexer.txt","w");
	//fprintf(fl, "Input:\n%s\n-----------------------\n",buffer);
	//printf( "Input:\n%s\n-----------------------\n",buffer);
	
	for(int i = 0; i<token_vector.size(); i++){
		printf("\033[1;33m[%s]\033[0m \033[1;31m{%s}\033[0m\n",token_vector[i].str.c_str(),TYPES_ARRAY[token_vector[i].token]);
	}

	free(ras);
	return 1;
}

int main(){

  	int x = regex_lexer();

	return 0;
}
