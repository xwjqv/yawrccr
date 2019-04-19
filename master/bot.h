#pragma once

#include <stdint.h>
#include <pthread.h>

struct bot{
	int tcp;
	int udp;

	int16_t speed;
	int16_t direction; //left positive, view from above

	int16_t accelerate;
	int16_t steer;

	pthread_t tid;	
};

extern int botCount;
extern struct bot **bots;


void *botTick(void *arg);
