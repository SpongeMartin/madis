#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char *argv[]){
    int client_fd, valread, status;
    struct sockaddr_in serv_addr;
    char buffer[4096];
    char* message;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(status == 0){
        printf("Enter message: ");
        fgets(buffer, 4096, stdin);
        if(send(client_fd, buffer, strlen(buffer), 0) < 0){
            printf("Send failed\n");
        } else {
            printf("Message sent\n");
        }
    }

    close(client_fd);
    return 0;
}