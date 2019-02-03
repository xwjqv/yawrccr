CC=gcc
CFLAGS= -o server -lwiringPi -Wall

OBJ=drive.o network.o streamer.o

server : server.c $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) server.c

$(OBJ) : server.h
