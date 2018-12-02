#ifndef LEXER_H
#define LEXER_H
#include "include.h"
#include "tokens.h"
#include "error.h"


std::vector<ufbg_token> lextok;
std::vector<string> lexstr;
std::vector<string> lexword;
std::vector<int> lexint;
std::vector<double> lexdouble;


//another approach
// her bir rule definition atomuna char atayalim ve match edersek string arraydan bakalım

/*
	örneğin word için
	{"l_ *ld_",WORD}, // l:letter,d:digit, _ :underscore, space: two different rule, * :until this rule is broken
	{"d *d",INT},
	{">0+b< *>01<",INT}, substring >... <  içine yazılabilir, içindeki herhangi bir karakter özel varsayılmayacak, | op hariç
	{">0+x< *h", INT}, h:hexadecimal
	{ "d >eE<+>\+\-< *d", DOUBLE}, //5e+3,5e-3, gerçek artı ve eksi için kaçış kullanalım yani e+ gelecek : e\+
	{ "d >.< *d",{digit},{'.'},{digit} ,DOUBLE},
	{ ">.< *d",{'.'},{digit}, DOUBLE },
	{ "d >.< *d >ij<"{digit},{'.'},{digit},{i,j} ,COMPLEX},
	{ ">.< *d >ij<"{'.'},{digit},{i,j},COMPLEX },
	{ "o *o",{operator},{operator},OPERATOR}, // operator icin o diyelim
	{ "\" *rnz",{"},{'\r','\n',''},{"},STRING }, //r: \r, n:\n, z: all anlaminda
	{ "\' *rnz",{'},{'\r','\n',''},{'},STRING }
Özetle
	+ -> x+y : we expect y immediately after x
	| -> x|y|z : we expect x OR y OR z
	letter : l -> [a-zA-Z]
	digit : d -> [0-9]
	hexadecimal: h -> [0-F]
	operator : o
	for a spesific char write after $, $a $b $c
	for a spesific string write inside > <, >abc<, >fbg< we can use operators inside ><
		like >abc|fbg|ufbg< or 
		>f|F<+>BG<  ---> this will look only fBG or FBG
		DO NOT put + inside >< just write them side by side >a+b< means >ab<
		however >f<|>F< will be too long therefore we can write >f|F|abcdef<
		if you want to write something like fB... or f...BG try this
		>f|F<+l>BG< ---> this will search f[a-zA-Z]BG		
	rule içinde bir pointer gezerken "l|_ *l|d|_" ele alalım, baştan başlayarak gezecek
	l  geldi ve kullanıcıdan aldığı stringin karakterine bakacak l ile match olmuyorsa 
	böylece içinde ilerlemeye devam edecek, _ görecek ve bakacak alt tire var mı. eğer yoksa 
	ilerleyecek ve boşluk gelecek: match edilmediğini anlayacak ve diğer kuralı kontrol edecek
	ama varsayalım ki _ ile match etti. böylece match edilenler listesinde bunu 1 yapacak

	,{ | olmadan da bulabiliriz	çünkü 
	>< kullanarak string match yapıyoruz}
 */
typedef struct{
	const char *rule;
	ufbg_token type;
}ufbg_lexer_rule_struct;


/*
	l: letter[a-zA-Z]
	d: digit [0-9]
	o : operator  
	$ : for only one character to see its real value, if you want to match l, do not put l, put $l
	* : until \0 or space following portion can come infinitely many.
	+ : Since we don't have or operator, + is going to try match following immediately after previous one
	><: takes real strings inside it and tries to match, for example >ufbg< is going to match as exactly ufbg in the string
	z: all matching, any character, operator, number except \t \n
	t: \t
	n: \n
	': string beginner or ender
	": string begginer or ender
	h: hexadecimal [0-F]
	b: binary [0-1]
	not sure c:complex i or j
	(i) : i.th rule connection , added to reduce search pattern for example if int is rule 1, complex can be said as (1) j  
	
*/

#define LXR_LETTER 0 //for letter:l
#define LXR_DIGIT 1 //for digit:d
#define	LXR_OPERATOR 2 //for op : o
#define LXR_LOOP 3 // for * metach
#define LXR_DOLLAR 4 //for $ metach
#define LXR_TAB 5
#define LXR_NEWLINE 6
#define LXR_MATCH_STR 7
#define LXR_BINARY 8
#define LXR_HEXADECIMAL 9
#define LXR_SPACE 10
#define LXR_FLAGS_NUM 11 //it has to be equal last one + 1

const char * ufbg_lexer_rule_group[] = 
{
	"*d", // so *d is (0)
	"(0) $. (0)" // so (1) is *d . *d but we refer to the first state
};

