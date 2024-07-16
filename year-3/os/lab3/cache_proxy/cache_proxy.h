#ifndef CACHE_PROXY_H
#define CACHE_PROXY_H

#include <stddef.h>
#include <pthread.h>

typedef struct
{
  pthread_mutex_t* cache_mutex;
  pthread_mutex_t global_mutex;
  size_t size_cache;
  char** names;
  char* is_finish_loaded;
  char** cache;
  size_t* max_size_cache;
  size_t* cur_size_cache;
  size_t last_writing_cache;
} CacheProxy;

CacheProxy* init_cache_proxy(size_t size_cache);
int find_name(const CacheProxy* cache_proxy, const char* name);
size_t add_record_cache_proxy(CacheProxy* cache_proxy, const char* name);
void add_data_cache_proxy(CacheProxy* cache_proxy, size_t pos, const char* buffer, int bytes_received);
void set_finish_loaded_cache_proxy(CacheProxy* cache_proxy, size_t pos);
void delete_cache_proxy(CacheProxy* cache_proxy);

#endif // CACHE_PROXY_H
