#include "relexer.h"

FILE *fl;
#define STR_EQ "================================================"
#define STR_PLUS "++++++++++++++++++++++++++++++++++++++++++++"
#define STR_LINE "--------------------------------------------"

#define block_print(f,x)(fprintf(f,"==================[%s]=================\n\n",x))
#define info_print(f,x)(fprintf(f,"++++++++++[%s]++++++++++\n\n",x))

void pretty_print_pointer(FILE *fl,const char *buffer ,const char * ptr){
	long int where = ptr-buffer;
	//printf("%s |ptr-> ",buffer);
	for (int i = 0; i < strlen(buffer); ++i){
		if(i== where) fprintf(fl,"[");
		fprintf(fl,"%c",*(buffer+i));
		if(i== where) fprintf(fl,"]");
	}
	fprintf(fl,"\n");	
}

#define DEBUG 
typedef struct{
	string str;
	ufbg_token token;
}token_and_string;



typedef struct{
	string rule_pattern;
	string char_match;
	fbgc_bool flag_star;
	fbgc_bool flag_dollar;
	fbgc_bool flag_table;
	uint8_t rule_section;
	const char *rule_ptr;
	fbgc_bool check;
	const char *mobile_ptr;
	const token_table_struct *table_ptr;
	uint8_t table_state;
	ufbg_token current_token; 

}rule_arrange_struct;


rule_arrange_struct *rule_reader(rule_arrange_struct * ras);
rule_arrange_struct *read_rule_table(rule_arrange_struct * ras);
rule_arrange_struct *check_char(rule_arrange_struct * ras);


