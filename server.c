#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>

#define PORT 7331

typedef struct sockaddr_in socket_address;

void* communation_thread(void *client_sock){
	
	// Voltando o socker descriptor do cliente para inteiro
    int client_socket = *(int*)client_sock;
	char buffer[30000] = {0};

	// Le o input do cliente até serem enviados 0 bytes
	while(read(client_socket , buffer, 30000)){
		printf("%s\n",buffer );

		// Escreve para o cliente
		char hello[30] = "Mensagem Recebida\n\n";
		write(client_socket , hello , strlen(hello));
	}
	printf("\n----- Conexão encerrada -----\n");

	close(client_socket);
}

int main(){
	socket_address address;
	int server_fd, client_socket, addrlen = sizeof(address);

	// Socket descriptor, inteiro que a aplicação usa sempre que quer se referir a este socket	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	// AF_INET: IPv4
	// INADDR_ANY: Para todas as interfaces de rede disponíveis
	// PORT: 1337
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = htonl(INADDR_ANY); 
	address.sin_port = htons(PORT); 

	// Vincula o socket ao localhost e a porta 1337
	bind(server_fd,(const struct sockaddr *)&address,sizeof(address));

	// Coloca o socket do servidor em modo passivo, ou seja, espera pelo cliente estabelecer uma conexão
	// Segundo parametro (backlog): Números máximo de conexões pendentes na fila
	listen(server_fd, 10);
	pthread_t thread_id;
	printf("\n----- Esperando Conexão -----\n");
	
	while(1){

		// Retira a primeira requisição da fila, cria um novo socket e retorna um novo socket descriptor (new_socket)
		client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		printf("\n----- Conexão Estabelecida -----\n");

		pthread_create( &thread_id , NULL , communation_thread , (void*) &client_socket);
		//pthread_join(thread_id , NULL);
	}

	return 0;
}

