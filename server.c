#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1337

typedef struct sockaddr_in socket_address;

int main(){
	socket_address address;
	int server_fd, new_socket, addrlen = sizeof(address);

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
	
	while(1){
		printf("\n----- Esperando Conexão -----\n");

		// Retira a primeira requisição da fila, cria um novo socket e retorna um novo socket descriptor (new_socket)
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

		// Le o que o que o cliente escreve
		char buffer[30000] = {0};
		read(new_socket , buffer, 30000);
		printf("%s\n",buffer );

		// Escreve para o cliente
		char* hello = "Ola, eu sou o servidor";
		write(new_socket , hello , strlen(hello));

		printf("\n----- Mensagem do servidor enviada -----\n");

		close(new_socket);
	}

	return 0;
}

