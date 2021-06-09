#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "../config.h"

// Socket struct
typedef struct sockaddr_in socket_address;

// A função lê o arquivo de pacote (1 KB) em pacote e o envia para o cliente
void send_file(FILE* fp, int client_socket, int last_packet_size, int n_packets){
	char buffer[PACKET_SIZE];
	memset(buffer, 0, PACKET_SIZE);

	if (last_packet_size != 0){
		n_packets--;
	}

	for(int i = 0 ; i < n_packets; i++){
    	fread(buffer, 1, PACKET_SIZE, fp);
    	write(client_socket , buffer , PACKET_SIZE);
  	}

  	fread(buffer, 1, last_packet_size, fp);
  	write(client_socket , buffer , last_packet_size);

  	fclose(fp);
}

void* communication_thread(void *client_sock){

	// Voltando o socker descriptor do cliente para inteiro
        int client_socket = *(int*)client_sock, file_size, n_packets, last_packet_size;
	char filename[FILENAME_SIZE];
	char comando[10];
	char full_path[60] ="";
	char path[60] = "./server_/files/";
	memset(filename, 0, FILENAME_SIZE);
        char buffer_in[PACKET_SIZE+1];

	FILE* fp;

        // Interage com o cliente até que o comando exit seja enviado
	while(read(client_socket, comando, 10)){
		// Comando get (baixar o arquivo)
		if (!strcmp(comando, "get")) {
			printf("Comando get.\n");
			read(client_socket, filename, FILENAME_SIZE);

			strcat (full_path, path);
			strcat (full_path, filename);

			fp = fopen(full_path, "rb");
			if (fp == NULL){
			    printf("\n----- Arquivo não encontrado -----\n\n");
			    n_packets = -1;
			    write(client_socket, &n_packets, sizeof(int));
			} else {
				fseek(fp, 0, SEEK_END);
				file_size = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				last_packet_size = file_size%PACKET_SIZE;
				printf("Tamanho do Arquivo: %d\n", file_size);
				n_packets = last_packet_size ? file_size/PACKET_SIZE+1 : file_size/PACKET_SIZE;
				printf("Numero de pacotes a ser enviado: %d\n", n_packets);

				write(client_socket, &n_packets, sizeof(int));
				send_file(fp, client_socket, last_packet_size, n_packets);

			}
		// Comando put (fazer upload do arquivo)
		} else if (!strcmp (comando, "put")) {
			printf("Comando put.\n");
			read(client_socket, filename, FILENAME_SIZE);

			read(client_socket, &n_packets, sizeof(int));
			strcat (full_path, path);
			strcat (full_path, filename);
			fp = fopen (full_path, "wb");

			printf("Numero de pacotes a receber: %d\n", n_packets);

			int packet_count = 0;
			int pkt_size;
			for (int i = 0; i < n_packets; i++){
			    pkt_size = read(client_socket, buffer_in, PACKET_SIZE);
			    fwrite(buffer_in, 1, pkt_size,fp);
			    packet_count++;
			}
			printf("Numero de pacotes: %d\n", packet_count);
			printf("\n----- Arquivo Recebido -----\n\n");
			fclose(fp);
		} else if (!strcmp(filename, "exit")) break;
		memset(full_path, 0, 60);	
		memset(filename, 0, FILENAME_SIZE);
	}
	printf("\n----- Conexão encerrada -----\n");
	close(client_socket);
	return NULL;
}

int main(){
	socket_address address;
	int server_fd, client_socket, addrlen = sizeof(address);

	// Socket descriptor, inteiro que a aplicação usa sempre que quer se referir a este socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	// AF_INET: IPv4
        // INADDR_LOOPBACK: Para a interface de loopback
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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

		pthread_create( &thread_id , NULL , communication_thread , (void*) &client_socket);
	}

	return 0;
}
