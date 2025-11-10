#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "hashtable.c"

#define PORT 8080
#define MAX_CONNECTIONS 50 // Could be cool to allow only SHA connections.

typedef struct {
    char *key;
    char *value;
    size_t value_size;
    size_t value_len;
} Payload;

typedef struct {
    size_t fd;
    char *buffer;
} Client;

int epoll_fd;

void handleMessage(Client *client) {
    ssize_t msg_length = recv(client->fd, client->buffer, sizeof(client->buffer), 0);
    if (msg_length <= 0) {
        printf("Client %li has died.\n", client->fd);
        close(client->fd);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client->fd, NULL);
        free(client->buffer);
        return;
    }
    client->buffer[msg_length] = '\0';
    printf("Recieved message from %li: %s", client->fd, client->buffer);
}


int main(int argc, char *argv[]) {
    // This is a TCP server! args should be: ./server <local> <port> where local means if server should run locally or remotely

    int server_door_fd;
    struct sockaddr_in address;
    struct epoll_event event, events[4096];
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    HashTable *ht_cache = constructHashTable(100000);
    HashTable *ht_clients = constructHashTable(MAX_CONNECTIONS);

    // Initialize server socket
    if ((server_door_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to port
    if (setsockopt(server_door_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    } // IPPROTO_TCP  for tcp

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_door_fd, (struct sockaddr*)&address, addrlen) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_door_fd, 3);
    printf("Listening on port %d\n", PORT);

    int flags = fcntl(server_door_fd, F_GETFL, 0);
    fcntl(server_door_fd, F_SETFL, flags | SOCK_NONBLOCK); // Make socket non-blocking

    // Set up epoll
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    //memset(&event, 0, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = server_door_fd;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_door_fd, &event) == -1) {
		perror("Failed to add file descriptor to epoll\n");
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}

    while(1) {
        int n = epoll_wait(epoll_fd, events, 1023, -1);
        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            if(fd == server_door_fd) {
                int client_fd = accept(server_door_fd, (struct sockaddr*)&address, &addrlen);
                int flags = fcntl(client_fd, F_GETFL, 0);
                fcntl(client_fd, F_SETFL, flags | SOCK_NONBLOCK);
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("Failed to add new socket to epoll\n");
                    break;
                }
                Client *client = malloc(sizeof(Client));
                client->fd = client_fd;
                addElement(ht_clients, (char *)fd, client);
                printf("New connection accepted %i\n", client_fd);
            } else {
                handleMessage((Client *)findElement(ht_clients, (char *)fd));
            }
        }
    }

    close(server_door_fd);
    return 0;
}