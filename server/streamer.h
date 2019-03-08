#define _GNU_SOURCE

#include <netinet/in.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>

pid_t streamer(struct in_addr host , uint port);
