/* An horrible dummy malloc */

#include <sys/types.h>
#include <unistd.h>

void *malloc(size_t size)
{
  void *p;
  p = sbrk(0);

  /* If sbrk fails, we return NULL */
  if (sbrk(size) == (void*) -1)
    return NULL;

  return p;
}
