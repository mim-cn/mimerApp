#include "memUtil.h"

void *_calloc(size_t nmemb, size_t size)
{
    void *mem = calloc(nmemb, size);
    return mem;
}

void _free(void *mem)
{
    if(mem){
        free(mem);
        mem = NULL;
    }
}

void *_malloc(size_t size)
{
    void *mem = malloc(size);
    memset(mem, 0, size);
    return mem;
}

void *_realloc(void *ptr, size_t size)
{
    void *mem = realloc(ptr, size);
    return mem;
}

char *_strdup(const char *s)
{
    char *str = _strdup(s);
    return str;
}

