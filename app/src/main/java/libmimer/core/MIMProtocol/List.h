#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Structure to hold all data for one list element
 */
typedef struct element
{
    element(void* cnt, size_t size){
        content = (void*)malloc(size);
        memcpy(content,cnt,size);
    }
    ~element(){
        if(content){
            free(content);
            content = NULL;
        }
    }
    struct element* prev,     /* pointer to previous list element */
                  * next;     /* pointer to next list element */
    void*           content;  /* pointer to element content */
} ListElement;

class List
{
public:
    List();
    ~List();
public:
    void  append(void* content);
    void  insert(void* content, ListElement* index);
    int   remove(void* content);
    void* pop();
    inline ListElement* next(ListElement** pos)
    {
        return *pos = (*pos == NULL) ? _first : (*pos)->next;
    }
    inline ListElement* prev(ListElement** pos)
    {
        return *pos = (*pos == NULL) ? _last : (*pos)->prev;
    }
    ListElement* find(void* content);
public:
    int size() {return _count;}
private:
    void _append(void* content, ListElement* newel);
private:
    ListElement* _first,    /* first element in the list */
               * _last,     /* last element in the list */
               * _current;  /* current element in the list, for iteration */
    int          _count;    /* no of items */
    //size_t       _size;     /* heap storage used */
};

#endif // List.h
