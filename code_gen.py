f = open('ex.fbgc','w')
#f = open('ex.py','w')

s = "var"

size = 500

f.write(s+"=")
	
for i in range(0,size):
	f.write(str(i))
	f.write('+\n')

f.write(";\n")

#for i in range(0,size):
#	f.write("print('{var:',"+"var"+str(i)+")\n")

