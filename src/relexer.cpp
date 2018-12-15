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

//#define DEBUG 

typedef struct{
	string str;
	fbgc_token token;
}token_and_string;



typedef struct{
	string rule_pattern,char_match;
	fbgc_bool flag_plus,flag_star,flag_char_match,flag_table;
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
	
	ras->flag_char_match = ras->flag_plus = ras->flag_star = 0;
	ras->rule_pattern =string("");
  	ras->char_match = string("");
  	//printf("ras->rule_ptr %s\n",ras->rule_ptr );

	if(*(ras->rule_ptr) !='\0') ras->rule_section++;
	while(*(ras->rule_ptr) != ' ' && *(ras->rule_ptr) != '\0'){

		if(*(ras->rule_ptr) == LXR_META_PLUS) {			
			ras->flag_plus = 1;
		}
		else if(*(ras->rule_ptr) == LXR_META_STAR){
			ras->flag_star = 1;
		}
		else if(*(ras->rule_ptr) == LXR_META_TABLE){
			//first assume just one char number

			(ras->rule_ptr)++;
			ras->flag_table = 1;
			
			//burayi daha flexible yap
			ras->table_ptr = lexer_table+(*((ras->rule_ptr)) - '0');
			(ras->rule_ptr) ++; // sweep to the end [i]
			ras->table_state = 0;
		}		
		else if(*(ras->rule_ptr) == LXR_META_BACKSLASH){
			ras->rule_ptr++; 
			//printf("Backslash Next %c\n",*ras->rule_ptr );
			if(fun_index(*(ras->rule_ptr)) < LXR_PATTERN_NUM)
				ras->rule_pattern.push_back(*(ras->rule_ptr)); 
			else if(*(ras->rule_ptr) == LXR_META_PLUS || *(ras->rule_ptr) == LXR_META_STAR
				||*(ras->rule_ptr) == ' '
			) {			
				ras->char_match.push_back(*(ras->rule_ptr++));
			}
			else {
				printf("Undefined rule c:%c d:%d\n",*(ras->rule_ptr),*(ras->rule_ptr));
				error("UNDEFINED RULE PATTERN");
			}
		}
		else{
			//printf("Match icin char :%c\n",*((ras->rule_ptr)));
			ras->char_match.push_back(*(ras->rule_ptr));			
			//if(*ras->rule_ptr == '\0') break;
			
		}
		if(*(ras->rule_ptr) != '\0')(ras->rule_ptr)++;
	}
	//
	if(*(ras->rule_ptr) == ' ') ras->rule_ptr++;
	//printf("{*:%d +:%d [:%d} Match:[%s] Pattern[%s]\n",ras->flag_star,ras->flag_plus,ras->flag_table,ras->char_match.c_str(),ras->rule_pattern.c_str() );
}

