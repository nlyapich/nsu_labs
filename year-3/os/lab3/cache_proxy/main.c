#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

#include "cache_proxy.h"

#define BUFFER_SIZE 8192
#define PORT 8835
#define SIZE_CACHE 256

typedef struct
{
  int bytes_received;
  char* name;
} LoadData;

CacheProxy* cache_proxy;

void* load_data(void* l_data)
{
  char buffer[BUFFER_SIZE];
  memset(&buffer, (char)0, BUFFER_SIZE);

  LoadData* my_load_data = (LoadData*) l_data;
  int bytes_received = my_load_data->bytes_received;
  char* name = my_load_data->name;

  char* host_start = strstr(name, "Host: ");
  if (!host_start)
  {
    pthread_exit(NULL);
  }
  host_start += 6;

  char* host_end = strchr(host_start, '\r');
  if (!host_end)
  {
    pthread_exit(NULL);
  }

  char result_host[100];
  memcpy(result_host, host_start, host_end - host_start);

  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(result_host, "http", &hints, &res) != 0)
  {
    perror("Ошибка при создании сокета сервера");
    pthread_exit(NULL);
  }

  int server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (server_socket < 0)
  {
    perror("Ошибка при создании сокета сервера");
    pthread_exit(NULL);
  }

  if (connect(server_socket, res->ai_addr, res->ai_addrlen) < 0)
  {
    perror("Ошибка при подключении к серверу");
    close(server_socket);
    freeaddrinfo(res);
    pthread_exit(NULL);
  }

  if (send(server_socket, name, bytes_received, 0) < 0)
  {
    perror("Ошибка при отправке данных на сервер");
    close(server_socket);
    freeaddrinfo(res);
    pthread_exit(NULL);
  }

  int pos = find_name(cache_proxy, name);
  while ((bytes_received = recv(server_socket, buffer, sizeof(buffer), 0)) > 0)
  {
    add_data_cache_proxy(cache_proxy, pos, buffer, bytes_received);
  }
  set_finish_loaded_cache_proxy(cache_proxy, pos);

  close(server_socket);
  freeaddrinfo(res);
  pthread_exit(NULL);
}

void* handle_client(void* client_socket_raw)
{
  int client_socket = (int)client_socket_raw;

  char buffer[BUFFER_SIZE];
  ssize_t bytes_received;

  memset(&buffer, (char)0, BUFFER_SIZE);

  bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
  if (bytes_received < 0)
  {
    perror("Ошибка в запросе клиента");
    close(client_socket);
    pthread_exit(NULL);
  }

  char name_cache[BUFFER_SIZE];
  memset(&name_cache, (char)0, BUFFER_SIZE);
  memcpy(name_cache, buffer, bytes_received);

  pthread_t tid;
  int pos_name = find_name(cache_proxy, name_cache);
  if (pos_name == -1)
  {
    pos_name = add_record_cache_proxy(cache_proxy, name_cache);

    LoadData l_data;
    l_data.bytes_received = bytes_received;
    l_data.name = name_cache;
    if (pthread_create(&tid, NULL, load_data, (void *) &l_data) != 0)
    {
      perror("Ошибка при создании потока");
      close(client_socket);
    }
  }

  size_t cur_pos_cache = 0;

  pthread_mutex_lock(&(cache_proxy->global_mutex));
  pthread_mutex_t* cur_mutex = cache_proxy->cache_mutex + pos_name;
  pthread_mutex_unlock(&(cache_proxy->global_mutex));
  while (1)
  {
    pthread_mutex_lock(cur_mutex);

    char* cache = (cache_proxy->cache[pos_name] + cur_pos_cache);
    size_t tmp_size = (cache_proxy->cur_size_cache[pos_name] - cur_pos_cache);
    size_t cur_bytes_received = (tmp_size >= BUFFER_SIZE) ? BUFFER_SIZE : (tmp_size);
    if ((cur_bytes_received <= 0) && (cache_proxy->is_finish_loaded[pos_name]))
    {
      pthread_mutex_unlock(cur_mutex);
      break;
    }
    if (cur_bytes_received <= 0)
    {
      pthread_mutex_unlock(cur_mutex);
      continue;
    }
    memcpy(buffer, cache, cur_bytes_received);
    pthread_mutex_unlock(cur_mutex);
    cur_pos_cache += cur_bytes_received;

    if (send(client_socket, buffer, cur_bytes_received, 0) < 0)
    {
      perror("Ошибка при отправке данных клиенту");
      break;
    }
  }

  printf("handle_client: end\n");
  close(client_socket);

  pthread_join(tid, NULL);

  pthread_exit(NULL);
}

void sigint_handler(int signo)
{
  if (signo == SIGINT)
  {
    printf("broken sigint\n");
    exit(EXIT_FAILURE);
  }
}

int main()
{
  signal(SIGINT, sigint_handler);

  cache_proxy = init_cache_proxy(SIZE_CACHE);
  if (!cache_proxy)
  {
    perror("Ошибка при создании кэша");
    exit(EXIT_FAILURE);
  }

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
    perror("Ошибка при создании сокета сервера");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, (int)0, (size_t)sizeof(server_addr));
  server_addr.sin_family = AF_UNSPEC;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Ошибка при привязке сокета сервера к адресу и порту");
    delete_cache_proxy(cache_proxy);
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  if (listen(server_socket, 30) < 0)
  {
    perror("Ошибка при прослушивании подключений");
    delete_cache_proxy(cache_proxy);
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

    if (client_socket < 0)
    {
      perror("Ошибка при принятии подключения");
      continue;
    }

    pthread_t tid;
    if (pthread_create(&tid, NULL, handle_client, (void *) client_socket) != 0)
    {
      perror("Ошибка при создании потока");
      close(client_socket);
    }
    else
    {
      pthread_detach(tid);
    }
  }

  delete_cache_proxy(cache_proxy);
  close(server_socket);
}
