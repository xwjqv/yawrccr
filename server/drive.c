#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

int pinLf = 21; 
int pinRf = 22;
int pinLb = 23;
int pinRb = 24; 

int init_drive()
{
	if(geteuid()==0) wiringPiSetup();
	else wiringPiSetupSys();

	pinMode(pinLf,OUTPUT);
	pinMode(pinRf,OUTPUT);
	pinMode(pinLb,OUTPUT);
	pinMode(pinRb,OUTPUT);

	printf("drive init complete\n");
	return 0;
}

int drive(char mode)
{
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
	return 0;
}
