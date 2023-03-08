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
#include "commands.h"

client_s clients[MAX_CLIENTS];

void* handle_client_connection_t (void* client_fd_ptr);

void reset_name(int index){
	memcpy(clients[index].name,"anon",4);
	memset(clients[index].name+4,0,12);
	printf("%s",clients[index].name);
}

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

void send_to_all_clients(char* message, int length){
	for(int i=0;i<MAX_CLIENTS;i++)
		if(clients[i].sockfd != 0)
			send(clients[i].sockfd,message,length,0);
}

void* handle_client_connection_t (void* client_index_notapointer){

	uint64_t index = (uint64_t)client_index_notapointer;
	client_s* client = &clients[index];

	char msg_buf[MESSAGE_LENGTH+1];
	int msg_len;

	memset(msg_buf,0,MESSAGE_LENGTH+1);

	while(client->sockfd){

		int name_len = strlen(client->name);
		msg_len = read(client->sockfd, msg_buf+name_len+3, MESSAGE_LENGTH-(name_len+3));
		printf("message recieved\n");
		if(msg_len == 0)
			break;

		if(msg_buf[name_len+3] == '/')
			process_command(client, msg_buf);

		else{
			memcpy(msg_buf,client->name,name_len);
			memcpy(msg_buf+name_len," | ",3);
			send_to_all_clients(msg_buf,msg_len+name_len+3);
		}

	}

	release_client(index, "disconnected due to error!\n");

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

void prep_client_data(){
	for(int i=0;i<MAX_CLIENTS;i++){
		clients[i].sockfd = 0;
		reset_name(i);
	}
}

int main(){

	prep_client_data();

	printf("Server Started\n");
	
	server_s server = create_server(PORT);

	start_listen_thread(server);

	cli_control(server);
}
