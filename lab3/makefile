all:
	gcc client.c -o client.o
	gcc server.c -o server.o
thread:
	gcc client.c -o client.o
	gcc server.c -DTHREAD -o server.o -lpthread
process:
	gcc client.c -o client.o
	gcc server.c -DPROCESS -o server.o -lpthread
clean:
	rm -rf *.o
	rm output.txt
