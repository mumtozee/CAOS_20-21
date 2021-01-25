#define _FILE_OFFSET_BITS 64
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int read_retry(int fd, char* data, ssize_t size) {
  char* cdata = data;
  while (1) {
    ssize_t read_bytes = read(fd, cdata, size);
    if (read_bytes == 0) {
      return cdata - data;
    }
    if (read_bytes < 0) {
      if (errno == EAGAIN || errno == EINTR) {
        continue;
      } else {
        return -1;
      }
    }
    cdata += read_bytes;
    size -= read_bytes;
    if (size == 0) {
      return cdata - data;
    }
  }
}

int write_retry(int fd, const char* data, ssize_t size) {
  const char* cdata = data;
  while (1) {
    ssize_t write_bytes = write(fd, cdata, size);
    if (write_bytes == 0) {
      return cdata - data;
    }
    if (write_bytes < 0) {
      if (errno == EAGAIN || errno == EINTR) {
        continue;
      } else {
        return -1;
      }
    }
    cdata += write_bytes;
    size -= write_bytes;
    if (size == 0) {
      return cdata - data;
    }
  }
}

ssize_t GetFileSize(int fd) {
  struct stat st;
  fstat(fd, &st);
  return st.st_size;
}

int compar(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

#define AVAILABLE_ARR_SIZE (1ULL << 22)

void CopyDataFromFileToFile(int src_d, int dest_d, ssize_t low, ssize_t high) {
  char* data = (char*)malloc(AVAILABLE_ARR_SIZE * 4);
  ssize_t bytes_to_read = AVAILABLE_ARR_SIZE * 4;
  ssize_t curr_size = high - low + 1;
  ssize_t residue = curr_size % AVAILABLE_ARR_SIZE;
  ssize_t i = 0;
  for (; i < curr_size * 4; i += bytes_to_read) {
    lseek(src_d, i, SEEK_SET);
    ssize_t bytes_read = read_retry(src_d, (char*)data, bytes_to_read);
    lseek(dest_d, low * 4 + i, SEEK_SET);
    write_retry(dest_d, (char*)data, bytes_read);
  }
  lseek(src_d, i - bytes_to_read, SEEK_SET);
  ssize_t bytes_read = read_retry(src_d, (char*)data, residue * 4);
  lseek(dest_d, low * 4 + i - bytes_to_read, SEEK_SET);
  write_retry(dest_d, (char*)data, bytes_read);
  free(data);
}

void PrintArray(const int* arr, size_t size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", arr[i]);
  }
}

