#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif


/*! @details This object is shared between parser main function and its helpers. Instead of pushing all the variables, it holds all the necessary
information about the state of parser.
We NEVER create this object dynamically, just parser function has one #parser_packet object and pass its address to helper functions
*/
struct parser_packet{
	struct fbgc_ll * head; 				//!< Linked list head object,namely main code list. This is our AST structure
	struct fbgc_ll_base * iter;			//!< Iterator for main code
	struct fbgc_ll_base * iter_prev;	//!< Always holds previous value for iterator, we use for insertion and deletions
	struct fbgc_ll_base * op;			//!< Operator stack, it's  a linked list but we use it as stack
	struct fbgc_ll_base * scope_list;	//!< Field,class and function scope list, we use it as a stack
	struct fbgc_ll_base * statement_begin;
	char * line;
	size_t line_no;
	uint8_t error_code;					//!< When an error occurs function assigns error code to this variable
	fbgc_grammar gm;					//!< State of grammar
	uint8_t state_pop_top;
};


uint8_t parse_file(struct fbgc_object ** field_obj, const char * file_name);
uint8_t parse_string(struct fbgc_object ** field_obj, char * str);



/*! \cond DOCUMENT_EVERYTHING
	Lets not document this part, just log definitions nothing important 
*/
#ifdef LOG_PARSER
#define PARSER_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define PARSER_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _PARSER_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _PARSER_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define PARSER_LOGV(...)
#define PARSER_LOGD(...)
#define _PARSER_LOGV(...)
#define _PARSER_LOGD(...)
#endif

#ifdef  __cplusplus
}
#endif

#endif