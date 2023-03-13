#ifndef __CLIENTS__H
#define __CLIENTS__H

#include "config.h"

#include<pthread.h>



typedef struct {
    int sockfd;
    pthread_t reciever_thread;
    pthread_mutex_t message_buffer_lock;
    char name[16];
} client_s;

extern client_s clients[MAX_CLIENTS];

void set_name(client_s* client, char* str);

void init_client_data();

void release_client(client_s* client, char* str_message);

client_s* get_empty_client_address();

#endif // __CLIENTS__H
