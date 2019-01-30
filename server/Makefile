CC=gcc
CFLAGS= -o server -lwiringPi

OBJ=drive.o network.o streamer.o

server : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ)

$(OBJ) : server.h
