import random
import sys
size = sys.argv[1];
open("tests/" + size + '.txt', 'w').close()

with open("tests/" + size + '.txt', 'a') as f:
	f.truncate()
	f.write(size + '\n')
	for x in range(0, int(size)):
		print x
		for y in range(0, int(size)):
			if (x == y):
				if (x == int(size) - 1):
					f.write('0')
				else:
					f.write('0 ')
			else:
                                if (y == int(size) - 1):
					f.write(str(random.randint(1,10)))
				else:
					short = str(random.randint(1,10)) + ' '
					f.write(short)	
		f.write('\n')
				
