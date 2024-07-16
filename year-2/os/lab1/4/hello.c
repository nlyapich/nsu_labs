#include <stdio.h>
#include <dlfcn.h>

int main()
{
  void *h;
  char* error;

  h = dlopen("./libhello_dyn_runtime_lib.so", RTLD_LAZY);
  if (!h)
  {
    printf("dlopen() failed: %s\n", dlerror());
    return 1;
  }

  dlerror();

  void (*hello_from_dyn_runtime_lib)() = (void (*)()) dlsym(h, "hello_from_dyn_runtime_lib");

  error = dlerror();
  if (error)
  {
    printf("dlsym() failed: %s\n", error);
    return 1;
  }

  hello_from_dyn_runtime_lib();
  dlclose(h);

  return 0;
}
