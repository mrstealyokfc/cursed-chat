#include "message_queue.h"

#include<stdint.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<stdio.h>

#include "config.h"
#include "clients.h"

message_s message_queue[256];

pthread_mutex_t queue_lock;

uint8_t next_read=0;
uint8_t current_size=0;

/* FIXME:
 *
 * if someone sends 2 messages fast enough the char[] that
 * message_s.message points to could be changed before the
 * content of the message is sent.
 */
void add_message(message_s message){
    pthread_mutex_lock(&queue_lock);

    message_queue[current_size] = message;
    current_size++;

    pthread_mutex_unlock(&queue_lock);
}

void send_message(client_s* dest, char* message,pthread_mutex_t* lock){
    message_s msg = {.dest = dest, .message = message, .lock = lock};
    add_message(msg);
}

void broadcast_message(char* message, pthread_mutex_t* lock){
    send_message(NULL,message,lock);
}

void* sender_thread(void* args){
    printf("initialized sender thread\n");

    while(1){

        if(next_read == current_size){
            usleep(1000);
            continue;
        }
        message_s msg = message_queue[next_read];
        if(msg.dest != NULL){
            send(msg.dest->sockfd,msg.message,strlen(msg.message),0);
            goto end;
        }

        for(int i=0;i<MAX_CLIENTS;i++){
            if(clients[i].sockfd == 0)
                continue;
            send(clients[i].sockfd,msg.message,strlen(msg.message),0);
        }

        end:
        if(msg.lock != NULL)
            pthread_mutex_unlock(msg.lock);
        next_read++;
    }
}



void start_message_queue(){
    pthread_mutex_init(&queue_lock,NULL);
    pthread_t tid;
    pthread_create(&tid,NULL,&sender_thread,NULL);
}
