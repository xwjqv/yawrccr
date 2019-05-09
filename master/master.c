#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "key.h"
#include "bot.h"
#include "network.h"


int main(int argc, char *argv[])
{


	pthread_t tid;
	if(argc < 2){
		pthread_create(&tid, 0, botTick, conBot("raspberrypi"));
	}else{
		for(int i=1; i<argc; i++){
			pthread_create(&tid, 0, botTick, conBot(argv[i]));
		}
	}

	system("xset r off");
	key();
	system("xset r on");

}	
