#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

size_t
GetFileSize (int fd)
{
  struct stat s;
  fstat (fd, &s);
  return s.st_size;
}

struct Block
{
  size_t size;
  struct Block *next;
  struct Block *prev;
} __attribute__ ((__packed__));

char *memory = NULL;
size_t fsize = 0;
size_t free_space = 0;
struct Block *head = NULL;

void
myalloc_initialize (int fd)
{
  fsize = GetFileSize (fd);
  memory = mmap (NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  free_space = fsize;
  head = (struct Block *)memory;
  head->size = fsize - sizeof (struct Block);
  head->next = NULL;
  head->prev = NULL;
}

void
myalloc_finalize ()
{
  munmap (memory, fsize);
}

void
DeleteBlockFromList (struct Block *del)
{
  if (del == head)
    {
      head = del->next;
    }
  if (del->next)
    {
      del->next->prev = del->prev;
    }
  if (del->prev)
    {
      del->prev->next = del->next;
    }
}

void *
my_malloc (size_t size)
{
  struct Block *curr = head;
  while (curr)
    {
      ssize_t cur_sz = curr->size;
      if (cur_sz > size + sizeof (struct Block))
        {
          struct Block *new_block
              = (struct Block *)((char *)curr + sizeof (struct Block) + size);
          new_block->size = cur_sz - size - sizeof (struct Block);
          new_block->next = curr->next;
          new_block->prev = curr;
          curr->next = new_block;
          curr->size = size;
        }

      if ((cur_sz > size && size + sizeof (struct Block) >= size)
          || cur_sz > size + sizeof (struct Block) || cur_sz == size)
        {
          DeleteBlockFromList (curr);
          return (void *)(curr + 1);
        }
      curr = curr->next;
    }
  if (!curr)
    {
      return NULL;
    }
}

int
AreAdjacent (struct Block *lhs, struct Block *rhs)
{
  char *lptr = (char *)lhs;
  char *rptr = (char *)rhs;
  if (lptr + sizeof (struct Block) + lhs->size == rptr)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

void
MergeBlockWithNext (struct Block *cur)
{
  struct Block *tmp = cur->next;
  cur->next = cur->next->next;
  if (cur->next)
    {
      cur->next->prev = cur;
    }
  cur->size += tmp->size + sizeof (struct Block);
}

void
my_free (void *ptr)
{
  if (!head)
    {
      head = (struct Block *)ptr - 1;
      head->prev = NULL;
      head->next = NULL;
      return;
    }
  char *cptr = (char *)ptr;
  struct Block *curr = head;
  while (curr && curr->next)
    {
      if ((char *)curr < cptr && (char *)curr->next > cptr)
        {
          struct Block *middle = (struct Block *)ptr - 1;
          if (curr)
            middle->next = curr->next;
          curr->next->prev = middle;
          middle->prev = curr;
          curr->next = middle;
          if (AreAdjacent (curr, middle))
            {
              MergeBlockWithNext (curr);
              if (AreAdjacent (curr, curr->next))
                {
                  MergeBlockWithNext (curr);
                }
            }
          else if (AreAdjacent (middle, middle->next))
            {
              MergeBlockWithNext (middle);
            }
          return;
        }
      if ((char *)curr > cptr && (char *)curr->next > cptr)
        {
          struct Block *new_head = (struct Block *)ptr - 1;
          new_head->prev = NULL;
          new_head->next = curr;
          curr->prev = new_head;
          if (AreAdjacent (new_head, curr))
            {
              MergeBlockWithNext (new_head);
            }
          head = new_head;
          return;
        }
      curr = curr->next;
    }
  if (curr && !curr->next)
    {
      struct Block *new = (struct Block *)ptr - 1;
      if (new > curr)
        {
          new->next = NULL;
          new->prev = curr;
          curr->next = new;
          if (AreAdjacent (curr, new))
            {
              MergeBlockWithNext (curr);
            }
        }
      else if (new < curr)
        {
          new->prev = NULL;
          new->next = curr;
          curr->prev = new;
          if (AreAdjacent (new, curr))
            {
              MergeBlockWithNext (new);
            }
          head = new;
        }
    }
}
