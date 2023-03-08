//creator: null
//date: III/VII/MMXXIII


#ifndef __CONFIG_H
#define __CONFIG_H

#define MESSAGE_LENGTH 4092
#define PORT 1982
#define MAX_CLIENTS 4
#define SERVER_CLOSE_MESSAGE "Server is going DOWN!!!"
#define SERVER_FULL_MESSAGE "Server is full, you cannot join."

typedef struct{
    int sockfd;
    char name[16];
} client_s;

#endif
