
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



output_str = ""


ls = list(operator_dict.items())


for i in range(len(ls)):
	output_str += ls[i][1]
	if(i!=len(ls)-1):
		output_str += '|'


print('"'+output_str+'"',end="\n\n\n")




keys_as_list = list(operator_dict.keys())


for i in range(len(keys_as_list)):
	print(keys_as_list[i],end='')
	if(i != len(keys_as_list)-1):
		print(',')

print()