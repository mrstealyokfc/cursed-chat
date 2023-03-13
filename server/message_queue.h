#ifndef __MESSAGE_QUEUE_H
#define __MESSAGE_QUEUE_H

#include "clients.h"

typedef struct{
    pthread_mutex_t* lock ;
    client_s* dest;
    char* message;
} message_s;

void add_message(message_s message);

void send_message(client_s* dest, char* message,pthread_mutex_t* lock);

void broadcast_message(char* message,pthread_mutex_t* lock);

void start_message_queue();

#endif // __MESSAGE_QUEUE_H
