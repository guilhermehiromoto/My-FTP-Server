#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1337
#define PACKET_SIZE 10

typedef struct sockaddr_in socket_address;

void write_file(FILE* fp , char* buffer_in){
    int i = 0;
    while(buffer_in[i] != '\0'){
        printf("|%c|", buffer_in[i]);
        //fwrite(&(buffer_in[i]),1,1,fp);
        i++;
    }
    printf("algumacoisa");
    //printf("\n");
    //getchar();
}

int main(){
    socket_address address;
    int client_fd, addrlen = sizeof(address);
    //FILE* fp;


    // Socket descriptor, inteiro que a aplicação usa sempre que quer se referir a este socket  
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // AF_INET: IPv4
    // INADDR_ANY: Para todas as interfaces de rede disponíveis
    // PORT: 1337
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
    address.sin_port = htons(PORT); 

    connect(client_fd, (const struct sockaddr *)&address, addrlen);
    printf("\n----- Conexão Estabelecida -----\n");

    char filename[30];
    memset(filename, 0, 30);
    char buffer_in[PACKET_SIZE];
    memset(buffer_in, 0, PACKET_SIZE);

    // Le o input do cliente até serem enviados 0 bytes
    while(1){    // Change
        scanf(" %[^\n]%*c", filename);
        //fp = fopen(filename, "wb");

        write(client_fd , filename, 30);

        if (!strcmp(filename, "sair")){
            printf("\n----- Conexão encerrada -----\n");
            break;
        }
        while(read(client_fd , buffer_in, PACKET_SIZE) > 0){
            //write_file(fp, buffer_in);
            printf("|%s|", buffer_in);
            //break;
        }

        //fclose(fp);
    }

    close(client_fd);

    return 0;
}