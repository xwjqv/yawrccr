CC=gcc
CFLAGS= -Wall -lm 

OBJ=drive.o network.o streamer.o

uname_p := $(shell uname -m)


ifeq ($(uname_p),armv6l)
CFLAGS += -pthread -lpigpio -lrt -DPIGPIO
endif

server : server.c $(OBJ)
	$(CC) -o server $(CFLAGS) $(OBJ) server.c

$(OBJ) : server.h
