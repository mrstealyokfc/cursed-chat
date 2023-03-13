//creator: null
//date: III/VII/MMXXIII

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<stdint.h>
#include<signal.h>

#include<sys/socket.h>
#include<netinet/in.h>

#include "clients.h"
#include "server.h"
#include "config.h"
#include "commands.h"
#include "message_queue.h"

void* client_handler(void* client_vp){
	client_s* client = (client_s*)client_vp;

	set_name(client,NULL);

	char msg_buf[MESSAGE_LENGTH+2] = {0};

	pthread_mutex_init(&client->message_buffer_lock,NULL);

	int read_len;
	while(client->sockfd){
		int name_len = strlen(client->name);

		pthread_mutex_lock(&client->message_buffer_lock);

		//FIXME: sometimes  blocks here when running test_server.py
		read_len = read(client->sockfd, msg_buf+name_len+3, MESSAGE_LENGTH-(name_len+3));

		if(read_len <= 0 || client->sockfd == 0)
			break;

		if(msg_buf[name_len+3] == '/'){
			process_command(client, msg_buf+name_len+3,read_len);
			continue;
		}

		memcpy(msg_buf,client->name,name_len);
		memcpy(msg_buf+name_len," | ",3);

		memset(msg_buf+read_len+name_len+3,0,2);
		if(*(msg_buf+name_len+2+read_len) != '\n')
			*(msg_buf+name_len+3+read_len) = '\n';

		broadcast_message(msg_buf,&client->message_buffer_lock);
		printf("broadcast %s\n",msg_buf);

	}

	release_client(client, "disconnected due to error!\n");

	pthread_mutex_destroy(&client->message_buffer_lock);

	return NULL;
}



void add_new_client(int client_fd){
	for(uint64_t i=0;i<MAX_CLIENTS;i++){
		if(clients[i].sockfd == 0){
			clients[i].sockfd = client_fd;

			pthread_create(&clients[i].reciever_thread,NULL,&client_handler,&clients[i]);

			return;
		}
	}

	const char* message = SERVER_FULL_MESSAGE;

	send(client_fd,message,strlen(message),0);
	close(client_fd);

}

void* add_new_clients_t(void* server_ptr){
	server_s server = *(server_s*)server_ptr;
	while(1){
		int new_client = await_client(server);
		if(new_client < 0){
			printf("Bad Client Connection");
			break;
		}
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
				release_client(&clients[i],SERVER_CLOSE_MESSAGE);
			printf("Server Has Been Closed\n");
			break;
		}
	}
}

void start_listen_thread(server_s server){
	pthread_t ptid;
	pthread_create(&ptid,NULL,&add_new_clients_t,&server);
}

void init(){
	signal(SIGPIPE, SIG_IGN);
	init_client_data();
	start_message_queue();
}

int main(){
	init();

	server_s server = create_server(PORT);

	if(server.fd <= 0){
		printf("Failed To Start Server\n");
		return -1;
	}

	printf("Listening on Port:%d\n",PORT);

	start_listen_thread(server);
	cli_control(server);
}
