//creator: null
//date: III/VII/MMXXIII


#include "server.h"

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>

server_s create_server(int port){
    server_s server;

    server.addrlen = sizeof(server.address);
    server.fd = socket(AF_INET,SOCK_STREAM,0);

    server.address.sin_family=AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port=htons(port);


    int opt = 1;

    setsockopt(server.fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    if(bind(server.fd, (struct sockaddr*)&server.address, sizeof(server.address)) <  0){
        printf("failed to bind server to port: %d",port);
        server.fd = 0;
        return server;
    }

    listen(server.fd, 3);

    return server;
}
int await_client(server_s server){
    return accept(server.fd, (struct sockaddr*)&server.address,(socklen_t*)&server.addrlen);
}

void close_server(server_s server){
    shutdown(server.fd, SHUT_RDWR);
}
