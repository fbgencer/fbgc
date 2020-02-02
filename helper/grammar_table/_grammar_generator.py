class grammar():
	#product is str
	#accept is dict
	#type is str
	def __init__(self,p,a,t):
		self.product = p
		self.accept = a
		self.type = t

		for i in a:
			if(t == p):
				a[i] = '1'
			else:
				a[i] = p[:3]


	def merge(self,ob):
		if(self.type == ob.type):
			new_dict = {**self.accept,**ob.accept}
			self.accept = new_dict

	def __str__(self):
		return self.type + "\nAccepts:" + str(self.accept) + "\nProduces:" + self.product+"\n"
	
	def __eq__(self, o):
		return self.type == o.type



filepath = 'fbgc_grammar.txt'

leftgm_list = []


with open(filepath) as fp:
	line = 1
	while line:
		line = fp.readline()
		if(line == '\n'):
			continue
		if(line == '' or "#" in line):
			continue
		
		a = {}

		if(line[0] != "\t" and line[-2:] == ":\n"):
			p = line.split(":")[0]
			#print("Product: ",p)
			line = fp.readline()
		while(line[0] == '\t'):
			if(line[1] == '?'):
				print("soru ")
				line = "".join(line.split())
				line = line.strip('?')
				condition = line.split("->")
				print(condition)
			else:
				dummy = line.split(') (')
				#print(dummy)
				ls = dummy[0].strip('\t( )\n').split(' | ')
				for i in ls:
					a[i] = 'X'
				#print("Accept: ",a)
				#a = accept_line.split('|')
				t = dummy[1].strip('\t( )\n')
				t = t.strip(' ')
				#print("Type: ",t,end="\n--------------------\n")
			line = fp.readline()
			if(line == "" or "#" in line): break
		

		q = grammar(p,a,t)
		is_in = ""
		try:
			is_in = leftgm_list.index(q)
		except ValueError:
			pass

		if(is_in == ""):
			leftgm_list.append(q)
		else:
			x = leftgm_list[is_in]
			x.merge(q)
			#print("aynisi %d'de bulundu"%is_in)


rows = []
for i in leftgm_list:
	rows.append(i.type)
columns = []
for i in leftgm_list:
	for j in i.accept:
		if(j not in columns):
			columns.append(j)


matrix = []

for i in range(len(rows)):
	x = ['-']*len(columns)
	matrix.append(x)

where = 0

for i in range(len(leftgm_list)):
	g = leftgm_list[i]
	for j in g.accept:
		try:
			where = columns.index(j)
		except ValueError:
			pass
		matrix[i][where] = g.accept[j]




print(" "*30,columns)
for i in range(len(matrix)):
	print("%30s"%(rows[i]),end="")
	for j in matrix[i]:
		print("%15s"%j,end="")
	print()
