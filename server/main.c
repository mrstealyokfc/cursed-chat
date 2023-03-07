//creator: null
//date: III/VII/MMXXIII

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<stdint.h>

#include<sys/socket.h>
#include<netinet/in.h>


#include "server.h"
#include "config.h"

typedef struct {
	int sockfd;
	char* name;
} client_s;

client_s clients[MAX_CLIENTS];

void* handle_client_connection_t (void* client_fd_ptr);

void release_client(int client_index, char* str_message){
	send(clients[client_index].sockfd,str_message,strlen(str_message),0);
	close(clients[client_index].sockfd);
	clients[client_index].sockfd=0;
}

void add_new_client(int client_fd){
	for(uint64_t i=0;i<MAX_CLIENTS;i++){
		if(clients[i].sockfd == 0){
			clients[i].sockfd = client_fd;
			pthread_t ptid;
			pthread_create(&ptid,NULL,&handle_client_connection_t,(void*) i);
			return;
		}
	}

	const char* message = SERVER_FULL_MESSAGE;

	send(client_fd,message,strlen(message),0);
	close(client_fd);

}



void* handle_client_connection_t (void* client_fd_ptr){
	uint64_t index = (uint64_t)client_fd_ptr;

	char message_buffer[MESSAGE_LENGTH+1];
	memset(message_buffer,0,MESSAGE_LENGTH+1);

	int read_len;
	while(1){

		read_len = read(clients[index].sockfd, message_buffer, MESSAGE_LENGTH);

		for(int i=0;i<MAX_CLIENTS;i++)
			if(clients[i].sockfd !=0)
				send(clients[i].sockfd,message_buffer,read_len,0);

		if(read_len == 0 || clients[index].sockfd == 0){
			release_client(index,"Error Has Occoured, you have been disconnected");
			break;
		}

	}
	return NULL;
}

void* add_new_clients_t(void* server_ptr){
	server_s server = *(server_s*)server_ptr;
	while(1){
		int new_client = await_client(server);
		if(new_client < 0)
			break;
		add_new_client(new_client);
	}
	return NULL;
}

void cli_control(server_s server){

	char input[256];

	while(1){
		memset(input,0,256);

		fgets(input,255,stdin);

		if(strcmp(input,"stop\n") == 0){
			close_server(server);
			for(int i=0;i<MAX_CLIENTS;i++)
				release_client(i,SERVER_CLOSE_MESSAGE);
			printf("Server Has Been Closed\n");
			break;
		}
	}
}

void start_listen_thread(server_s server){
	pthread_t ptid;
	pthread_create(&ptid,NULL,&add_new_clients_t,&server);
}

int main(){
	printf("Server Started\n");
	
	server_s server = create_server(PORT);

	start_listen_thread(server);

	cli_control(server);
}