rule_arrange_struct *rule_reader(rule_arrange_struct * ras){
	fprintf(fl,"%s is called rule_ptr:%s\n",__FUNCTION__,ras->rule_ptr);
	ras->flag_dollar = ras->flag_star = 0;
	ras->rule_pattern =string("");
  	ras->char_match=string("");

	while(*(ras->rule_ptr) != ' ' && *(ras->rule_ptr) != '\0'){

		if(*(ras->rule_ptr) == '*') {
			ras->flag_star = 1;
		}
		else if(*(ras->rule_ptr) == '$') {
			ras->flag_dollar = 1;
			(ras->rule_ptr)++;
			ras->char_match.push_back(*(ras->rule_ptr));
		}
		else if(*(ras->rule_ptr) == '['){
			//first assume just one char number
			ras->flag_table = 1;
			fprintf(fl,"tablo acildi\n");
			ras->table_ptr = lexer_table+(*((ras->rule_ptr)+1) - '0');
			(ras->rule_ptr) += 5; // sweep to the end [i]
			ras->table_state = 0;
		}
		else if(fun_index(*(ras->rule_ptr)) != LXR_PATTERN_NUM){
			ras->rule_pattern.push_back(*(ras->rule_ptr)); 
		}
		else {
			error("UNDEFINED RULE PATTERN");
			return NULL;
		}
		if(*(ras->rule_ptr) != '\0')(ras->rule_ptr)++;
	}
	ras->rule_section++;
	fprintf(fl,"FLAG : {*:%d $:%d}, PATTERN:%s STR_MATCH:%s\n",
	ras->flag_star,ras->flag_dollar,ras->rule_pattern.c_str(),ras->char_match.c_str());	
	if(*(ras->rule_ptr) == ' ') ras->rule_ptr++;
	return ras;
}
rule_arrange_struct *read_rule_table(rule_arrange_struct *ras){

	fprintf(fl,"=====[%s]$$$$$$$$$$$$$$$$$$$$$$$\n",__FUNCTION__);

	fprintf(fl,"size:%d\n",ras->table_ptr->size ); // so the matrix is size+1 x size 
	uint8_t const size = ras->table_ptr->size;
	//rule_arrange_struct * temp_ras = (rule_arrange_struct*) malloc(sizeof(rule_arrange_struct));
	//hold the current rule struct
	
	const char * hold_rule_ptr = ras->rule_ptr;
	const char * hold_mobile_ptr = ras->mobile_ptr;
	
	fbgc_bool match_occured = 0;

	int koruyucu = 0;


	while(*(ras->mobile_ptr) != '\0' ) { 
		for(uint8_t i = 0; i<size; i++){
			ras->rule_section = 0;
			ras->rule_ptr = ras->table_ptr->inputs[i]; // okunacak yeni kurallar tablodan alınır
			//temp_ras->mobile_ptr = ras->mobile_ptr;
			fprintf(fl, "---------------------------------------\n");
			fprintf(fl,"Inputs[%d]:%s\n",i,ras->rule_ptr);

			while(*(ras->rule_ptr) != '\0'){
				//alınan ilk kural için flagleri belirleyelim
				ras = rule_reader(ras);
				//kuralı verilen char için okuyalım
				ras->flag_table = 0;
				ras = check_char(ras);
				ras->flag_table = 1;
				if(ras->check && *(ras->rule_ptr) != '\0') {
					ras->mobile_ptr++;
					if(*(ras->rule_ptr) == ' ') ras->rule_ptr++;
					fprintf(fl, "So okey move rule_ptr :%s\n",ras->rule_ptr);
				}
				else {  
					while(ras->check == 0 && ras->rule_section != 1) {
						fprintf(fl, "&&&&&&&&&&&&&&&&&&&&&&&&&&6 working\n" );
						ras->rule_section--;
						ras->mobile_ptr--;
					}
					break;

				}
				koruyucu++;
				if(koruyucu >= 50) break;
			}

			fprintf(fl,"Inside for loop >> Check : %d\n",ras->check);

			if(ras->check){
				match_occured = 1;
				//check 1 ise tabloda yeni bir state'e gitmemiz gerekiyor.
				uint8_t dummy_state = ras->table_ptr->states[i + (size * ras->table_state)];
				fprintf(fl,"State >> %d\n",dummy_state );
				if(dummy_state >= size+1){				
					fprintf(fl, "##Hata\n");
					ras->current_token = UNKNOWN;//this is an error state
					ras->check = 0;
				}
				else if(dummy_state == 0){
					//token is found and state returned to beginning, do not change the state
					fprintf(fl, "##En basa dondu\n");
					ras->current_token = ras->table_ptr->tokens[ras->table_state-1];
					ras->check = 0;
					ras->flag_table = 1;
				}
				else{
					//succesfully moving
					
					ras->table_state = ras->table_ptr->states[i + (size * ras->table_state)];
					ras->current_token = ras->table_ptr->tokens[ras->table_state-1];
					ras->flag_table = 1;
					fprintf(fl, "##Moving to state : %d\n",ras->table_state);
					if(ras->table_state == 0){
						fprintf(fl, "########################no need to check again.....\n" );
					}
					
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
		fprintf(fl, "^^^^^^^^^^^^^^^^^^^^^^ERROR VERMELI^^^^^^^^^^^^^^^^^^^\n" );
		ras->flag_table = 0;
	}

	fprintf(fl,"Ras Rule : %s\n",ras->rule_ptr);
	fprintf(fl, "Ras str : %s\n",ras->mobile_ptr );

	
	return ras;
}

rule_arrange_struct *check_char(rule_arrange_struct * ras){

	fprintf(fl, "check char is called mobile:%c \n",*(ras->mobile_ptr) );
	ras->check = 0;
  	for(uint8_t i = 0; i<ras->rule_pattern.size(); i++){
  		ras->check = ras->check || comparator[fun_index(ras->rule_pattern[i])](*(ras->mobile_ptr));
  	}
  	if(ras->flag_dollar){
  		for(uint8_t i = 0; i<ras->char_match.size(); i++){
  			ras->check = ras->check || (ras->char_match[i] == *(ras->mobile_ptr));
  		}
  	}
  	if(ras->flag_table){
  		ras = read_rule_table(ras);
  	}
	return ras;
}



uint8_t regex_lexer(const string &str){
	char *buffer = (char*) malloc((str.size()+1)*sizeof(char));
	strcpy(buffer,str.c_str());
	//char *mobile_ptr = buffer;
	const char *first_ptr = buffer;// this shows the first char location for each token
  	
  	ufbg_lexer_rule_struct * rule_struct_ptr = ufbg_lexer_rule_holder;
  	rule_arrange_struct * ras =(rule_arrange_struct*) malloc(sizeof(rule_arrange_struct));
  	ras->rule_ptr = rule_struct_ptr->rule; //rule pointer travels between different token rules
	ras->current_token = rule_struct_ptr->token;
	uint8_t current_rule_index = 0;
	ras->mobile_ptr = buffer;
	 
	string satisfied_rule_section_str;
  	
  

  	
  	fl = fopen ("relexer.txt","w");
  	block_print(fl,"INPUT STRING");	fprintf(fl,"{%s}\n",buffer );
  	block_print(fl,"WHILE LOOP BEGIN");

  	uint8_t satisfied_rule_section = 0;

  	

  	std::vector<token_and_string> token_vector;


  	int break_loop = 0;
  	#define BREAKER 3000

  	while(*(ras->mobile_ptr) != '\0'){	break_loop++;
  		if( ras->flag_star == 0 && ras->flag_table == 0){
  			ras = rule_reader(ras);
	  	}

	  	
	  	ras = check_char(ras);
	  	fprintf(fl, "$$$$$$$$$$$$$$$$$$$$$$$$\n");
	  	fprintf(fl,"Check %d for :",ras->check); pretty_print_pointer(fl,buffer,ras->mobile_ptr);
	  	fprintf(fl,"FLAG Table:%d\n",ras->flag_table );
	  	fprintf(fl, "Current token : %s\n",TYPES_ARRAY[ras->current_token].c_str());
	  	if(ras->flag_table){
	  		fprintf(fl,"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE:%d\n",ras->flag_table );
	  		ras->flag_table = 0;
			if(ras->current_token != UNKNOWN) 
				//return error("unkown token");
			//else 
				current_rule_index = RULE_NUMBER-1;	  	
				satisfied_rule_section = ras->rule_section;	
	  	}else{
	  		//fprintf(fl, "tablo bunu bulamadi \n");
	  	}



	  	if(ras->check){
	  		(ras->mobile_ptr)++;
	  		if(!(ras->flag_table) && satisfied_rule_section != ras->rule_section){
	  			satisfied_rule_section = ras->rule_section;
	  			satisfied_rule_section_str = string((rule_struct_ptr+current_rule_index)->rule, ras->rule_ptr);
	  			if(satisfied_rule_section_str[satisfied_rule_section_str.size()-1] == ' ')
	  				satisfied_rule_section_str.pop_back();
	  			fprintf(fl,"###[%s]###\n",string("Satisfied rule section :"
	  				+satisfied_rule_section_str).c_str());
	  		} 	
	  	} 
	  	
	  	if(!(ras->check) || *(ras->mobile_ptr) == '\0'){
	  		info_print(fl,"CHECK = 0 CASE");
	  		
	  		// 3 durum : rule içinde ilerlemek, rule değiştirmek ve token kabul edip baştaki rule'a dönmek
	  		//move into the current rule
	  		if(satisfied_rule_section == ras->rule_section && *(ras->rule_ptr) != '\0'){
	  			block_print(fl,"MOVING IN RULE");
	  			ras->flag_star = 0;
	  		}
	  		else if(current_rule_index < (RULE_NUMBER-1) && satisfied_rule_section != ras->rule_section){
	  			//we finished the rule but we couldn't catch a sequence, take next rule
	  			block_print(fl,"CHANGING the RULE");
	  			current_rule_index++;
	  			ras->rule_ptr = (rule_struct_ptr+current_rule_index)->rule;
	  			ras->current_token = (rule_struct_ptr+current_rule_index)->token;
	  			satisfied_rule_section = ras->rule_section = 0;
	  			ras->flag_star = ras->flag_table = 0;

	  			fprintf(fl, "Yeni rule : ");
	  			pretty_print_pointer(fl,(rule_struct_ptr+current_rule_index)->rule,ras->rule_ptr);
	  			if(satisfied_rule_section_str.size()>0){ 
	  				fprintf(fl,"CHECK PREVIOUS RULE [%s]\n",satisfied_rule_section_str.c_str() );
	  			
		  			if(strncmp(ras->rule_ptr,satisfied_rule_section_str.c_str(),satisfied_rule_section_str.size()) == 0){
		  				ras->rule_ptr += satisfied_rule_section_str.size();
		  				if(*(ras->rule_ptr) ==' ') ras->rule_ptr++;
		  				info_print(fl,"THIS RULE ALREADY SATISFIED !");
		  				
		  				satisfied_rule_section = ++ras->rule_section;
		  				fprintf(fl,"Satisfied rule section : %d, ras->rule_section :%d\n",satisfied_rule_section,ras->rule_section);
		  			}	
		  		}
	  		}
	  		else {
	  			
					if(first_ptr != (ras->mobile_ptr) && satisfied_rule_section == ras->rule_section &&*(ras->rule_ptr) == '\0'){
						block_print(fl,"TOKEN IS FOUND");
						string tempstr = string(first_ptr,ras->mobile_ptr);
						fprintf(fl,"--------------------------------------------------\n");
						fprintf(fl,"[%s : %s]\n",tempstr.c_str(), TYPES_ARRAY[ras->current_token].c_str() );
						fprintf(fl,"--------------------------------------------------\n\n\n\n");
						token_and_string v;
						v.str = tempstr; v.token = ras->current_token;
						token_vector.push_back(v);
						
						current_rule_index = 0;
						ras->rule_ptr = rule_struct_ptr->rule;
						ras->current_token = rule_struct_ptr->token;
						
						satisfied_rule_section_str = string("");
						satisfied_rule_section = ras->rule_section = 0;
						
						while(*((ras->mobile_ptr)) == ' ') ras->mobile_ptr++;
						if(*(ras->mobile_ptr) == '\0') break;
						first_ptr = (ras->mobile_ptr);
					}
					else {
						if(current_rule_index == RULE_NUMBER-1){
							//last rule but token couldn't find
							fprintf(fl, "THERE IS NO RULE FOR THIS TOKEN\n");
							fprintf(fl, "LEXER FINISHED WITH AN ERROR\n");
							return 0;
						}
					}
	  		}
	  	}
	  	

  		if(break_loop > BREAKER){
  			fprintf(fl, "##############\noo loop\n##############\n");
  			break;
  		}
  		ras->check = 0;

  		
  		
	}
	for(int i = 0; i<token_vector.size(); i++){
		fprintf(fl,"[%s : %s]\n",token_vector[i].str.c_str(), TYPES_ARRAY[token_vector[i].token].c_str() );
	}

	free(ras);
	free(buffer);
	return 1;
}

int main(){


	string lexerinput = "xy = 0b010100 __fbgencer8__ ";
	
	/*char buffer1[] = "d+ $. d+";
  	char buffer2[] = "d+ $. d+";
	for(char *qs = buffer1; qs<buffer1+strlen(buffer1); qs++){
		pretty_print_pointer(buffer1,qs);
	}*/

	//printf("MEMcmp:%d\n",strncmp( buffer2,buffer1,strlen(buffer1)));
	
	//"'>> e l i f<<' _x12_ + a1b2c4 aAbBcCdDeEfF_gGhHiI ____fbg___ _____ f_b_g 123456 123.323 1E16 1E-12 0x42 0b010100 'LOLnaber' > => ";
	//12323 \"FbG123===\" 'fbg' 2.232 _x_2 3.312

	regex_lexer(lexerinput);
	
	return 0;
}

  			/*fprintf(fl, "--------RULE ARRANGMENT[%d]-----------\n",current_rule_index );
  			if(*rule_ptr != '\0') ras->rule_section++;
  			tempstr = pattern_str = "";
  			fprintf(fl,"Satisfied rule section : %d, ras->rule_section :%d\n",satisfied_rule_section,ras->rule_section);
	  		while(*rule_ptr != ' ' && *rule_ptr != '\0'){
	  			pretty_print_pointer(fl,(rule_struct_ptr+current_rule_index)->rule,rule_ptr);

	  			if(*rule_ptr == '*') flags[LXR_META_STAR] = 1;
	  			else if(*rule_ptr == '$') {flags[LXR_META_DOLLAR] = 1;
	  					rule_ptr++;
	  					tempstr.push_back(*rule_ptr);
				}
	  			else if(fun_index(*rule_ptr) != LXR_PATTERN_NUM){
	  				pattern_str.push_back(*rule_ptr); // yeni rule atarken ya da rule icinde kayarken bunu temizle
	  			}
	  			else {
	  				return error("UNDEFINED RULE PATTERN");
	  			}
	  			rule_ptr++;
	  		}
	  		fprintf(fl,"FLAG : {*:%d $:%d}, PATTERN:%s STR_MATCH:%s\n--------------\n",flags[LXR_META_STAR],flags[LXR_META_DOLLAR],pattern_str.c_str(),tempstr.c_str());
	  		if(*rule_ptr != '\0') rule_ptr++;
	  		*/