import csv
from pprint import *


# take type element for sort
def take_type(elem):
    return gm.tokens[elem.type]

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
		self.lrule_list = []
		self.rrule_list = []
		self.table = []
		self.grammar_tokens = {}
		self.tokens = {}
		self.defines = {}
		

	def append_rule(self,where,rule):
		if(where == "left"):
			rule_list = self.lrule_list
		elif(where == "right"):
			rule_list = self.rrule_list

		rule_list.append(rule)
		rule_list.sort(key=take_type)

	def append_define(self,word,val):

		for vl in range(len(val)): 
			st = val[vl]
			print("st:",st)
			is_predefined = (st in self.defines.keys());
			if((st in self.tokens.keys()) == False and is_predefined == False ):
				self.tokens[st];
			if(is_predefined):
				val.remove(st)
				ls = self.defines[st]
				for i in ls:
					val.append(i)
			 
		self.defines[word] = val
		
	def make_ltable(self):
		self.table = [[0 for x in range(len(self.grammar_tokens))] for y in range(len(self.tokens))]

		for rl in self.lrule_list:
			x = self.tokens[rl.type]-1
			for a in rl.accept:
				y = self.grammar_tokens[a]-1
				self.table[x][y] = self.grammar_tokens[rl.product]
				if(rl.special != []):
					self.table[x][y] *= -1
	
		return self.table;
	def make_rtable(self):
		self.table = [[0 for x in range(len(self.grammar_tokens))] for y in range(len(self.tokens))]

		for rl in self.rrule_list:
			x = self.tokens[rl.type]-1
			for a in rl.accept:
				y = self.grammar_tokens[a]-1
				self.table[x][y] = self.grammar_tokens[rl.product]
				if(rl.special != []):
					self.table[x][y] *= -1
	
		return self.table;

gm = grammar()

filepath = 'fbgc_grammar.txt'

state = ''

with open(filepath) as fp:
	line = 1
	while line:
		line = fp.readline()
		if(line == '\n' or line == '' or "#" in line):
			continue
		
		p,a,t,special = '',[],'',[]
		

		if(line[0] == '!'):
			#read keywords
			line = line.strip('!\t\n')

			print("Reading keyword:",line)
			if(line == "grammar_tokens"):
				state = line;
				line = fp.readline()
				while(line[0] == '\t'):
					gm.grammar_tokens[line.strip('\t\n ')] = len(gm.grammar_tokens)+1
					line = fp.readline()

				pprint(gm.grammar_tokens)
				print("="*50)

			elif(line == "tokens"):
				state = line;
				line = fp.readline()
				while(line[0] == '\t'):
					gm.tokens[line.strip('\t\n ')] = len(gm.tokens)+1
					line = fp.readline()

				pprint(gm.tokens)
				print("="*50)

			elif(line == "left"):
				state = line
				continue

		elif(line[0] == '.'):
			#read assignments
			definition = line 
			line = fp.readline()
			while(line[0] == '\t'):
				definition += line
				line = fp.readline()

			#print(definition)
			line = definition
			line = line.replace('.','')
			line = line.replace('\t','')
			line = line.replace('\n','')
			line = line.replace(' ','')
			#print(line)
			ass_line = line.split("=")
			word = ass_line[0]
			values = ass_line[1]
			values = values.split('|')

			print("Definition:",word,'=:',values,end="\n"+"*"*50+"\n")

			gm.append_define(word,values)


		#reading product
		elif(line[0] != "\t" and line[-2:] == ":\n"):
			p = line.split(":")[0]
			#print("p:",p)
			line = fp.readline()
		while(line != "" and line[0] == '\t'):
			if(line[1] == '('):
				dummy = line.split(') (')
				#print(dummy)
				ls = dummy[0].strip('\t( )\n').split(' | ')
				for i in ls:
					a.append(i)
				#a = accept_line.split('|')
				t = dummy[1].strip('\t( )\n')
				t = t.strip(' ')
				#print("a:",a,"t:",t)
			elif(line[1] == '?'):
				
				line = "".join(line.split())
				line = line.strip('?')
				special = line.split("->")
				print("Condition:",special)
			line = fp.readline()


		if(len(a) > 0):
			print("Product: ",p)
			print("Accept: ",a)
			print("Type: ",t,end="\n"+"-"*50+"\n")
			
			for tit in gm.defines:
				is_defined = 0
				if(tit == t):
					is_defined = 1
					for q in gm.defines[t]:
						gm.tokens[q];#this part will throw an error if we used undefined token
						gm.append_rule(state,rule(p,a,q,special))
				if(is_defined):
					break		



lm = gm.make_ltable()
rm = gm.make_rtable()

#print()
#rint(gms)
#for i in range(len(m)):
#	print("%s\t\t\t"%list(tokens.keys())[list(tokens.values()).index(i+1)],m[i])
#print()

