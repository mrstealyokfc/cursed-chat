//creator: null
//date: III/VII/MMXXIII
#ifndef __SERVER_H
#define __SERVER_H

#include<sys/socket.h>
#include<netinet/in.h>

#include "settings.h"

typedef struct {
    int fd;
    struct sockaddr_in address;
    int addrlen;
} server_s;

server_s create_server(int port);

int await_client(server_s server);

void close_server(server_s server);

#endif
