#include "network.h"
#include "drive.h"
#include "streamer.h"

#define _GNU_SOURCE

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define maxsock  10

static struct pollfd fds[maxsock];

struct sockaddr_in clien_addr[maxsock-2];
socklen_t client_addr_len = sizeof(clien_addr[0]);
char cmd[maxsock][256];
int cmd_len[maxsock] ={0};
char buff[256];
int tcpcon = 0;

void init_network(int tcpport, int udpport)
{

	//udp
	fds[0].fd = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in udpaddr;
	udpaddr.sin_family = AF_INET;
	udpaddr.sin_port = htons(8555);
	udpaddr.sin_addr.s_addr = INADDR_ANY;	

	if(bind(fds[0].fd,(const struct sockaddr*)&udpaddr,sizeof(udpaddr)))
		perror("bind udp");

	//tcp
	fds[1].fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8554);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	if(bind(fds[1].fd, (const struct sockaddr*)&servaddr, sizeof(servaddr))) 
		perror("bind tcp");

	listen(fds[1].fd,4);


	for(int i = 2; i < maxsock; i++)
		fds[i].fd = -1;

	for(int i=0; i< maxsock; i++)
		fds[i].events = POLLIN | POLLRDHUP;

	printf("network init complete\n");
}

void handle_connection()
{
	poll(fds,maxsock,0);

	//udp commands
	if(fds[0].revents==POLLIN){
		memset(buff,0,sizeof(buff));
		recv(fds[0].fd, buff, sizeof(buff),0 );
		if(buff[0] == 'b' && buff[1] == 'i' && buff[2] == 'n' && buff[3] == '!') //speed and direction
			pdrive(buff+4);
		else
			drive(buff[0]);
	}

	//new connection handling
	if(fds[1].revents==POLLIN){
		for(int i=2;i<=maxsock;i++){
			if(fds[i].fd==-1){
				fds[i].fd = accept(fds[1].fd, (struct sockaddr *) &clien_addr[i], &client_addr_len);
				if(fds[i].fd < 0){
					perror("accept tcp");
				}else{
					tcpcon++;
					if(tcpcon<=1)
						streamer(clien_addr[i].sin_addr, (uint)5000);
					printf("connected to: %s\n",inet_ntoa(clien_addr[i].sin_addr));
				}
			}else if(i==maxsock)
				printf("socketlimit reached\n");
		}
	}
			
	//old connections
	for(int i=2;i<maxsock;i++){
		//errors
		if(0 != (fds[i].revents &(POLLRDHUP|POLLERR|POLLHUP))){
			printf("connection lost to: %s\n",inet_ntoa(clien_addr[i].sin_addr));
			close(fds[i].fd);
			fds[i].fd = -1;
			tcpcon--;
			if(tcpcon<=0)
				system("killall gst-launch-1.0\n");
		}

		//new input
		if(fds[i].revents==POLLIN){
			memset(buff,0,sizeof(buff));
			recv(fds[i].fd, buff, sizeof(buff),0 );
			for(int j=0;j<sizeof(buff);j++){
				if(buff[i]=='\0'){
					break;
				}else if(buff[j]==';'){
					cmd[i][cmd_len[i]]=buff[j];
					//interpretcmd();
					cmd_len[i]=0;
					break;
				}else{
					cmd[i][cmd_len[i]]=buff[j];
					cmd_len[i]++;
				}
			}
		}
	}
}	
