from random import *

f = open("ex.fbgc",'w')

alph = list(map(chr, range(97, 123)))

no = 10000
for i in range(0,no):
	x = alph[randint(0,len(alph)-1)]
	y = alph[randint(0,len(alph)-1)]
	z = alph[randint(0,len(alph)-1)]
	f.write("x = '"+x+y+z+str(i)+str(randint(0,15000))+"'\n")
	f.write('print(x)\n')



f.close()