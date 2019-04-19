#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "key.h"
#include "bot.h"
#include "network.h"


int main(void)
{

	system("xset r off");

	pthread_t tid;
	pthread_create(&tid, 0 ,botTick, conBot("raspberrypi"));

	key();

	system("xset r on");

}	
