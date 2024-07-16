#include <libusb-1.0/libusb.h>
#include <stdio.h>

using namespace std;

void printdev(libusb_device *dev)
{
  libusb_device_descriptor desc; // дескриптор устройства
  libusb_device_handle *dev_handle = nullptr;
  unsigned char str[256];

  int r = libusb_get_device_descriptor(dev, &desc);

  if (r < 0)
  {
    fprintf(stderr, "Ошибка: дескриптор устройства не получен, код: %d.\n",r);
    return;
  }
  printf
  (
    "%.2x %.4x %.4x ",
    desc.bDeviceClass,
    desc.idVendor,
    desc.idProduct
  );

  libusb_open(dev, &dev_handle);

  if (dev_handle && desc.iSerialNumber)
  {
    libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, str, sizeof(str));
    printf("%s", str);
  }
  else
  {
    printf("null");
  }
  printf("\n");

  libusb_close(dev_handle);
}

void printHeader()
{
  printf("===========================================================\n");
  printf("    | * класс устройства\n");
  printf("    | | * идентификатор производителя\n");
  printf("    | | | * идентификатор устройства\n");
  printf("    | | | | * серийный номер\n");
  printf("----+--+----+----+------------+----------------------------\n");
}
void printEnding()
{
  printf("----+--+----+----+------------+----------------------------\n");
  printf("===========================================================\n");
}

int main(){
  libusb_device **devs; // указатель на указатель на устройство,
                        // используется для получения списка устройств
  libusb_context *ctx = NULL; // контекст сессии libusb
  int r; // для возвращаемых значений
  ssize_t cnt; // число найденных USB-устройств
  ssize_t i; // индексная переменная цикла перебора всех устройств

  r = libusb_init(&ctx); // инициализировать библиотеку libusb, открыть сессию работы с libusb
  if(r < 0)
  {
    fprintf(stderr, "Ошибка: инициализация не выполнена, код: %d.\n", r);
    return 1;
  }

  cnt = libusb_get_device_list(ctx, &devs); // получить список всех найденных USB- устройств
  if(cnt < 0)
  {
    fprintf(stderr, "Ошибка: список USB устройств не получен, код: %d.\n", r);
    return 1;
  }

  printHeader();

  for(i = 0; i < cnt; i++)
  {
    printf("%ld\\%ld: ", i + 1, cnt);
    printdev(devs[i]); // печать параметров устройства
  }

  printEnding();

  libusb_free_device_list(devs, 1); // освободить память, выделенную функцией получения списка устройств
  libusb_exit(ctx); // завершить работу с библиотекой libusb,

  return 0;
}

// g++ -o lab6 -I/usr/include/libusb-1.0 lab6.cpp -lusb-1.0
// sudo ./lab6
