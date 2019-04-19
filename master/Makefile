CC=gcc
CFLAGS= -lX11 -pthread -Wall -Wpadded

master: master.c key.o bot.o network.o
	$(CC) $(CFLAGS) $^

*.o: *.c