output_file = 'gm_matrix.csv'

with open(output_file, mode='w') as gm_matrix:
    gm_matrix = csv.writer(gm_matrix, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    gm_matrix.writerow(["Left Table",*gm.grammar_tokens])
    for i in range(len(lm)):
    	gm_matrix.writerow( ["%s"%list(gm.tokens.keys())[list(gm.tokens.values()).index(i+1)],*lm[i]])
    gm_matrix.writerow(["Right Table",*gm.grammar_tokens]) 	
    for i in range(len(rm)):
    	gm_matrix.writerow( ["%s"%list(gm.tokens.keys())[list(gm.tokens.values()).index(i+1)],*rm[i]])


print("Definitions")
pprint(gm.defines)


print("File:%s is constructed."%output_file)




# """
# GM_NEWLINE:
# 	(GM_NEWLINE | GM_ATOM) (NEWLINE)
# """
# p = 'GM_NEWLINE'
# a = ['GM_NEWLINE','GM_ATOM']; t = 'NEWLINE';
# gmc.append(rule(p,a,t))

# """
# GM_ATOM:
# 	(GM_NEWLINE) (ATOM)
# 	(GM_BINOP | GM_ASSIGNOP | GM_UNOP) (ATOM)
# 	(GM_UNBALANCED_LIST | GM_LPARA | GM_LBRACK ) (ATOM)
# 	(GM_SEMICOLON | GM_COMMA) (ATOM)
# """	
# p = 'GM_ATOM'
# a = ['GM_NEWLINE','GM_BINOP','GM_UNOP',
# 'GM_UNBALANCED_LIST','GM_LPARA' ,'GM_LBRACK','GM_SEMICOLON','GM_COMMA' ]; t = 'ATOM';
# gmc.append(rule(p,a,t))



# """
# GM_ID:
# 	(GM_NEWLINE) (ID)
# 	(GM_BINOP | GM_ASSIGNOP | GM_UNOP) (ID)
# 	(GM_UNBALANCED_LIST | GM_LPARA | GM_LBRACK ) (ID)
# 	(GM_SEMICOLON | GM_COMMA) (ATOM)
# """	
# p = 'GM_ID'
# a = ['GM_NEWLINE','GM_BINOP','GM_ASSIGNOP','GM_UNOP',
# 'GM_UNBALANCED_LIST','GM_LPARA' ,'GM_LBRACK','GM_SEMICOLON','GM_COMMA' ]; t = 'ID';
# gmc.append(rule(p,a,t))


# """
# GM_LPARA:
# 	(GM_NEWLINE | GM_BINOP | GM_UNOP | GM_ASSIGNOP | GM_LPARA | GM_LBRACK | GM_UNBALANCED_LIST | GM_ID) (LPARA)
# 	(GM_SEMICOLON | GM_COMMA) (LPARA)
# """		
# p = 'GM_LPARA'
# a = ['GM_NEWLINE','GM_BINOP','GM_UNOP','GM_UNBALANCED_LIST','GM_LPARA',
# 'GM_LBRACK','GM_SEMICOLON','GM_COMMA','GM_ID','GM_COMMA','GM_SEMICOLON']; t = 'LPARA';
# gmc.append(rule(p,a,t))




# """
# GM_UNOP:
# 	(GM_NEWLINE | GM_BINOP | GM_UNOP) (UNOP)
# """	
# p = "GM_UNOP"
# a = ["GM_NEWLINE","GM_BINOP","GM_UNOP"]; t = "UNOP";
# gmc.append(rule(p,a,t))



# """
# GM_BINOP:
# 	(GM_ATOM) (BINOP)
# """
# p = "GM_BINOP"
# a = ["GM_ATOM"]; t = "BINOP";
# gmc.append(rule(p,a,t))


# """
# GM_UNOP:
# 	(GM_NEWLINE | GM_BINOP | GM_UNOP) (BINOP)
# 	PLUS -> UPLUS
# 	MINUS -> UMINUS
# """
# p = "GM_UNOP"
# a = ["GM_NEWLINE","GM_BINOP","GM_UNOP"]; t = "BINOP";
# special = "PLUS -> UPLUS | MINUS -> UMINUS"
# gmc.append(rule(p,a,t,special))



# m = gmc.make_table()

# print()
# print(gms)
# for i in range(len(m)):
# 	print("%s\t\t\t"%list(tokens.keys())[list(tokens.values()).index(i+1)],m[i])
# print()

# import csv

# with open('gm_matrix.csv', mode='w') as gm_matrix:
#     gm_matrix = csv.writer(gm_matrix, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
#     gm_matrix.writerow([' ',*gms])
#     for i in range(len(m)):
#     	gm_matrix.writerow( ["%s"%list(tokens.keys())[list(tokens.values()).index(i+1)],*m[i]])

#     #employee_writer.writerow(['Erica Meyers', 'IT', 'March'])