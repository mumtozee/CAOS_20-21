#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

extern char *memory;
extern size_t fsize;
extern void myalloc_initialize (int fd);
extern void myalloc_finalize ();

extern void *my_malloc (size_t size);
extern void my_free (void *ptr);

#define FILESIZE (600)

int
main (void)
{
  const char fname[] = "buffer.txt";
  int fd = open (fname, O_RDWR | O_CREAT | O_TRUNC);
  ftruncate (fd, FILESIZE);
  myalloc_initialize (fd);
  char *arr1 = my_malloc (3);
  char *arr2 = my_malloc (3);
  char *arr3 = my_malloc (3);
  printf ("%p\n", arr1);
  my_free (arr1);
  my_free (arr2);
  // my_free(arr3);
  arr1 = my_malloc (3);
  printf ("%p\n", arr1);
  myalloc_finalize ();
  close (fd);
  return 0;
}