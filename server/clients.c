#include "clients.h"
#include<pthread.h>
#include<unistd.h> //send() close()
#include<string.h> //memset() memcpy() memcmp()
#include<sys/socket.h>
#include<stdbool.h>

#include "utils.h" // min()

client_s clients[MAX_CLIENTS];


void set_name(client_s* client,char* str){
	memset(client->name,0,16);
	if(str != NULL)
		memcpy(client->name,str,min(strlen(str),15));
	else
		memcpy(client->name,DEFAULT_USERNAME,min(strlen(DEFAULT_USERNAME),15));
}

void release_client(client_s* client, char* str_message){

	send(client->sockfd,str_message,strlen(str_message),0);
	close(client->sockfd);

	client->sockfd=0;

}

bool is_name_taken(char* name){
	for(int i=0;i<MAX_CLIENTS;i++)
		if(memcmp(name,clients[i].name,15) == 0)
			return true;
	return false;
}

void init_client_data(){
	memset(clients,0,sizeof(client_s)*MAX_CLIENTS);
}
