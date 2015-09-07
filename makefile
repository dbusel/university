main: main.c
	gcc -o main main.c -I.
clean: rm -f *.o main