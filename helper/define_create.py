
# TILDE $ ~
# COLON $ :
# COMMA $ ,
# DOT $ .
# SEMICOLON $ ;
# PLUSPLUS $ ++
# MINUSMINUS $ --

# R_SHIFT $ >>
# L_SHIFT $ <<
# STARSTAR $ **
# SLASHSLASH $ //
# PLUS $ +
# MINUS $ -
# STAR $ *
# SLASH $ /
# CARET $ ^
# PERCENT $ %

# LO_EQ $ <=
# GR_EQ $ >=
# EQ_EQ $ ==
# NOT_EQ $ !=
# LOWER $ <
# GREATER $ >
# PIPE $ |
# AMPERSAND $ &
# EXCLAMATION $ !
# ASSIGN $ =

set1 = {
	"TILDE" : "~",
	"COLON" : ":",
	"COMMA" : ",",
	"DOT" : ".",
	"SEMICOLON" : ";",
	"PLUSPLUS" : "++",
	"MINUSMINUS" : "--",	
}

set2 = {
	"R_SHIFT_ASSIGN" : ">>=",
	"L_SHIFT_ASSIGN" : "<<=",
	"STARSTAR_ASSIGN" : "**=",
	"SLASHSLASH_ASSIGN" : "//=",	
	"PLUS_ASSIGN" : "+=",
	"MINUS_ASSIGN" : "-=",
	"STAR_ASSIGN" : "*=",
	"SLASH_ASSIGN" : "/=",
	"CARET_ASSIGN" : "^=",
	"PERCENT_ASSIGN" : "%=",	
	"R_SHIFT" : ">>",
	"L_SHIFT" : "<<",
	"STARSTAR" : "**",
	"SLASHSLASH" : "//",	
	"PLUS" : "+",
	"MINUS" : "-",
	"STAR" : "*",
	"SLASH" : "/",
	"CARET" : "^",
	"PERCENT" : "%",	
}

set3 = {
	"LO_EQ" : "<=",
	"GR_EQ" : ">=",
	"EQ_EQ" : "==",
	"NOT_EQ" : "!=",
	"LOWER" : "<",
	"GREATER" : ">",
	"PIPE" : "|",
	"AMPERSAND" : "&",
	"EXCLAMATION" : "!",
	"ASSIGN" : "=",
}



operators = (set1,set2,set3)

for j in range(len(operators)):
	ls = list(operators[j].items())
	output_str = ""
	print("Set",j,sep="")
	for i in range(len(ls)):
		output_str += ls[i][1]
		if(i!=len(ls)-1):
			output_str += '|'
	
	print('"'+output_str+'"',end="\n")




# keys_as_list = list(operator_dict.keys())


# for i in range(len(keys_as_list)):
# 	print(keys_as_list[i],end='\n')
# 	#if(i != len(keys_as_list)-1):
# 		#print(',')

# print()