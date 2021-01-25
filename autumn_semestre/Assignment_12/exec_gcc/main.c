#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_EXP_SIZE 4096

int
write_retry (int fd, const char *data, ssize_t size)
{
  const char *cdata = data;
  while (1)
    {
      ssize_t write_bytes = write (fd, cdata, size);
      if (write_bytes == 0)
        {
          return cdata - data;
        }
      if (write_bytes < 0)
        {
          if (errno == EAGAIN || errno == EINTR)
            {
              continue;
            }
          else
            {
              return -1;
            }
        }
      cdata += write_bytes;
      size -= write_bytes;
      if (size == 0)
        {
          return cdata - data;
        }
    }
}

int
main ()
{
  char *input_exp = (char *)malloc (MAX_EXP_SIZE + 1);
  if (fgets (input_exp, MAX_EXP_SIZE + 1, stdin) == NULL)
    {
      return 0;
    }
  size_t exp_size = strlen (input_exp);
  if (input_exp[exp_size - 1] == '\n')
    {
      input_exp[exp_size - 1] = '\0';
      --exp_size;
    }
  char program_body_1[]
      = "#include <stdio.h>\n#include <math.h>\nint main(){long long a=(";
  char program_body_2[] = ");printf(\"%lld\",a);return 0;}";
  size_t fsize = strlen (program_body_1) + strlen (program_body_2) + exp_size;
  char *program_body = (char *)malloc (fsize + 1);
  program_body[0] = '\0';
  strcat (program_body, program_body_1);
  strcat (program_body, input_exp);
  strcat (program_body, program_body_2);
  int fd = open ("temp.c", O_WRONLY | O_CREAT | O_TRUNC,
                 S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  write_retry (fd, program_body, fsize);
  free (input_exp);
  free (program_body);

  pid_t compile_pid = fork ();
  if (compile_pid == 0)
    {
      execlp ("gcc", "gcc", "temp.c", "-o", "a.out", NULL);
    }
  else
    {
      int status1 = 0;
      waitpid (compile_pid, &status1, 0);

      pid_t execute_pid = fork ();
      if (execute_pid == 0)
        {
          execl ("a.out", "a.out", NULL);
        }
      else
        {
          int status2 = 0;
          waitpid (execute_pid, &status2, 0);
        }
    }
  return 0;
}