void Merge(int fd, ssize_t low, ssize_t mid, ssize_t high) {
  int out_d = open("temp.dat", O_RDWR | O_CREAT,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  int* left = (int*)malloc(AVAILABLE_ARR_SIZE * 2);
  int* right = (int*)malloc(AVAILABLE_ARR_SIZE * 2);
  int* merged = (int*)malloc(AVAILABLE_ARR_SIZE * 4);

  ssize_t left_sz = mid - low + 1;
  ssize_t right_sz = high - mid;
  ssize_t bytes_to_read = AVAILABLE_ARR_SIZE * 2;
  if (bytes_to_read > (left_sz * 4)) {
    bytes_to_read = left_sz * 4;
  }
  if (bytes_to_read > (right_sz * 4)) {
    bytes_to_read = right_sz * 4;
  }
  lseek(fd, low * 4, SEEK_SET);
  ssize_t left_read = read_retry(fd, (char*)left, bytes_to_read);
  lseek(fd, (mid + 1) * 4, SEEK_SET);
  ssize_t right_read = read_retry(fd, (char*)right, bytes_to_read);

  ssize_t i = 0;
  ssize_t j = 0;
  ssize_t k = 0;
  while (i < left_sz && j < right_sz) {
    if (right[j % (AVAILABLE_ARR_SIZE / 2)] >=
        left[i % (AVAILABLE_ARR_SIZE / 2)]) {
      merged[k % AVAILABLE_ARR_SIZE] = left[i % (AVAILABLE_ARR_SIZE / 2)];
      ++i;
      if (i > 0 && (i % (AVAILABLE_ARR_SIZE / 2)) == 0) {
        bytes_to_read = (left_sz - i) * 4;
        if (bytes_to_read > AVAILABLE_ARR_SIZE * 2) {
          bytes_to_read = AVAILABLE_ARR_SIZE * 2;
        }
        lseek(fd, (low + i) * 4, SEEK_SET);
        read_retry(fd, (char*)left, bytes_to_read);
      }
    } else {
      merged[k % AVAILABLE_ARR_SIZE] = right[j % (AVAILABLE_ARR_SIZE / 2)];
      ++j;
      if (j > 0 && (j % (AVAILABLE_ARR_SIZE / 2)) == 0) {
        bytes_to_read = (right_sz - j) * 4;
        if (bytes_to_read > AVAILABLE_ARR_SIZE * 2) {
          bytes_to_read = AVAILABLE_ARR_SIZE * 2;
        }
        lseek(fd, (mid + 1 + j) * 4, SEEK_SET);
        read_retry(fd, (char*)right, bytes_to_read);
      }
    }
    ++k;
    if (k > 0 && (k % AVAILABLE_ARR_SIZE) == 0) {
      write_retry(out_d, (char*)merged, AVAILABLE_ARR_SIZE * 4);
    }
  }

  while (i < left_sz) {
    merged[k % AVAILABLE_ARR_SIZE] = left[i % (AVAILABLE_ARR_SIZE / 2)];
    ++i;
    if (i > 0 && (i % (AVAILABLE_ARR_SIZE / 2)) == 0) {
      bytes_to_read = (left_sz - i) * 4;
      if (bytes_to_read > (AVAILABLE_ARR_SIZE * 2)) {
        bytes_to_read = AVAILABLE_ARR_SIZE * 2;
      }
      lseek(fd, (low + i) * 4, SEEK_SET);
      read_retry(fd, (char*)left, bytes_to_read);
    }
    ++k;
    if (k > 0 && (k % AVAILABLE_ARR_SIZE) == 0) {
      write_retry(out_d, (char*)merged, AVAILABLE_ARR_SIZE * 4);
    }
  }

  while (j < right_sz) {
    merged[k] = right[j];
    ++j;
    if (j > 0 && (j % (AVAILABLE_ARR_SIZE / 2)) == 0) {
      bytes_to_read = (right_sz - j) * 4;
      if (bytes_to_read > (AVAILABLE_ARR_SIZE * 2)) {
        bytes_to_read = AVAILABLE_ARR_SIZE * 2;
      }
      lseek(fd, (mid + 1 + j) * 4, SEEK_SET);
      read_retry(fd, (char*)right, bytes_to_read);
    }
    ++k;
    if (k > 0 && (k % AVAILABLE_ARR_SIZE) == 0) {
      write_retry(out_d, (char*)merged, AVAILABLE_ARR_SIZE * 4);
    }
  }
  write_retry(out_d, (char*)merged, (k % AVAILABLE_ARR_SIZE) * 4);
  free(merged);
  free(left);
  free(right);
  CopyDataFromFileToFile(out_d, fd, low, high);
  close(out_d);
}

void SortFile(int fd, ssize_t low, ssize_t high) {
  if (low < high) {
    ssize_t curr_size = high - low + 1;
    if (curr_size <= AVAILABLE_ARR_SIZE) {
      ssize_t bytes_to_read = curr_size * 4;
      int* array = (int*)calloc(curr_size, sizeof(int));
      lseek(fd, low * 4, SEEK_SET);
      ssize_t bytes_read = read_retry(fd, (char*)array, bytes_to_read);
      qsort((void*)array, bytes_read / 4, sizeof(int), compar);
      lseek(fd, low * 4, SEEK_SET);
      write_retry(fd, (char*)array, bytes_read);
      free(array);
    } else {
      ssize_t middle = (low + high) / 2;
      SortFile(fd, low, middle);
      SortFile(fd, middle + 1, high);
      Merge(fd, low, middle, high);
    }
  }
}

int main(int argc, char* argv[]) {
  int fd = open(argv[1], O_RDWR | O_CREAT,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  ssize_t size = GetFileSize(fd);
  SortFile(fd, 0, (size / 4) - 1);
  close(fd);
  return 0;
}