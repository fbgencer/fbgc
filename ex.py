#create random variable names and strings
# from random import *

# f = open("ex.fbgc",'w')

# alph = list(map(chr, range(97, 123)))

# no = 10000
# for i in range(0,no):
# 	x = alph[randint(0,len(alph)-1)]
# 	y = alph[randint(0,len(alph)-1)]
# 	z = alph[randint(0,len(alph)-1)]
# 	f.write("x = '"+x+y+z+str(i)+str(randint(0,15000))+"'\n")
# 	f.write('print(x)\n')

# f.close()


##############################

# 1000 no padding
# Execution time [LEXER] :0.007984
# Execution time [PARSER] :0.000343
# Execution time [INTERPRETER] :0.003685
# Total ex time 0.012012

# 1000 padding

# Execution time [LEXER] :0.007925
# Execution time [PARSER] :0.000292
# Execution time [INTERPRETER] :0.002771
# Total ex time 0.010988




# f = open("ex.fbgc","w")


# no = 10

# var_str = 'a'

# for i in range(no):
# 	f.write(var_str+' = '+"'str(no-i)'"+'\n')
# 	f.write("print('abdefgasdkasdkasfksdfnsdfsdfsdfsdjfkjsdfjsdkf',a)\n")
# for i in range(no):
# 	pass
	

# f.close()

# import sys

# # Writen by Attractive Chaos; distributed under the MIT license

# # reference: http://www.syntagmatic.net/matrix-multiplication-in-python/

# def matmul(a, b): # FIXME: no error checking
# 	c = [[b[j][i] for j in range(len(b))] for i in range(len(b[0]))]
# 	d = [[0 for j in range(len(b[0]))] for i in range(len(a))] # transpose
# 	for i in range(len(a)):
# 		for j in range(len(b[0])):
# 			s = 0
# 			ai = a[i]
# 			cj = c[j]
# 			for k in range(len(a[0])):
# 				s += ai[k] * cj[k]
# 			d[i][j] = s
# 	return d
	
# def main():
# 	n = 10
# 	if (len(sys.argv) > 1): n = int(sys.argv[1])
# 	n = int(float(n)/2) * 2 # FIXME: I am sure there are better ways to do this...
# 	tmp = 1. / n / n
# 	a = [[tmp * (i - j) * (i + j) for j in range(n)] for i in range(n)]
# 	b = [[tmp * (i - j) * (i + j) for j in range(n)] for i in range(n)]
# 	print(len(a[0]))
# 	d = matmul(a, b)
# 	print(d[int(n/2)][int(n/2)])

# if __name__ == '__main__': main()



f = open("ex.fbgc","w+")

for i in range(250000):
	f.write("for i = 1,2\n\ti \nend\n");

f.close()