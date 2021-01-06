#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERV_PORT 8085

void *connection_thread(void *);

// Global variables
int client_list[10];

int main(int argc, char *argv[])
{
	for (int i = 0; i < sizeof(client_list); ++i)
		client_list[i] = -1;

        // Create socket
        int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
                perror("Could not create socket");
		return -1;
	}
	
	// Prepare the sockaddr_in structure
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERV_PORT);
	
	// Bind
	if(bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Socket bind failed");
		return -1;
	}
	puts("Socket bind complete");
	
	// Listen
        listen(socket_desc, 5);
	puts("Waiting for incoming connections...");
	
	// Accept an incoming connection
	int new_socket;
	int sockaddr_size = sizeof(struct sockaddr_in);
	struct sockaddr_in client;
        while((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&sockaddr_size))) {
		puts("Connection accepted");
		
		// Placing new client into client_list
		for (int i = 0; i < sizeof(client_list); ++i) {
			if (client_list[i] == -1) {
				client_list[i] = new_socket;
				break;
			}
                }
		
		// Creating thread for client handler
		pthread_t thread_id;
		int *sock = (int *)malloc(sizeof(int));
		*sock = new_socket;
                if(pthread_create(&thread_id, NULL, connection_thread, sock) < 0) {
			perror("Could not create thread");
			return -1;
		}
		printf("Handler assigned to Client [%d]\n", new_socket);
	}
	
	if (new_socket < 0) {
		perror("Socket accept failed");
		return -1;
	}
	
	return 0;
}


// Handles connection for each client
void *connection_thread(void *socket_desc)
{
	// Get the socket descriptor
	int socket = *(int *)socket_desc;
	int read_size;
	char message[2000], client_message[1900], *terminate;
	terminate = "quit";

	// Server start
	while (1) {
		memset(client_message, 0, sizeof(client_message));
		printf("recieving message from client.. \n");
                read_size = recv(socket, client_message, 1, 0);
		printf("\nRecived size of bytes: %d\n", read_size);
		if (!read_size){
			break;
		}
		else if(strncmp(client_message, "0", strlen("0")) == 0){
			char login[128],password[128];
			memset(login, 0, sizeof(login));
			memset(password, 0, sizeof(password));			
			printf("recieving LOGIN from client.. \n");
                	recv(socket, login, sizeof(login), 0);
			printf("recieving PASSWORD from client.. \n");
			recv(socket, password, sizeof(password), 0);
			printf("\nlogin: %s\npassword:%s\n",login,password);
			if (strcmp(login, "hello") == 0 && strcmp(password, "world") == 0)
			{
				printf("\nTrue\n");
				send(socket, "1", strlen("1"),0);
            }
            else
			{
				printf("\nFalse\n");
				send(socket, "0", strlen("0"),0);
			}
		}
		else{
			printf("Client [%d] ", socket);
			printf("BULLSHIT KEY IS: %s\n", client_message);
		}
		
		/*
		// Action on receive
		printf("[Client %d]: %s \n", socket, client_message);
		printf("sending to client\n Please write smth: ");
		fgets(message, sizeof(message), stdin);
		send(socket, message, strlen(message), 0);
		*/

		/*
                // Broadcasting message to other clients
                sprintf(message, "[Client %d]: %s", socket, client_message);
		for (int i = 0; i < sizeof(client_list); ++i)
			if (client_list[i] != -1 && client_list[i] != socket)
				send(client_list[i], message, strlen(message), 0);
		*/
	}
	
	printf("Client [%d] disconnected\n", socket);
	
	// Remove client from client_list
	for (int i = 0; i < sizeof(client_list); ++i) {
		if (client_list[i] == socket) {
			client_list[i] = -1;
			break;
		}
	}
	
	fflush(stdout);
	free(socket_desc);
	
	return 0;
}
