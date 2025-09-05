#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE_SIZE 5000
#define PORT 8000
#define ADDR "127.0.0.1"

int create_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in create_sockaddr() {
    struct sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT);
    sock.sin_addr.s_addr = inet_addr(ADDR);
    return sock;
}

int main() {
    int server_socket = create_socket();

    if (server_socket == -1) { 
        printf("Socket was not created.\n");
        return 1;
    }
    printf("Socket created - [%d]\n", server_socket);

    struct sockaddr_in socketaddress = create_sockaddr();

    if (bind(server_socket, (struct sockaddr*)&socketaddress, sizeof(socketaddress)) == -1) {
        printf("Binding failed.\n");
        close(server_socket);
        return 1;
    }
    printf("Binding Successful.\n");

    if (listen(server_socket, 1) == -1) {
        printf("Listening failed.\n");
        close(server_socket);
        return 1;
    }
    printf("Listening on port %d...\n", PORT);

    while (true) {
    struct sockaddr_storage client_addr;
        socklen_t addr_size = sizeof(client_addr);

        int newsocket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (newsocket == -1) {
            printf("Error: Failed to accept connection.\n");
            close(server_socket);
            return 1;
        }
        printf("Connection accepted!\n");

        char* buffer = malloc(RESPONSE_SIZE);
        if (!buffer) {
            printf("Memory allocation failed.\n");
            close(newsocket);
            close(server_socket);
            return 1;
        }

        
        int length;
        int sent = false;  
        while ((length = recv(newsocket, buffer, RESPONSE_SIZE - 1, 0)) > 0) {
            buffer[length] = '\0';
            printf("%s", buffer);

            if (!sent) {
                char* msg = "User: josh\nHome directory: /home/josh\nLast login: 2025-09-05 01:15\n"; // example finger response
                int send_res = send(newsocket, msg, strlen(msg), 0);
                if (send_res == -1) {
                    printf("Sending message back failed.\n");
                } else {
                    printf("Sending... Done!\n");
                }
                sent = true; 
            }
        }

        if (length == 0) {
            printf("\nClient %d closed the connection.\n", newsocket);
        } else if (length < 0) {
            printf("\nError receiving data.\n");
        }

        free(buffer);
        close(newsocket);
    }
   
    close(server_socket);
    return 0;
}
