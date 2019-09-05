
operator_dict = {
"THREE_DOT" : "...",
"STARSTAR" : "**",
"PLUSPLUS" : "++",
"MINUSMINUS" : "--",
"SLASHSLASH" : "//",
"LO_EQ" : "<=",
"GR_EQ" : ">=",
"EQ_EQ" : "==",
"NOT_EQ" : "!=",
"PLUS_ASSIGN" : "+=",
"MINUS_ASSIGN" : "-=",
"STAR_ASSIGN" : "*=",
"SLASH_ASSIGN" : "/=",
"CONST_ASSIGN" : ":=",
"IF_THEN" : "=>",
"R_SHIFT" : ">>",
"L_SHIFT" : "<<",
"R_ARROW" : "->",
"L_ARROW" : "<-",
"RW_ARROW" : "~>",
"LW_ARROW" : "<~",
"TWO_COLON" : "::",
"ASSIGN" : "=",
"SEMICOLON" : ";",
"COMMA" : ",",
"DOT" : ".",
"COLON" : ":",
"CARET" : "^",
"TILDE" : "~",
"PERCENT" : "%",
"EXCLAMATION" : "!",
"LOWER" : "<",
"GREATER" : ">",
"PIPE" : "|",
"AMPERSAND" : "&",
"SLASH" : "/",
"STAR" : "*",
"MINUS" : "-",
"PLUS" : "+",
}

operator_dict2 = {
"THREE_DOT" : "...",
"R_ARROW" : "->",
"L_ARROW" : "<-",
"RW_ARROW" : "~>",
"LW_ARROW" : "<~",
"TWO_COLON" : "::",
"PLUS_ASSIGN" : "+=",
"MINUS_ASSIGN" : "-=",
"STAR_ASSIGN" : "*=",
"SLASH_ASSIGN" : "/=",
"CONST_ASSIGN" : ":=",
"PLUSPLUS" : "++",
"MINUSMINUS" : "--",
"IF_THEN" : "=>",
"STARSTAR" : "**",
"SLASHSLASH" : "//",
"LO_EQ" : "<=",
"GR_EQ" : ">=",
"EQ_EQ" : "==",
"NOT_EQ" : "!=",
"R_SHIFT" : ">>",
"L_SHIFT" : "<<",
"CARET" : "^",
"PERCENT" : "%",
"LOWER" : "<",
"GREATER" : ">",
"PIPE" : "|",
"AMPERSAND" : "&",
"SLASH" : "/",
"STAR" : "*",
"MINUS" : "-",
"PLUS" : "+",
"EXCLAMATION" : "!",
"TILDE" : "~",
"SEMICOLON" : ";",
"COMMA" : ",",
"DOT" : ".",
"COLON" : ":",
"ASSIGN" : "=",
}

output_str = ""

operator_dict = operator_dict2

ls = list(operator_dict.items())


for i in range(len(ls)):
	output_str += ls[i][1]
	if(i!=len(ls)-1):
		output_str += '|'


print('"'+output_str+'"',end="\n\n\n")




keys_as_list = list(operator_dict.keys())


for i in range(len(keys_as_list)):
	print(keys_as_list[i],end='\n')
	#if(i != len(keys_as_list)-1):
		#print(',')

print()