ufbg_lexer_rule_struct ufbg_lexer_rule_holder [] = 
{
	{"(0) $j",COMPLEX}, //123j
	{"(1) $j",COMPLEX}, //1234.56547j
	{"(1)",DOUBLE}, //123.875
	{"(0)",INT},//12346
};
/*
	{"(4) $j",COMPLEX},	// .012345j		
	{">0b< *$0$1",INT},//0b0101 // binary	
	{">0x< *hd", INT}, // 0xAF
	{"l$_ *ld$_",WORD}, // abc,a12b,a1_,_1_,_2_,____,a_2_b
	{"*o",OP}, // ==, ++, -, *
	{"$. *d",DOUBLE},
	{"*d $E d$+$- *d", DOUBLE}, //5E8,5E+3,5E-3
	{"$\" *tnz $\"",STRING}, //"u\nf\nb\ng" z:
	{"$' *tnz $'",STRING},//'fbgc'
		
*/

#define RULE_NUMBER 4

#define DEBUG 

uint8_t regex_lexer(const string &str){
	char *sptr = strdup(str.c_str());
  	printf("INPUT:[%s]\n",str.c_str() );
  	ufbg_lexer_rule_struct * rule_struct_ptr = ufbg_lexer_rule_holder;
  	const char *rule_ptr = rule_struct_ptr->rule; //rule pointer travels between different token rules
   	
	uint8_t current_rule_index = 0;

	string tempstr;
  	char *str_first; // this shows the first string location
  	ufbg_token current_token = UNKNOWN;
  	
  	//flags
  	bool flags[LXR_FLAGS_NUM] = {0};
#define clear_flags()({	for(uint8_t i = 0; i< LXR_FLAGS_NUM ; flags[i++] = 0);})  	
  	bool check=0;

  	str_first = sptr;
  	string char_matcher = ""; //for $ sign
  	int8_t last_satisfied_rule = -1;
  	int8_t ref_to_other_rule = -1; // first we have no reference

  	while(*sptr != '\0'){
  		printf("================================[fptr:%c | sptr:%c]===========================================\n",*str_first,*sptr);	
		if(!flags[LXR_LOOP]){
			while(*rule_ptr != ' ' && *rule_ptr != '\0'){
				switch(*rule_ptr++){
					case 'l': flags[LXR_LETTER] = 1; break;
					case 'd': flags[LXR_DIGIT] = 1; break;
					case 'o': flags[LXR_OPERATOR] = 1; break;
					case 'z': flags[LXR_LETTER] = flags[LXR_DIGIT] = flags[LXR_OPERATOR] = flags[LXR_SPACE] = flags[LXR_DOLLAR] = 1; 
						char_matcher.push_back('_');
						char_matcher.push_back('.');
					break;
					case '*': flags[LXR_LOOP] = 1; break;
					case '>': flags[LXR_MATCH_STR] = 1;
						while(*rule_ptr != '<') char_matcher.push_back(*rule_ptr++);
						rule_ptr++;
						break;
					case '$': flags[LXR_DOLLAR] = 1; 
						char_matcher.push_back(*rule_ptr++);
					break;
					case '(': 				
						tempstr = "";
						while(*rule_ptr != ')')
							tempstr += *rule_ptr++;
						ref_to_other_rule = stoi(tempstr);
						tempstr = "";
						rule_ptr++;

						/*if(last_satisfied_rule == ref_to_other_rule){
							rule_ptr++;
							last_satisfied_rule = -1;
						}else {
							current_rule_index++;
							rule_struct_ptr++;
							rule_ptr = rule_struct_ptr->rule;							
						}
						printf("Buradayiz, rule: %c\n",*rule_ptr );*/

						break;
					case 'n': flags[LXR_NEWLINE] = 1; break;
					case 't': flags[LXR_TAB] = 1; break;
					case 'b': flags[LXR_BINARY] = 1; break;
					case 'h': flags[LXR_HEXADECIMAL] = 1; break;
					default: printf("Token : %c",*rule_ptr); return error("Lexer undefined rule error.");
				}
			}
			
		}



		check =  (flags[LXR_LETTER] && is_letter(*sptr)) || (flags[LXR_DIGIT] && is_digit(*sptr)) 
		|| (flags[LXR_OPERATOR] && is_operator(*sptr))
		|| (flags[LXR_NEWLINE] && is_newline(*sptr)) || (flags[LXR_TAB] && is_tab(*sptr)) || (flags[LXR_HEXADECIMAL] && is_hexadecimal(*sptr))
		|| (flags[LXR_SPACE] && is_space(*sptr));
		
		if((flags[LXR_DOLLAR] )){
			for(uint8_t i = 0; i< char_matcher.size(); i++ ){
				check = check || (char_matcher[i] == *sptr);
			}
		}
		if((flags[LXR_MATCH_STR])){
			check = check || !(memcmp ( char_matcher.c_str() , sptr, char_matcher.size()));
			if(check) sptr+= char_matcher.size();
		}

		#ifdef DEBUG
		printf("Flags = {");
		for(uint8_t i = 0; i< LXR_FLAGS_NUM; i++ ){
			printf("%d",flags[i]);
			if(i == LXR_FLAGS_NUM - 1) printf("} ");
			else printf(",");
		}

		printf("Current Rule : %s : [%s]\n",rule_struct_ptr->rule, TYPES_ARRAY[rule_struct_ptr->type].c_str() );
		printf("character : {%c}\t",*sptr );
		printf("Check: %d\n",check );		
		#endif

		if(check){
			//so the ch of string is alllowed by the current rule
			//then if rule continue, we have to check other conditions
			sptr++; // next character
			
			//eat space
			current_token = rule_struct_ptr->type;

		}

		if(check && *rule_ptr == '\0'){
			last_satisfied_rule = current_rule_index;
			printf("Last_satisfied_rule is determined as %d\n",last_satisfied_rule );
		}

		if(!check || *sptr == '\0' || *sptr == ' '){
			//printf("==============[CHECK = 0] =============\n");
			//maybe we catch different char for same rule, we have iterate the rule pointer
			//printf("Rule_ptr :%c\n",*rule_ptr );
			//if(str_first != sptr && *rule_ptr == ' ' && *rule_ptr != '\0')
			if( *sptr != ' ' && *rule_ptr != '\0'){
				#ifdef DEBUG
				printf("-----> Rule icinde ilerle[%s]\n",TYPES_ARRAY[rule_struct_ptr->type].c_str());
				#endif
				for(uint8_t i = 0; i< LXR_FLAGS_NUM ; i++ ) *(flags+i) = 0;	
				
				rule_ptr++;
			}

			//change the current rule_index 
			//else if(rule_is_satisfied &&*rule_ptr =='\0') rule_is_satisfied = 0;
			else if( *sptr != ' ' && ( current_rule_index != RULE_NUMBER - 1 ) && *sptr != '\0'){
				#ifdef DEBUG
				printf("-----> Rule degistir[%s]\n",TYPES_ARRAY[rule_struct_ptr->type].c_str());
				#endif
				
				//change the rule maybe it is going to fit into another rule.
				current_rule_index++;
				rule_struct_ptr++;
				rule_ptr = rule_struct_ptr->rule;
				sptr--;
				//sptr = str_first;
				char_matcher ="";
				
				clear_flags();	
					
				//printf("#####RULE CHANGED.\n");
			}
			else if(*sptr == ' ' || current_rule_index == RULE_NUMBER-1 || *sptr == '\0'){
				//evertyhing is searched..
				//check the previous rule what we catch
				#ifdef DEBUG
				printf("Her sey bitti, Token bulabiliriz\n");
				#endif
				
				printf("%c %c\n",*str_first, *sptr );

				if(str_first != sptr){
					while(str_first != sptr){
						tempstr.push_back(*str_first);
						str_first++;
					}
					printf("=================================================\n");
					printf("Finished tempstr : [%s] token :%s\n",tempstr.c_str(), TYPES_ARRAY[current_token].c_str() );
					printf("=================================================\n");



					tempstr="";
					//sptr++;
					
					rule_struct_ptr = ufbg_lexer_rule_holder;
					rule_ptr = rule_struct_ptr->rule;
					char_matcher ="";
					
					clear_flags();

					current_rule_index = 0;
					while(isspace(*(sptr))) sptr++; 
					str_first = sptr;
				}
				else {
					return error("Lexer undefined token");
				}
			}			
			else {
				printf("rule| %d\n",current_rule_index);
				return error("Lexer Grammar rule couldn't find.\n");
			}	

			
		}
		
	}
	

	return 1;
}

