#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

typedef struct {
    char *key;
    char *value;
    size_t value_size;
    size_t value_len;
} CacheEntry;

int main(int argc, char *argv[]) {
    // This is a TCP server! args should be: ./server <local> <port> where local means if server should run locally or remotely

    int server_door_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[4096] = {0};

    if ((server_door_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

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

    while(1) {
        if (new_socket <= 0) {
            if((new_socket = accept(server_door_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
                perror("Connection failed\n");
                continue;
            } else {
                printf("Connection accepted!\n");
            }
        }
        printf("Waiting for messages...\n");
        valread = read(new_socket, buffer, 1023);
        printf("Recieved message: %s", buffer);
    }

    close(new_socket);

    close(server_door_fd);
    return 0;
}