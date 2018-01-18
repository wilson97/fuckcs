serial: serial.c serial.h 
	gcc -g -Wall -Werror -O3 serial.c -o serial  

parallel: parallel.c parallel.h 
	gcc -pthread -Wall -Werror -O3 parallel.c -o parallel 

simple_parallel: simple_parallel.c simple_parallel.h
	gcc -pthread -Wall -Werror -O3 simple_parallel.c -o simple_parallel 
