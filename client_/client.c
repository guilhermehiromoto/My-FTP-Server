#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 7331
#define PACKET_SIZE 1024

typedef struct sockaddr_in socket_address;

int main(){
    socket_address address;
    int client_fd, addrlen = sizeof(address);
    int counter = 0;
    char filename[30];
    char buffer_in[PACKET_SIZE];
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
        int packet_count = 0;
        int pkt_size;
        while(1){
            pkt_size=read(client_fd , buffer_in, PACKET_SIZE);
            
            if(pkt_size < 2) break;

            fwrite(buffer_in,1,pkt_size-1,fp);
            printf("\n----- Pacote Recebido -----\n");

        }
        printf("---------\n");
        fclose(fp);
    }

    close(client_fd);

    return 0;
}
