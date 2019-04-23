#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "network.h"
#include "bot.h"
#include "../packet/packet.h"



struct bot *conBot(char *hostc)
{
	printf("connecting to:%s\n",hostc);
	struct hostent *host = gethostbyname(hostc);
	if(host == 0){
		printf("host not obtainable\n");
		return NULL;
	}
	printf("%x",*(host->h_addr_list[0]));
	struct in_addr addr;
	addr = *(struct in_addr*)(host->h_addr_list[0]);

	printf("got host\n");

	if(botCount == 0)
		bots = malloc(sizeof(*bots));
	else
		bots = realloc(bots, (botCount+1) * sizeof(*bots));

	bots[botCount] = malloc(sizeof(struct bot));

	bots[botCount]->tcp = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in tcpaddr;
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(8554);
	tcpaddr.sin_addr = addr;
	if(connect(bots[botCount]->tcp,(const struct sockaddr*)&tcpaddr,sizeof(tcpaddr)))
		perror("conBot tcp");
	
	bots[botCount]->udp = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in udpaddr;
	udpaddr.sin_family = AF_INET;
	udpaddr.sin_port = htons(8555);
	udpaddr.sin_addr = addr;
	if(connect(bots[botCount]->udp,(const struct sockaddr*)&udpaddr,sizeof(udpaddr)))
		perror("conBot udp");

	botCount++;

	printf("connected bot\n");
	return bots[botCount-1];
}

void sendParam(struct bot *bot)
{
	struct packet pac;

	pac.speed = htole16(bot->speed);
	pac.direction = htole16(bot->direction);

	void *buff = malloc(4+sizeof(pac));

	sprintf(buff,"bin!");
	memcpy(buff+4,&pac,sizeof(pac));
	if(send(bot->udp, buff, 4+sizeof(pac), 0) == -1)
		perror("sendPraram");

	free(buff);
}	
