#include "clients.h"

#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>

client_s clients[MAX_CLIENTS];

void reset_name(int index){
	memcpy(clients[index].name,"anon",4);
	memset(clients[index].name+4,0,12);
}

void release_client(client_s* client, char* str_message){
	send(client->sockfd,str_message,strlen(str_message),0);
	close(client->sockfd);
	client->sockfd=0;
}

void send_to_all_clients(char* message, int length){
	for(int i=0;i<MAX_CLIENTS;i++)
		if(clients[i].sockfd != 0)
			send(clients[i].sockfd,message,length,0);
}

void prep_client_data(){
	for(int i=0;i<MAX_CLIENTS;i++){
		clients[i].sockfd = 0;
		reset_name(i);
	}
}
