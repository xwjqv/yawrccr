#include <stddef.h>

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
		if((bot->speed < 32000 && bot->accelerate > 32000) || (bot->speed > 32000 && bot->accelerate < 32000))
			bot->speed += bot->accelerate * mul;
		else if ((bot->speed -= bot->speed >> 5) < 1 << 5)
			bot->speed = 0;
	
		if((bot->direction < 32000 && bot->steer > 32000) || (bot->direction > 32000 && bot->steer < 32000))
			bot->direction += bot->steer * mul;
		else if ((bot->direction -= bot->direction >> 5) < 1 << 5)
			bot->direction = 0;

		sendParam(bot);
	}while(1);
}
