"""
#This is a comment
GM_NEWLINE:
	(GM_NEWLINE | GM_ATOM) (NEWLINE)

#newline soluna atom ya da newline gelebilşr

GM_ATOM:
	(GM_NEWLINE) (ATOM)
	(GM_BINOP) (ATOM)
	(GM_UNOP) (ATOM)

#atom soluna newline ya da binary op gelebilir 1+2 gibi

GM_BINOP:
	(GM_ATOM) (BINOP)

#binop soluna atom gelebilir
# +2*4 : 2,+,4,*

GM_UNOP:
#(GM_BINOP) (BINOP)
	?PLUS -> UPLUS
	?MINUS -> UMINUS 


GM_ASSIGNOP:
	GM_ID (ASSIGNOP) 
"""

from pprint import *
# take type element for sort
def take_type(elem):
    return tokens[elem.type]

class rule():
	#product is num
	#accept is list
	#type is num
	def __init__(self,p,a,t,spe = []):
		self.product = p
		self.accept = a
		self.type = t
		self.special = spe;

	def __str__(self):
		s1 = "(%s):"%str(self.product);
		s2 = "("
		for i in self.accept:
			s2 += str(i) + "|"
		s2+= ')'
		s3 = "(%s)"%str(self.type);

		return s1+"\n\t"+s2+" "+s3+"\n"


class grammar():

	def __init__(self):
		self.rule_list = []
		self.table = []
		

	def append(self,rule):
		self.rule_list.append(rule)
		self.rule_list.sort(key=take_type)
		
	def make_table(self):
		self.table = [[0 for x in range(len(gms))] for y in range(len(tokens))]

		for rl in self.rule_list:
			x = tokens[rl.type]-1
			for a in rl.accept:
				y = gms[a]-1
				self.table[x][y] = gms[rl.product]
				if(rl.special != []):
					self.table[x][y] *= -1
				
		return self.table;

gms = {

	"GM_NEWLINE" : 1,
	"GM_ATOM" : 2,
	"GM_BINOP" : 3,
	"GM_UNOP" :4,
	'GM_ASSIGNOP':5,
	'GM_UNBALANCED_LIST':6,
	'GM_BALANCED_LIST':7,
	'GM_LPARA':8,
	'GM_LBRACK':9,
	'GM_SEMICOLON':10,
	'GM_COMMA':11,
	'GM_ID':12
}

tokens = {
	'NEWLINE':1,
	'ATOM':2,
	'BINOP':3,
	'UNOP':4,
	'ASSIGNOP':5,
	'ID':6,
	'LPARA':7,
	'LBRACK':8,
	'COMMA':9,
	'RPARA':10,
	'RBRACK':11,
	'GM_SEMICOLON':12,	
}

gmc = grammar()


"""
GM_NEWLINE:
	(GM_NEWLINE | GM_ATOM) (NEWLINE)
"""
p = 'GM_NEWLINE'
a = ['GM_NEWLINE','GM_ATOM']; t = 'NEWLINE';
gmc.append(rule(p,a,t))

"""
GM_ATOM:
	(GM_NEWLINE) (ATOM)
	(GM_BINOP | GM_ASSIGNOP | GM_UNOP) (ATOM)
	(GM_UNBALANCED_LIST | GM_LPARA | GM_LBRACK ) (ATOM)
	(GM_SEMICOLON | GM_COMMA) (ATOM)
"""	
p = 'GM_ATOM'
a = ['GM_NEWLINE','GM_BINOP','GM_UNOP',
'GM_UNBALANCED_LIST','GM_LPARA' ,'GM_LBRACK','GM_SEMICOLON','GM_COMMA' ]; t = 'ATOM';
gmc.append(rule(p,a,t))



"""
GM_ID:
	(GM_NEWLINE) (ID)
	(GM_BINOP | GM_ASSIGNOP | GM_UNOP) (ID)
	(GM_UNBALANCED_LIST | GM_LPARA | GM_LBRACK ) (ID)
	(GM_SEMICOLON | GM_COMMA) (ATOM)
"""	
p = 'GM_ID'
a = ['GM_NEWLINE','GM_BINOP','GM_ASSIGNOP','GM_UNOP',
'GM_UNBALANCED_LIST','GM_LPARA' ,'GM_LBRACK','GM_SEMICOLON','GM_COMMA' ]; t = 'ID';
gmc.append(rule(p,a,t))


"""
GM_LPARA:
	(GM_NEWLINE | GM_BINOP | GM_UNOP | GM_ASSIGNOP | GM_LPARA | GM_LBRACK | GM_UNBALANCED_LIST | GM_ID) (LPARA)
	(GM_SEMICOLON | GM_COMMA) (LPARA)
"""		
p = 'GM_LPARA'
a = ['GM_NEWLINE','GM_BINOP','GM_UNOP','GM_UNBALANCED_LIST','GM_LPARA',
'GM_LBRACK','GM_SEMICOLON','GM_COMMA','GM_ID','GM_COMMA','GM_SEMICOLON']; t = 'LPARA';
gmc.append(rule(p,a,t))




"""
GM_UNOP:
	(GM_NEWLINE | GM_BINOP | GM_UNOP) (UNOP)
"""	
p = "GM_UNOP"
a = ["GM_NEWLINE","GM_BINOP","GM_UNOP"]; t = "UNOP";
gmc.append(rule(p,a,t))



"""
GM_BINOP:
	(GM_ATOM) (BINOP)
"""
p = "GM_BINOP"
a = ["GM_ATOM"]; t = "BINOP";
gmc.append(rule(p,a,t))


"""
GM_UNOP:
	(GM_NEWLINE | GM_BINOP | GM_UNOP) (BINOP)
	PLUS -> UPLUS
	MINUS -> UMINUS
"""
p = "GM_UNOP"
a = ["GM_NEWLINE","GM_BINOP","GM_UNOP"]; t = "BINOP";
special = "PLUS -> UPLUS | MINUS -> UMINUS"
gmc.append(rule(p,a,t,special))



m = gmc.make_table()

print()
print(gms)
for i in range(len(m)):
	print("%s\t\t\t"%list(tokens.keys())[list(tokens.values()).index(i+1)],m[i])
print()

import csv

with open('gm_matrix.csv', mode='w') as gm_matrix:
    gm_matrix = csv.writer(gm_matrix, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    gm_matrix.writerow([' ',*gms])
    for i in range(len(m)):
    	gm_matrix.writerow( ["%s"%list(tokens.keys())[list(tokens.values()).index(i+1)],*m[i]])

    #employee_writer.writerow(['Erica Meyers', 'IT', 'March'])