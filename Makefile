CC = gcc
CFLAGS  = -pthread

all: project

project: server.o client.o
	 $(CC) $(CFLAGS) -o server server.o
	 $(CC) $(CFLAGS) -o client client.o

server.o: server.c
	  $(CC) $(CFLAGS) -c server.c

client.o: client.c
	  $(CC) -c client.c

clean: 
	rm -rf *.o project
