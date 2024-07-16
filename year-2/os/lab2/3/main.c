#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>

int main()
{
  pid_t child_pid;
  int status;
  child_pid = fork();
  if (child_pid == 0)
  {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execl("/bin/ls", "ls", NULL);
  }
  else
  {
    while(1)
    {
      wait(&status);
      if (WIFEXITED(status))
      {
        break;
      }
      long orig_rax = ptrace(PTRACE_PEEKUSER, child_pid, 8 * ORIG_RAX, NULL);
      printf("System call %ld\n", orig_rax);
      ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
    }
  }
  return 0;
}
