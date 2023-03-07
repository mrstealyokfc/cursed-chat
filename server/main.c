#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<stdint.h>

#include<sys/socket.h>
#include<netinet/in.h>


#include "server.h"
#include "config.h"

int clients[MAX_CLIENTS];


void add_new_client(int client_fd){

}

void* handle_client_connection_t (void* client_fd_ptr){
	uint64_t index = (uint64_t)client_fd_ptr;

	char message_buffer[MESSAGE_LENGTH+1];
	memset(message_buffer,0,MESSAGE_LENGTH+1);
	int read_len;
	while(1){
		read_len = read(clients[index], message_buffer, MESSAGE_LENGTH);
		printf(message_buffer);
		if(read_len == 0 || clients[index] == 0){
			clients[index]=0;
			close(clients[index]);
			break;
		}
	}
	return NULL;
}

void add_new_clients_t(server_s server){
	while(1){
		int new_client = await_client(server);
		if(new_client < 0)
			break;
		add_new_client(new_client);
	}
}

void* control_thread(void* args){

	server_s server = *((server_s*)args);
	char input[256];

	while(1){
		memset(input,0,256);

		fgets(input,255,stdin);

		if(strcmp(input,"stop\n") == 0){
			close_server(server);
			const char* cm = SERVER_CLOSE_MESSAGE;
			for(int i=0;i<MAX_CLIENTS;i++){
				send(clients[i],cm,strlen(cm),0);
				close(clients[i]);
			}
			memset(clients,0,MAX_CLIENTS*4);
			break;
		}
	}
	return NULL;
}

void start_control_thread(server_s server){
	pthread_t ptid;
	pthread_create(&ptid,NULL,&control_thread,&server);
}

int main(){
	printf("Program Started");
	
	server_s server = create_server(PORT);

	start_control_thread(server);

	int client_fd = await_client(server);

	clients[0]=client_fd;
	handle_client_connection_t(0);

	//close_server(server);

}
