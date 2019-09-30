f = open('mos.fbgc','w')
#f = open('ex.py','w')


size =1000000

for i in range(size):
	st = ("x%d"%i) + (" = %d"%i) + "\n" 
	st += "print(x%d)" % i + "\n";
	f.write(st);

