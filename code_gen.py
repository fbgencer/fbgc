f = open('ex.fbgc','w')
#f = open('ex.py','w')

s = "var"

size = 50000

f.write(s+"=")
	
for i in range(0,size):
	f.write("var"+str(i)+"="+str(i))
	f.write(";\n")

#for i in range(0,size):
#	f.write("var=',"+"var"+str(i)+"\n")

