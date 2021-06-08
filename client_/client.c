#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "../config.h"

typedef struct sockaddr_in socket_address;

int main(){
    socket_address address;
    int client_fd, addrlen = sizeof(address);
    int n_packets;
    char filename[30];
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
        scanf("%[^\n]%*c", filename);
        fp = fopen(filename, "wb");

        write(client_fd , filename, 30);

        if (!strcmp(filename, "sair")){
            printf("\n----- Conexão encerrada -----\n");
            break;
        }

        read(client_fd , &n_packets, sizeof(int));
        printf("Numero de pacotes a receber: %d\n", n_packets);

        int packet_count = 0;
        int pkt_size;
        for (int i = 0; i < n_packets; i++){
            pkt_size=read(client_fd , buffer_in, PACKET_SIZE);
            fwrite(buffer_in,1,pkt_size,fp);
            packet_count++;
        }
        printf("Numero de pacotes: %d\n", packet_count);
        printf("\n----- Arquivo Recebido -----\n");
        printf("---------\n");
        fclose(fp);
    }

    close(client_fd);

    return 0;
}
