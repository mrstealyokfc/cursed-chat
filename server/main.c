#include<stdio.h>
#include<unistd.h>

#include<sys/socket.h>
#include<netinet/in.h>


#include "server.h"
#include "config.h"

/*
struct sockaddr_in address;
int addrlen;

int create_server(int port){
	int server_fd;

	//create socket descriptor
	server_fd = socket(AF_INET,SOCK_STREAM,0);

	//setting up the socket i guess.
	int opt = 1;
	int fail = setsockopt(server_fd, SOL_SOCKET,
			   SO_REUSEADDR | SO_REUSEPORT,
			   &opt, sizeof(opt));

	if(fail<0)
		printf("failed at setsockopt\n");


	addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	fail = bind(server_fd, (struct sockaddr*)&address,
		 sizeof(address));

	if(fail<0)
		printf("failed to bind\n");

	fail = listen(server_fd,3);

	if(fail< 0)
		printf("failed to listen\n");

	return server_fd;

}
*/
int main(){
	printf("Program Started");
	
	server_s server = create_server(PORT);

	int client_fd = await_client(server);
	printf("%d\n",client_fd);

	int client_2 = await_client(server);
	printf("%d\n",client_2);

	close(client_2);
	close(client_fd);

	close_server(server);
}
