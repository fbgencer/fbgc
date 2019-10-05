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


def partition(xs, start, end):
	follower = leader = start
	while leader < end:
		if xs[leader] <= xs[end]:
			xs[follower], xs[leader] = xs[leader], xs[follower]
			follower += 1
		leader += 1
	xs[follower], xs[end] = xs[end], xs[follower]
	return (follower)

def _quicksort(xs, start, end):
	if start >= end:
		return

	p = partition(xs, start, end)
	_quicksort(xs, start, p-1)
	_quicksort(xs, p+1, end)
	

def quicksort(xs):
	_quicksort(xs, 0, len(xs)-1)



xs = [1,5,100,3,2,-1]
quicksort(xs)
print(xs)

import dis

def foo(a,b,c):
	a = b
	c = a
	return c


def lol():
	tp = (5,10,20,30,40)
	for i in tp:
		print(i);

def zo():
	foo(1,2,3)

dis.dis(foo)
print("=============")
dis.dis(zo)