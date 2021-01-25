#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void CompareAndPrint(size_t i, size_t N) {
  if (i == N) {
    printf("%ld\n", i);
  } else {
    printf("%ld ", i);
  }
}

int main(int argc, char* argv[]) {
  size_t N = strtoull(argv[1], NULL, 10);
  CompareAndPrint(1, N);
  fflush(stdout);
  for (size_t i = 2; i <= N; ++i) {
    pid_t pid = fork();
    fflush(stdout);
    if (pid == 0) {
      CompareAndPrint(i, N);
      return 0;
    }
    if (pid > 0) {
      int status = 0;
      waitpid(pid, &status, 0);
    }
  }
  return 0;
}