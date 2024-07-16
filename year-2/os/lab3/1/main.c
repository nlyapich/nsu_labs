#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void reverse_mem(char* buf, int size)
{
  for (int i = 0; i < (size / 2); i++)
  {
    char tmp = buf[i];
    buf[i] = buf[size - i - 1];
    buf[size - i - 1] = tmp;
  }
}

void copy_file_reverse(const char* src_path, const char* dst_path)
{
  FILE* src_file = fopen(src_path, "rb");
  if (!src_file)
  {
    return;
  }

  FILE* dst_file = fopen(dst_path, "wb");
  if (!dst_file)
  {
    fclose(src_file);
    return;
  }

  int buf_size = 512;
  char* buf = (char*) malloc(buf_size*sizeof(char));
  if (!buf)
  {
    fclose(src_file);
    fclose(dst_file);
  }

  fseek(src_file, 0, SEEK_END);
  long int file_size = ftell(src_file);
  long int cur_pos = file_size;

  while (cur_pos > 0)
  {
    if (cur_pos >= buf_size)
    {
      cur_pos -= buf_size;
    }
    else
    {
      buf_size = cur_pos;
      cur_pos = 0;
    }

    fseek(src_file, cur_pos, SEEK_SET);
    int count_symb = fread(buf, sizeof(char), buf_size, src_file);
    if (!count_symb)
    {
      break;
    }
    reverse_mem(buf, count_symb);
    fwrite(buf, sizeof(char), count_symb, dst_file);
  }

  free(buf);
  fclose(src_file);
  fclose(dst_file);
}

void copy_dir_reverse(const char* src_path)
{
  char* dst_path = (char*) malloc(strlen(src_path)+1);
  if (!dst_path)
  {
    printf("Не удалось выделить память для dst_path\n");
    return;
  }

  strcpy(dst_path, src_path);
  reverse_mem(dst_path, strlen(dst_path));

  // Чтобы предоставить все права всем (режим 0777 = rwxrwxrwx)
  if (mkdir(dst_path, 0777) == -1)
  {
    printf("Не удалось создать каталог %s\n", dst_path);
    free(dst_path);
    return;
  }

  // Открываем исходный каталог для чтения
  DIR* src_dir = opendir(src_path);
  if (!src_dir){
    printf("Не удалось открыть каталог %s для чтения\n", src_path);
    free(dst_path);
    return;
  }

  int buf_size = 256;
  char* src_file_path = (char*) malloc(strlen(src_path) + buf_size);
  if (!src_file_path){
    printf("Не удалось выделить память для src_file_path\n");
    free(dst_path);
    return;
  }

  char* dst_file_path = (char*) malloc(strlen(dst_path) + buf_size);
  if (!dst_file_path){
    printf("Не удалось выделить память для dst_file_path\n");
    free(dst_path);
    free(src_file_path);
    return;
  }

  // Копируем файлы задом наперед
  struct dirent* entry;
  while ((entry = readdir(src_dir)))
  {
    if (entry->d_type != DT_REG)
    {
      continue;
    }

    // Полный путь к исходному файлу
    sprintf(src_file_path, "%s/%s", src_path, entry->d_name);

    // Полный путь к перевернутому
    reverse_mem(entry->d_name, strlen(entry->d_name));
    sprintf(dst_file_path, "%s/%s", dst_path, entry->d_name);

    copy_file_reverse(src_file_path, dst_file_path);
  }

  closedir(src_dir);
  free(dst_path);
  free(src_file_path);
  free(dst_file_path);
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("Некорректный ввод\n");
    return 1;
  }

  copy_dir_reverse(argv[1]);

  return 0;
}
