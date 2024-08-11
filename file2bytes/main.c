#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* microsoft need me to use fopen_s */
#if _MSC_VER
FILE* msvcrt_fopen_wrap(
  const char *p_filename,
  const char *p_mode)
{
  FILE* fp;
  fopen_s(&fp, p_filename, p_mode);
  return fp;
}
#define fopen(f, m) msvcrt_fopen_wrap(f, m)
#endif

uint8_t* file_to_memory(size_t* p_dst_size, const char* p_file_path)
{
  uint8_t* p_buffer = NULL;
  size_t   num_bytes_read;
  FILE* fp = fopen(p_file_path, "rb");
  if (!fp) {
    perror("Error opening file");
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  *p_dst_size = (size_t)ftell(fp);
  fseek(fp, 0, SEEK_SET);
  p_buffer = (uint8_t *)malloc(*p_dst_size);
  if (p_buffer) {
    num_bytes_read = fread(p_buffer, 1, *p_dst_size, fp);
    if (num_bytes_read == *p_dst_size) {     
      fclose(fp);
      return p_buffer;
    }
    perror("Error reading file");
    free(p_buffer);   
  }
  perror("Memory allocation failed");
  fclose(fp);
  return NULL;
}

int main(int argc, char* argv[])
{
  uint8_t* p_file_bytes;
  size_t   data_size;

  if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  p_file_bytes = file_to_memory(&data_size, argv[1]);
  if (p_file_bytes) {
    printf("unsigned char fileData[] = {\n");
    for (size_t i = 0; i < data_size; i++) {
      printf("0x%02x", p_file_bytes[i]);
      if (i < data_size - 1)
        printf(", ");

      if ((i + 1) % 16 == 0) {
        printf("\n\t");
      }
    }
    printf("\n};\n");
    free(p_file_bytes);
    return 0;
  }
  printf("Press any key to continue...\n");
  getchar();
  return 1;
}
