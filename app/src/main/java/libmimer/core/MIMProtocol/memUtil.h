#ifndef MEMUTIL_H
#define MEMUTIL_H

#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#if defined(__APPLE__)
#  include <malloc/malloc.h>
#elif defined(__FreeBSD__)
#  include <malloc_np.h>
#else
#  include <malloc.h>
#endif

void *_calloc(size_t nmemb, size_t size);
void  _free(void *mem);
void *_malloc(size_t size);
void *_realloc(void *ptr, size_t size);
char *_strdup(const char *s);

#endif // MEMUTIL_H
