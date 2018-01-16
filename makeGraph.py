import random

with open('2.txt', 'a') as f:
	f.truncate()
	f.write('1000\n')
	for x in range(0, 1000):
		print x
		for y in range(0, 1000):
			if (x == y):
				f.write('0 ')
			else:
				short = str(random.randint(0,10)) + ' '
				f.write(short)	
		f.write('\n')
				
