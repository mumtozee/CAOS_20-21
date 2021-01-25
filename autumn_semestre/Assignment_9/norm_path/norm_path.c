void
CutSubstr (char *str, int begin, int end) // [begin, end)
{
  if (begin >= end)
    {
      return;
    }
  int i = 0;
  while (str[end + i] != '\0')
    {
      str[begin + i] = str[end + i];
      ++i;
    }
  str[begin + i] = '\0';
}

extern void
normalize_path (char *path)
{
  int i = 0;
  while (path[i] != '\0')
    {
      if (path[i] == '/')
        {
          int j = i + 1;
          while (path[j] == '/')
            {
              ++j;
            }
          CutSubstr (path, i + 1, j);
          if (path[i + 1] == '.' && path[i + 2] == '.' && path[i + 3] == '/')
            {
              if (i == 0)
                {
                  CutSubstr (path, i + 1, i + 4);
                }
              else
                {
                  int j = i - 1;
                  while (path[j] != '/')
                    {
                      --j;
                    }
                  CutSubstr (path, j + 1, i + 4);
                  i = j;
                }
            }
          else if (path[i + 1] == '.' && path[i + 2] == '/')
            {
              CutSubstr (path, i + 1, i + 3);
            }
          else
            {
              ++i;
            }
        }
      else
        {
          ++i;
        }
    }
}