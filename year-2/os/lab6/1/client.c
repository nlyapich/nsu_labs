#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE     1024
#define SERVER_PORT     8888

const char* SERVER_IP = "127.0.0.1";

int main(int argc, char **argv)
{
  int err;
  int client_sock;
  char buffer[BUFFER_SIZE];
  socklen_t sock_addr_length = sizeof(struct sockaddr_in);
  struct sockaddr_in server_sock_addr;

  // Create client socket
  client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (client_sock == -1)
  {
    perror("socket");
    close(client_sock);
    return 0;
  }

  // Initialize server address
  server_sock_addr.sin_family = AF_INET;
  server_sock_addr.sin_port = SERVER_PORT;
  err = inet_pton(AF_INET, SERVER_IP, &(server_sock_addr.sin_addr));
  if (err == -1)
  {
    perror("inet_pton");
    close(client_sock);
    return 0;
  }

  while (1)
  {
    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter message: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    if (buffer[strlen(buffer) - 1] == '\n')
    {
      buffer[strlen(buffer) - 1] = '\0';
    }

    err = sendto(client_sock, buffer, strlen(buffer), 0, (struct sockaddr *) &server_sock_addr, sock_addr_length);
    if (err == -1)
    {
      perror("sendto");
      continue;
    }

    err = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &server_sock_addr, &sock_addr_length);
    if (err == -1)
    {
      perror("recvfrom");
      continue;
    }
    printf("Message received: %s\n\n", buffer);
  }

  close(client_sock);

  return 0;
}
