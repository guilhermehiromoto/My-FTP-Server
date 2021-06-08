CC=gcc
SERVER_BINARY=./server_/server
CLIENT_BINARY=./client_/client
SERVER_SRC=./server_/server.c
CLIENT_SRC=./client_/client.c
OPTIONS=-Wall -g -lpthread

all: server client

server: $(SERVER_SRC)
	$(CC) $(SERVER_SRC) -o $(SERVER_BINARY) $(OPTIONS)

client: $(CLIENT_SRC)
	$(CC) $(CLIENT_SRC) -o $(CLIENT_BINARY) $(OPTIONS)

runServer: $(SERVER_BINARY)
	$(SERVER_BINARY)

runClient: $(CLIENT_BINARY)
	$(CLIENT_BINARY)

clean:
	rm $(SERVER_BINARY) $(CLIENT_BINARY)
