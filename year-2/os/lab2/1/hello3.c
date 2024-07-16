#include <unistd.h>
#include <sys/syscall.h>

static ssize_t mywrite(int fd, const void* buf, size_t count)
{
  return syscall(SYS_write, fd, buf, count);
}

int main()
{
    mywrite(1, "Hello world\n", 12);
    return 0;
}
