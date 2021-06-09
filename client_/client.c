#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "../config.h"

typedef struct sockaddr_in socket_address;

void send_file(FILE* fp, int client_fd, int last_packet_size, int n_packets){
    char buffer[PACKET_SIZE];
    memset(buffer, 0, PACKET_SIZE);

    if (last_packet_size != 0){
        n_packets--;
    }

    for(int i = 0 ; i < n_packets; i++){
        fread(buffer, 1, PACKET_SIZE, fp);
        write(client_fd , buffer , PACKET_SIZE);
    }

    fread(buffer, 1, last_packet_size, fp);
    write(client_fd , buffer , last_packet_size);

    fclose(fp);
}

int main(){
    socket_address address;
    int client_fd, addrlen = sizeof(address);
    int n_packets;
    char filename[30];
    char comando[10];
    char buffer_in[PACKET_SIZE+1];
    FILE* fp;


    // Socket descriptor, inteiro que a aplicação usa sempre que quer se referir a este socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // AF_INET: IPv4
    // INADDR_ANY: Para todas as interfaces de rede disponíveis
    // PORT: 1337
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(PORT);

    if (!connect(client_fd, (const struct sockaddr *)&address, addrlen)){
        printf("\n----- Conexão Estabelecida -----\n");
    }

    // Le o input do cliente até serem enviados 0 bytes
    while(1){    // Change
        scanf("%s", comando);
        
        if (!strcmp(comando, "exit")){
            write(client_fd, comando, 40);
            printf("\n----- Conexão encerrada -----\n");
            break;
        }

        char path[60] = "./client_/files/";
        char full_path[60] = "";

        scanf(" %[^\n]%*c", filename);
        
        strcat(full_path, path);
        strcat(full_path, filename);
        
        if (!strcmp(comando, "get")) {

            write(client_fd, comando, 40);
            write(client_fd, filename, 40);
            read(client_fd, &n_packets, sizeof(int));
	    if(n_packets == -1)
		printf("\n----- Arquivo não encontrado -----\n\n");
	    else {
                fp = fopen(full_path, "wb");
                printf("Numero de pacotes a receber: %d\n", n_packets);

                int packet_count = 0;
                int pkt_size;
                for (int i = 0; i < n_packets; i++){
                    pkt_size = read(client_fd, buffer_in, PACKET_SIZE);
                    fwrite(buffer_in, 1, pkt_size,fp);
                    packet_count++;
                }
                printf("Numero de pacotes: %d\n", packet_count);
                printf("\n----- Arquivo Recebido -----\n\n");
                fclose(fp);
	    }

        } else if (!strcmp(comando, "put")) {
            fp = fopen(full_path, "rb");
            if (fp == NULL) {
		printf("\n----- Arquivo não encontrado -----\n\n");
            } else {
                int file_size, n_packets, last_packet_size;
                fseek(fp, 0, SEEK_END);
                file_size = ftell(fp);
                fseek(fp, 0, SEEK_SET);

                last_packet_size = file_size%PACKET_SIZE;
                printf("Tamanho do Arquivo: %d\n", file_size);
                n_packets = last_packet_size ? file_size/PACKET_SIZE+1 : file_size/PACKET_SIZE;
                printf("Numero de pacotes a ser enviado: %d\n", n_packets);

                write(client_fd, comando, 40);
                write(client_fd, &n_packets, sizeof(int));
                send_file(fp, client_fd, last_packet_size, n_packets);

            }
        } else {
            printf("Comando inexistente. Digite 'get', 'put' ou 'exit' para encerrar a conexão.\n");
        }
        memset(filename, 0, FILENAME_SIZE); 
    }
    close(client_fd);

    return 0;
}
