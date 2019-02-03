#include "network.h"
#include "drive.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("started\n");
	init_drive();

	init_network(8554, 8555);

	while(1){
		handle_connection();
	}

	//if(kill(child,15) == -1) perror("kill");
	//if(wait(0) == -1) perror("wait");
	//system("killall gst-launch-1.0");

}
