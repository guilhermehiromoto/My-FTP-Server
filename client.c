#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 7331

typedef struct sockaddr_in socket_address;


int main(){
    socket_address address;
    int client_fd, addrlen = sizeof(address);

    // Socket descriptor, inteiro que a aplicação usa sempre que quer se referir a este socket  
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // AF_INET: IPv4
    // INADDR_ANY: Para todas as interfaces de rede disponíveis
    // PORT: 1337
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(PORT); 

    connect(client_fd, (const struct sockaddr *)&address, addrlen);
    char buffer_out[30000] = "\n----- Conexão Estabelecida -----\n";
    char buffer_in[30000];
    // Le o input do cliente até serem enviados 0 bytes
    while(1){
        scanf("%[^\n]%*c", buffer_out);
        write(client_fd , buffer_out, 30000);
        read(client_fd , buffer_in, 30000); 
        printf("%s\n",buffer_in);
    }

    close(client_fd);

    return 0;
}