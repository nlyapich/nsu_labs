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
  int server_sock;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in server_sock_addr;
  struct sockaddr_in client_sock_addr;
  socklen_t sock_addr_length = sizeof(struct sockaddr_in);

  server_sock_addr.sin_family = AF_INET;
  server_sock_addr.sin_port = SERVER_PORT;
  server_sock_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

  server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (server_sock == -1)
  {
    perror("socket");
    close(server_sock);
    return 0;
  }

  err = bind(server_sock, (struct sockaddr *) &server_sock_addr, sock_addr_length);
  if (err == -1)
  {
    perror("bind");
    close(server_sock);
    return 0;
  }

  while (1)
  {
    memset(buffer, 0, BUFFER_SIZE);
    err = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_sock_addr, &sock_addr_length);
    if (err == -1)
    {
      perror("recvfrom");
      continue;
    }

    printf("Message content: %s\n\n", buffer);

    err = sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_sock_addr,
    sock_addr_length);
    if (err == -1)
    {
      perror("sendto");
      continue;
    }
  }

  close(server_sock);

  return 0;
}
