#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
GetFileNameFormPath (const char *path)
{
  int len = strlen (path);
  int i = len - 1;
  while (path[i] != '/' && i >= 0)
    {
      --i;
    }
  return (i == 0) ? 0 : i + 1;
}

int
main (void)
{
  struct stat st;
  char filename[PATH_MAX] = "";
  while (fgets (filename, sizeof (filename), stdin))
    {
      int len = strlen (filename);
      filename[len - 1] = '\0';
      lstat (filename, &st);
      if (S_ISLNK (st.st_mode))
        {
          char abs_link[PATH_MAX] = "";
          realpath (filename, abs_link);
          printf ("%s\n", abs_link);
        }
      else if ((st.st_mode & S_IFMT) == S_IFREG)
        {
          char link_prefix[9] = "link_to_";
          char *link_name = (char *)malloc (len + 8);
          link_name[0] = '\0';
          strcat (link_name, link_prefix);
          int realname_idx = GetFileNameFormPath (filename);
          strcat (link_name, filename + realname_idx);
          symlink (filename + realname_idx, link_name);
          free (link_name);
        }
    }
  return 0;
}