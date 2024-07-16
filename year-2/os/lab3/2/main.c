#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int create_dir(const char *dir_name)
{
  if (mkdir(dir_name, 0777) == -1)
  {
    printf("Ошибка при создании каталога\n");
    return 1;
  }

  return 0;
}

int print_dir(const char *dir_name)
{
  DIR *dir;
  struct dirent *entry;

  if ((dir = opendir(dir_name)) == NULL)
  {
    printf("Ошибка при открытии каталога\n");
    return 1;
  }

  printf("Содержимое каталога %s:\n", dir_name);

  while ((entry = readdir(dir)) != NULL)
  {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);

  return 0;
}

int delete_dir(const char *dir_name)
{
  if (rmdir(dir_name) == -1)
  {
    printf("Ошибка при удалении каталога\n");
    return 1;
  }

  return 0;
}

int create_file(const char *file_name)
{
  int fd = open(file_name, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    printf("Ошибка при создании файла\n");
    return 1;
  }

  close(fd);

  return 0;
}

int print_file(const char *file_name)
{
  int fd = open(file_name, O_RDONLY);
  if (fd == -1)
  {
    printf("Ошибка при открытии файла\n");
    return 1;
  }

  int buf_size = 1024;
  char buf[1024];
  ssize_t num_read;

  while ((num_read = read(fd, buf, 1024)) > 0)
  {
    write(STDOUT_FILENO, buf, num_read);
  }

  if (num_read == -1)
  {
    printf("Ошибка при чтении файла\n");
    return 1;
  }

  close(fd);

  return 0;
}

int delete_file(const char *file_name)
{
  if (unlink(file_name) == -1)
  {
    printf("Ошибка при удалении файла\n");
    return 1;
  }

  return 0;
}

int create_symlink(const char *file_name, char *link_name)
{
  if (symlink(file_name, link_name) == -1)
  {
    printf("Ошибка при создании символьной ссылки\n");
    return 1;
  }

  return 0;
}

int print_symlink(const char *link_name)
{
  char buf[BUFSIZ];
  ssize_t num_read = readlink(link_name, buf, BUFSIZ);

  if (num_read == -1)
  {
    printf("Ошибка при чтении символьной ссылки\n");
    return 1;
  }
  else
  {
      buf[num_read] = '\0';
      printf("Содержимое символьной ссылки %s: %s\n", link_name, buf);
  }

  return 0;
}

int read_file(const char *file_name)
{
  FILE *file = fopen(file_name, "r");
  if (file == NULL)
  {
    printf("Ошибка при чтении файла\n");
    return 1;
  }
  char c;
  while ((c = fgetc(file)) != EOF)
  {
    putchar(c);
  }
  fclose(file);

  return 0;
}

int create_symbolic_link(const char *source_path, const char *link_path)
{
  if (symlink(source_path, link_path) == -1)
  {
    perror("Ошибка при создании символьной ссылки\n");
    return 1;
  }

  return 0;
}

int read_linked_file(const char *file_name)
{
  char link_target[1024];
  if (readlink(file_name, link_target, sizeof(link_target)) == -1)
  {
    printf("Ошибка при чтении файла по символьной ссылке\n");
    return 1;
  }
  read_file(link_target);

  return 0;
}

int delete_symbolic_link(const char *path)
{
  if (unlink(path) == -1)
  {
    printf("Ошибка при удалении символьной ссылке\n");
    return 1;
  }

  return 0;
}

int create_hard_link(const char *source_path, const char *link_path)
{
  if (link(source_path, link_path) == -1)
  {
    printf("Ошибка при создании жесткой ссылки\n");
    return 1;
  }

  return 0;
}

int delete_hard_link(const char *path)
{
  if (unlink(path) == -1)
  {
    printf("Ошибка при удалении жесткой ссылки\n");
    return 1;
  }

  return 0;
}

int print_file_info(const char *path)
{
  struct stat st;
  if (stat(path, &st) == -1)
  {
    printf("Ошибка при выводе информации о файле\n");
    return 1;
  }

  // printf("Размер файла: %lld bytes\n", (long long) st.st_size);
  printf("Кол-во жестких ссылок: %lu\n", (unsigned long) st.st_nlink);
  printf("Права: ");
  printf((S_ISDIR(st.st_mode)) ? "d" : "-");
  printf((st.st_mode & S_IRUSR) ? "r" : "-");
  printf((st.st_mode & S_IWUSR) ? "w" : "-");
  printf((st.st_mode & S_IXUSR) ? "x" : "-");
  printf((st.st_mode & S_IRGRP) ? "r" : "-");
  printf((st.st_mode & S_IWGRP) ? "w" : "-");
  printf((st.st_mode & S_IXGRP) ? "x" : "-");
  printf((st.st_mode & S_IROTH) ? "r" : "-");
  printf((st.st_mode & S_IWOTH) ? "w" : "-");
  printf((st.st_mode & S_IXOTH) ? "x" : "-");
  printf("\n");

  return 0;
}

int change_file_permissions(const char* path, int perms)
{
  if (chmod(path, perms) == -1)
  {
    printf("Ошибка при изменеии прав доступа к файлу");
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 1)
  {
    printf("Введите аргументы для работы программы\n");
    return 0;
  }

  if (!strcmp(argv[0], "./a_create_dir"))
  {
    return create_dir(argv[1]);
  }
  else  if (!strcmp(argv[0], "./b_print_dir"))
  {
    return print_dir(argv[1]);
  }
  else if (!strcmp(argv[0], "./c_delete_dir"))
  {
    return delete_dir(argv[1]);
  }
  else if (!strcmp(argv[0], "./d_create_file"))
  {
    return create_file(argv[1]);
  }
  else if (!strcmp(argv[0], "./e_print_file"))
  {
    return print_file(argv[1]);
  }
  else if (!strcmp(argv[0], "./f_delete_file"))
  {
    return delete_file(argv[1]);
  }
  else if (!strcmp(argv[0], "./g_create_simlink"))
  {
    if (argc <= 2)
    {
      printf("Мало аргументов\n");
      return 1;
    }
    return create_symbolic_link(argv[1], argv[2]);
  }
  else if (!strcmp(argv[0], "./h_print_simlink"))
  {
    return print_symlink(argv[1]);
  }
  else if (!strcmp(argv[0], "./i_print_file_simlink"))
  {
    return read_linked_file(argv[1]);
  }
  else if (!strcmp(argv[0], "./j_delete_simlink"))
  {
    return delete_symbolic_link(argv[1]);
  }
  else if (!strcmp(argv[0], "./k_create_hard_link"))
  {
    if (argc <= 2)
    {
      printf("Мало аргументов\n");
      return 1;
    }
    return create_hard_link(argv[1], argv[2]);
  }
  else if (!strcmp(argv[0], "./l_delete_hard_link"))
  {
    return delete_hard_link(argv[1]);
  }
  else if (!strcmp(argv[0], "./m_print_file_info"))
  {
    return print_file_info(argv[1]);
  }
  else if (!strcmp(argv[0], "./n_change_file_permissions"))
  {
    int perms;
    sscanf(argv[2], "%o", &perms);
    return change_file_permissions(argv[1], perms);
  }

  return 0;
}
