#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "bot.h"
#include "network.h"

int botCount = 0;
struct bot **bots = NULL;

int mul = 20;

void *botTick(void *arg)
{
	struct bot *bot = arg;
	if(bot == NULL)
		return (void*)-1;
	bot->tid = pthread_self();

	do{
		if((bot->speed < -32000 && bot->accelerate > 0) || (bot->speed > 32000 && bot->accelerate < 0) || (bot->speed < 32000 && bot->speed > -32000 && bot->accelerate != 0))
			bot->speed += bot->accelerate * mul;
		else if (abs(bot->speed -= bot->speed >> 5) < 1 << 5)
			bot->speed = 0;
	
		if((bot->direction < -32000 && bot->steer > 0) || (bot->direction > 32000 && bot->steer < 0) || (bot->direction < 32000 && bot->direction > -32000 && bot->steer !=0))
			bot->direction += bot->steer * mul;
		else if (abs(bot->direction -= bot->direction >> 5) < 1 << 5)
			bot->direction = 0;

		/*
		bot->speed = bot->accelerate * mul;
		bot->direction = bot->steer * mul;
		*/	

		sendParam(bot);
		printf("speed: %d\ndirection: %d\n", bot->speed, bot->direction);
		printf("accelerate: %d\nsteer: %d\n", bot->accelerate, bot->steer);
		nanosleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 10000000},NULL);
	}while(1);
}