void check_char(rule_arrange_struct * ras){

	#ifdef DEBUG
	printf("<%s> rule_pattern :[%s] char_match :[%s] \n",__FUNCTION__,ras->rule_pattern.c_str(), ras->char_match.c_str() );
	#endif
	ras->check = 0;
  	for(uint8_t i = 0; i<ras->rule_pattern.size(); i++){
  		ras->check = (ras->check) || comparator[fun_index(ras->rule_pattern[i])](*(ras->mobile_ptr));
  	}
  	for(uint8_t i = 0; i<ras->char_match.size(); i++){
  		ras->check = (ras->check)  || (ras->char_match[i] == *(ras->mobile_ptr));
  	}
  	
  	if(ras->flag_table){
  		#ifdef DEBUG
  		printf("Table acildi..\n");
  		#endif
  		read_rule_table(ras);
  	}
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
				ras->flag_table = 0; //bu degistebilir mi ?
				check_char(ras);
				ras->flag_table = 1;

				if(ras->check && *(ras->rule_ptr) != '\0') {
					ras->mobile_ptr++;
					if(*(ras->rule_ptr) == ' ') ras->rule_ptr++;
				}
				else {  
					if( !ras->check  && ras->flag_star) {
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
					ras->flag_table = 1;
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
					ras->flag_table = 1;
					
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
		
		ras->flag_table = 0;
	}
}

//#undef DEBUG


uint8_t regex_lexer(){
	const char * buffer = "0b0101 0xFF 1+2j x=5.123123; 1.2E+3j wow _x_f alper alam";
	const char *first_ptr = buffer;

  	const fbgc_lexer_rule_struct * rule_struct_ptr = fbgc_lexer_rule_holder;
  	rule_arrange_struct * ras = (rule_arrange_struct *) malloc(sizeof(rule_arrange_struct));
  	ras->rule_ptr = rule_struct_ptr->rule; //rule pointer travels between different token rules
	ras->current_token = rule_struct_ptr->token;
	uint8_t current_rule_index = 0;
	ras->mobile_ptr = buffer;
	 
	string satisfied_rule_section_str;
  	
  


  	uint8_t satisfied_rule_section = 0;

  	std::vector<token_and_string> token_vector;
  	
  	int break_loop = 0;
  	#define BREAKER 3000

  	while(*(ras->mobile_ptr) != '\0'){	break_loop++;
  		
  		if( ras->flag_plus == 0 && ras->flag_table == 0 && ras->flag_star == 0){
  			#ifdef DEBUG
  			pretty_print_pointer(rule_struct_ptr->rule,ras->rule_ptr);
  			#endif
  			rule_reader(ras);
	  	}
	  	check_char(ras);

	  	#ifdef DEBUG
	  	pretty_print_pointer(first_ptr,ras->mobile_ptr);
	  	printf("Check : %d\n",ras->check );
	  	#endif


	  	if(ras->flag_table){
	  		
	  		ras->flag_table = 0;
			if(ras->current_token != UNKNOWN) 
				//return error("unkown token");
			//else 
				current_rule_index = RULE_NUMBER-1;	  	
				satisfied_rule_section = ras->rule_section;	
	  	}else{
	  		//
	  	}
	  	 

	  	if(ras->flag_star && ras->check == 0 ){
	  		
	  		satisfied_rule_section = ras->rule_section;
	  	}

	  	if(ras->check){
	  		(ras->mobile_ptr)++;
	  		//printf("check1: %c\n",*ras->mobile_ptr );
	  		if(!(ras->flag_table) && satisfied_rule_section != ras->rule_section){
	  			satisfied_rule_section = ras->rule_section;
	  			satisfied_rule_section_str = string((rule_struct_ptr+current_rule_index)->rule, ras->rule_ptr);
	  			if(satisfied_rule_section_str[satisfied_rule_section_str.size()-1] == ' ')
	  				satisfied_rule_section_str.pop_back();
	  			
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
	  			ras->flag_plus = 0;
	  			ras->flag_star = 0;
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
	  			ras->flag_plus = ras->flag_table = 0;

	  			if(satisfied_rule_section_str.size()>0){ 
	  				#ifdef DEBUG 
	  				printf("-------------{Check the previous rule}---------------\n");
	  				printf("previous Rule :%s\n",satisfied_rule_section_str.c_str());
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
						printf("[%s : %s]\n",tempstr.c_str(), TYPES_ARRAY[ras->current_token].c_str() );
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
						ras->flag_plus = ras->flag_table = ras->flag_star = ras->flag_char_match = 0;

						
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
		cout<<token_vector[i].str<<":"<<TYPES_ARRAY[token_vector[i].token]<<endl;
		//fprintf(fl,"[%s : %s]\n",token_vector[i].str.c_str(), TYPES_ARRAY[token_vector[i].token].c_str() );
		//printf("[%s : %s]\n",token_vector[i].str.c_str(), TYPES_ARRAY[token_vector[i].token].c_str() );
	}

	free(ras);
	//free(buffer);
	
	

	return 1;
}

int main(){

  	int x = regex_lexer();

	return 0;
	
/*	string lexerinput; //" 'f _ b _ g' 1.2e "
	while(lexerinput != "exit")
{	
	cout<<"Enter text : ";
	cin>>lexerinput;
	//"12.12 10j 8 12E-23 1.213E2j abcdef ++ __FUNCTION__ 1E+2 1E-2 1E88554154j .213123 x _ + xy x_ _x_x ____ _x123_   ";
	regex_lexer(lexerinput);
}
*/
 /* string line;
  ifstream myfile ("relexer_test.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      cout << line << '\n';
    }
    myfile.close();
  }
  else cout << "Unable to open file"; 
  */

}
