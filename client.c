#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define RESPONSE_SIZE 5000
#define PORT 8000
#define ADDR "127.0.0.1"

int create_socket() {
  int res = socket(AF_INET, SOCK_STREAM, 0);
  return res;
}

struct sockaddr_in create_sockaddr() {
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_port = htons(PORT);
  sock.sin_addr.s_addr = inet_addr(ADDR);
  return sock;
}

int main () {
  int socket = create_socket();
  if (socket == -1) { 
    printf("Error: Socket was not created.\n");
    return 1;
  }
  printf("Socket created - [%d]\n", socket);

  struct sockaddr_in sockaddress = create_sockaddr();

  int connect_res = connect(socket, (struct sockaddr *) &sockaddress, sizeof(sockaddress));
  if (connect_res == -1) { 
    printf("Error: Connection was not made\n");
    return 1;
  }
  printf("Connected!\n");

  char* msg = "josh\r\n"; // example finger message

  printf("-----SEND MESSAGE-----\n");
  printf("%s", msg);
  int send_res = send(socket, msg, strlen(msg), 0);
  if (send_res == -1) { 
    printf("Failed to send message."); 
    return 1;
  }

  printf("-----RECEIVE MESSAGE-----\n");

  // main response
  char* output = malloc(RESPONSE_SIZE);
  int length = recv(socket, output, RESPONSE_SIZE, 0);
  printf("%s", output);
  close(connect_res);
  close(socket);
  return 0;
}