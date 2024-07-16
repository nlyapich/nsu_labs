#include "cache_proxy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_BUFFER_PROXY 8192

CacheProxy* init_cache_proxy(size_t size_cache)
{
  CacheProxy* cache_proxy = (CacheProxy*) malloc(sizeof(CacheProxy));
  if (!cache_proxy)
  {
    return NULL;
  }

  cache_proxy->cache_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t) * size_cache);
  for (size_t i = 0; i < size_cache; ++i)
  {
    pthread_mutex_init(cache_proxy->cache_mutex + i, NULL);
  }

  pthread_mutex_init(&(cache_proxy->global_mutex), NULL);
  cache_proxy->size_cache = size_cache;
  cache_proxy->names = (char**) calloc(size_cache, sizeof(char*));
  cache_proxy->is_finish_loaded = (char*) calloc(size_cache, sizeof(char));
  cache_proxy->cache = (char**) calloc(size_cache, sizeof(char*));
  cache_proxy->max_size_cache = (size_t*) calloc(size_cache, sizeof(size_t));
  cache_proxy->cur_size_cache = (size_t*) calloc(size_cache, sizeof(size_t));
  cache_proxy->last_writing_cache = -1;

  return cache_proxy;
}

int find_name(const CacheProxy* cache_proxy, const char* name)
{
  int return_value = -1;

  pthread_mutex_lock(&(cache_proxy->global_mutex));
  char** names = cache_proxy->names;
  for (size_t i = 0; i < cache_proxy->size_cache; ++i)
  {
    pthread_mutex_lock(&(cache_proxy->cache_mutex[i]));
    if (!names[i])
    {
      pthread_mutex_unlock(&(cache_proxy->cache_mutex[i]));
      break;
    }
    if (strcmp(name, names[i]) == 0)
    {
      return_value = (int)i;
      pthread_mutex_unlock(&(cache_proxy->cache_mutex[i]));
      break;
    }
    pthread_mutex_unlock(&(cache_proxy->cache_mutex[i]));
  }

  pthread_mutex_unlock(&(cache_proxy->global_mutex));
  return return_value;
}

size_t add_record_cache_proxy(CacheProxy* cache_proxy, const char* name)
{
  pthread_mutex_lock(&(cache_proxy->global_mutex));
  size_t new_pos = (cache_proxy->last_writing_cache + 1) % (cache_proxy->size_cache);
  pthread_mutex_lock(&(cache_proxy->cache_mutex[new_pos]));
  cache_proxy->last_writing_cache++;

  free(cache_proxy->names[new_pos]);

  cache_proxy->is_finish_loaded[new_pos] = 0;
  cache_proxy->names[new_pos] = (char*) calloc((strlen(name) + 1), sizeof(char));
  memcpy(cache_proxy->names[new_pos], name, strlen(name));

  free(cache_proxy->cache[new_pos]);
  cache_proxy->cache[new_pos] = (char*) calloc(SIZE_BUFFER_PROXY, sizeof(char));
  cache_proxy->max_size_cache[new_pos] = SIZE_BUFFER_PROXY;
  cache_proxy->cur_size_cache[new_pos] = 0;
  pthread_mutex_unlock(&(cache_proxy->cache_mutex[new_pos]));
  pthread_mutex_unlock(&(cache_proxy->global_mutex));

  return new_pos;
}

void add_data_cache_proxy(CacheProxy* cache_proxy, size_t pos, const char* buffer, int bytes_received)
{
  pthread_mutex_lock(&(cache_proxy->cache_mutex[pos]));
  size_t max_size_cache = cache_proxy->max_size_cache[pos];
  size_t cur_size_cache = cache_proxy->cur_size_cache[pos];
  if ((cur_size_cache + bytes_received) > max_size_cache)
  {
    cache_proxy->cache[pos] = (char*) realloc(cache_proxy->cache[pos], max_size_cache * 2);
    max_size_cache *= 2;
    cache_proxy->max_size_cache[pos] = max_size_cache;
  }

  memcpy(cache_proxy->cache[pos] + cur_size_cache, buffer, bytes_received);
  cache_proxy->cur_size_cache[pos] += bytes_received;
  pthread_mutex_unlock(&(cache_proxy->cache_mutex[pos]));
}

void set_finish_loaded_cache_proxy(CacheProxy* cache_proxy, size_t pos)
{
  pthread_mutex_lock(&(cache_proxy->cache_mutex[pos]));
  cache_proxy->is_finish_loaded[pos] = 1;
  pthread_mutex_unlock(&(cache_proxy->cache_mutex[pos]));
}

void delete_cache_proxy(CacheProxy* cache_proxy)
{
  pthread_mutex_lock(&(cache_proxy->global_mutex));
  size_t size_cache = cache_proxy->size_cache;

  pthread_mutex_t* cache_mutex = cache_proxy->cache_mutex;
  for (size_t i = 0; i < size_cache; ++i)
  {
    pthread_mutex_destroy(cache_mutex + i);
  }
  free(cache_mutex);

  char** names = cache_proxy->names;
  for (size_t i = 0; i < size_cache; ++i)
  {
    free(names[i]);
  }
  free(names);

  free(cache_proxy->is_finish_loaded);

  char** cache = cache_proxy->cache;
  for (size_t i = 0; i < size_cache; ++i)
  {
    free(cache[i]);
  }
  free(cache);

  free(cache_proxy->max_size_cache);
  free(cache_proxy->cur_size_cache);
  pthread_mutex_unlock(&(cache_proxy->global_mutex));

  pthread_mutex_destroy(&(cache_proxy->global_mutex));
}
