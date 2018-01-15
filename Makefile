serial: serial.c serial.h
	gcc -Wall -Werror -O3 serial.c -o serial  

parallel: parallel.c parallel.h
	gcc -pthread -Wall -Werror -O3 parallel.c -o parallel 
