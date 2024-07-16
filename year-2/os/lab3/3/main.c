#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 512

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <pid>\n", argv[0]);
    return 1;
  }

  char filename[BUF_SIZE];
  snprintf(filename, BUF_SIZE, "/proc/%s/pagemap", argv[1]);

  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    printf("Failed to open file %s\n", filename);
    return 1;
  }

  unsigned long long page_flags;

  while (fread(&page_flags, sizeof(page_flags), 1, fp) == 1)
  {
    unsigned long long page_frame_num = page_flags & ((1ULL << 55) - 1);
    int page_present = (page_flags >> 63) & 1;
    int page_swapped = (page_flags >> 62) & 1;
    int page_file_mapped = (page_flags >> 61) & 1;
    int page_unevictable = (page_flags >> 60) & 1;

    if (page_frame_num || page_present || page_swapped || page_file_mapped || page_unevictable)
      printf("Page Frame Num: %llu, Present: %d, Swapped: %d, File Mapped: %d, Unevictable: %d\n",
      page_frame_num, page_present, page_swapped, page_file_mapped, page_unevictable);
  }


  fclose(fp);
  return 0;
}