string lexerinput = "1.2+3";
//"'>> e l i f<<' _x12_ + a1b2c4 aAbBcCdDeEfF_gGhHiI ____fbg___ _____ f_b_g 123456 123.323 1E16 1E-12 0x42 0b010100 'LOLnaber' > => ";
//12323 \"FbG123===\" 'fbg' 2.232 _x_2 3.312


/*
	\	   |
	 \Input|
State \    |
	   \   |digit| 
----------------------------------------------------------
(INT)	   |     |
	       |     |
	       |     |
	       |     |
	       |     |
	       |     |    
		   |
           |""
           |
           |
           |
           |

*/

int main(){





	regex_lexer(lexerinput);

  /*string line;
  ifstream myfile ("lexer_checker.txt");

  if (myfile.is_open()){
    while ( getline (myfile,line)){
      cout << line << '\n';
    }
    myfile.close();
  }*/
	//cout<<"stoi : "<<atof("2E+3")<<endl;
	//printf("cevap : %ld\n",strtol("5e+3",NULL,10));
	
	return 0;
}

#endif


/*
begin,continue,type
{
	{ {letter,underscore}, {letter,digit,underscore}, WORD }, //a_b, _b_, ____, __ab, abc_
	{ {digit},{digit} ,INT}, //123 , 2223
	{ {0b},{binary} ,INT}, // 0b000101
	{ {0x},{hexadecimal} ,INT},
	{ {digit},{exponential,exponential+unary},{digit}, DOUBLE}, //5e+3,5e-3
	{ {digit},{'.'},{digit} ,DOUBLE},
	{ {'.'},{digit}, DOUBLE },
	{ {digit},{'.'},{digit},{i,j} ,COMPLEX},
	{ {'.'},{digit},{i,j},COMPLEX },
	{ {operator},{operator},OPERATOR},
	{ {"},{'\r','\n',''},{"},STRING },
	{ {'},{'\r','\n',''},{'},STRING }

};	
*/