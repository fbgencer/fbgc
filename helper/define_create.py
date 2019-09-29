
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
	"TILDE" : ("~",100),
	"COLON" : (":",100),
	"COMMA" : (",",100),
	"DOT" : (".",200),
	"SEMICOLON" : (";",300),
	"PLUSPLUS" : ("++",48),
	"MINUSMINUS" : ("--",48)	
}

set2 = {
	"R_SHIFT_ASSIGN" : (">>=",30),
	"L_SHIFT_ASSIGN" : ("<<=",30),
	"STARSTAR_ASSIGN" : ("**=",30),
	"SLASHSLASH_ASSIGN" : ("//=",30),	
	"PLUS_ASSIGN" : ("+=",30),
	"MINUS_ASSIGN" : ("-=",30),
	"STAR_ASSIGN" : ("*=",30),
	"SLASH_ASSIGN" : ("/=",30),
	"CARET_ASSIGN" : ("^=",30),
	"PERCENT_ASSIGN" :("%=",30),	
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
	"ASSIGN" : ("=",100)
}



operators = (set1,set2,set3)

for j in range(len(operators)):
	ls = list(operators[j].items()) #convert eveything into a list
	output_str = ""
	print("Set",j,sep="")
	for i in range(len(ls)):
		if(type(ls[i][1]) == type( (1,2,3) ) ):
			output_str += ls[i][1][0]
			print("Look :",ls[i][1][1])
		else:
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