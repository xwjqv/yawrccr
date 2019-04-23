#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <endian.h>


#if defined PIGPIO
#include <pigpio.h>
#elif defined WIRING
#include <wiringPi.h>
#endif

#include "../packet/packet.h"

int pinLf = 5; 
int pinRf = 6;
int pinLb = 13;
int pinRb = 12; 

int freq = 2500;

int init_drive()
{
#if defined PIGPIO
	if(gpioInitialise()< 0)
		printf("Pigpio init failed: not root or already running");
	else
		printf("initilised Pigpio");
	
#elif defined WIRING
	if(geteuid()==0)
		wiringPiSetupGpio();
	else
		wiringPiSetupSys();

	pinMode(pinLf,OUTPUT);
	pinMode(pinRf,OUTPUT);
	pinMode(pinLb,OUTPUT);
	pinMode(pinRb,OUTPUT);
#endif

	printf("drive init complete\n");
	return 0;
}

int pdrive(void *pac)
{	
	struct packet *move = pac;
	
	int speed = (int)le16toh(move->speed) << 5;
	int direction = le16toh(move->direction);
	

	int slow = (int)(speed * cos(2*M_PI* (float)direction/(float)(1<<14) )); //slow wheel while turning
	
#if defined PIGPIO
	if(direction > 0){//left
		gpioHardwarePWM(pinLb, freq, slow>0 ? 1000000-abs(slow) : abs(slow));
		gpioWrite(pinLf, slow > 0 ? 1 : 0);

		gpioHardwarePWM(pinRb, freq, speed>0 ? 1000000-abs(speed) : abs(speed));
		gpioWrite(pinRf, speed > 0 ? 1 : 0);
	}else{//right
		gpioHardwarePWM(pinRb, freq, slow>0 ? 1000000-abs(slow) : abs(slow));
		gpioWrite(pinRf, slow > 0 ? 1 : 0);

		gpioHardwarePWM(pinLb, freq, speed>0 ? 1000000-abs(speed) : abs(speed));
		gpioWrite(pinLf, speed > 0 ? 1 : 0);
	}
#endif

	return 0;
}
	

int drive(char mode)
{
	
#if defined WIRING
	digitalWrite(pinLf,0);
	digitalWrite(pinRf,0);
	digitalWrite(pinLb,0);
	digitalWrite(pinRb,0);
	switch(mode){
		case 'L': 
			digitalWrite(pinRf,1);
			break; 
		case 'R': 
			digitalWrite(pinLf,1);
			break; 
		case 'F': 
			digitalWrite(pinRf,1);
			digitalWrite(pinLf,1);
			break; 
		case 'B': 
			digitalWrite(pinRb,1);
			digitalWrite(pinLb,1);
			break; 
	}
#endif
	printf("drive: %d\n",mode);
	return 0;
}
