#include "clients.h"

#include<unistd.h>
#include<string.h>
#include<sys/socket.h>

void reset_name(int index){
	memcpy(clients[index].name,"anon",4);
	memset(clients[index].name+4,0,12);
}

void release_client(int client_index, char* str_message){
	send(clients[client_index].sockfd,str_message,strlen(str_message),0);
	close(clients[client_index].sockfd);
	clients[client_index].sockfd=0;
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
