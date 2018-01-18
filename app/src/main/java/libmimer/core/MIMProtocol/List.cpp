#include "List.h"

List::List():_count(0){
    _first   = (ListElement*)malloc(sizeof(ListElement));
    _last    = (ListElement*)malloc(sizeof(ListElement));
    _current = (ListElement*)malloc(sizeof(ListElement));
}

List::~List(){
    while (_first != NULL)
    {
        ListElement* first = _first;
        if (first->content != NULL)
            free(first->content);
        _first = first->next;
        free(first);
    }
    _count = 0;
    _current = _first = _last = NULL;
}

void  List::append(void* content)
{
    ListElement* newel = (ListElement*)malloc(sizeof(ListElement));
    _append(content,newel);
}

void  List::insert(void* content, ListElement* index)
{
    ListElement* newel = (ListElement*)malloc(sizeof(ListElement));
    if ( index == NULL )
        _append(content, newel);
    else
    {
        newel->content = content;
        newel->next = index;
        newel->prev = index->prev;

        index->prev = newel;
        if ( newel->prev != NULL )
            newel->prev->next = newel;
        else
            _first = newel;
        ++(_count);
    }
}

int   List::remove(void* content)
{
    ListElement* next = NULL;
    ListElement* saved = _current;
    int freeContent = 1;
    int saveddeleted = 0;

    if (!find(content))
        return 0; /* false, did not remove item */

    if (_current->prev == NULL)
        /* so this is the first element, and we have to update the "first" pointer */
        _first = _current->next;
    else
        _current->prev->next = _current->next;

    if (_current->next == NULL)
        _last = _current->prev;
    else
        _current->next->prev = _current->prev;

    next = _current->next;
    if (freeContent)
        free(_current->content);
    if (saved == _current)
        saveddeleted = 1;
    free(_current);
    if (saveddeleted)
        _current = next;
    else
        _current = saved;
    --(_count);
    return 1; /* successfully removed item */
}

void* List::pop()
{
    void* content = NULL;
    if (_count > 0)
    {
        ListElement* last = _last;
        if (_current == last)
            _current = last->prev;
        if (_first == last) /* i.e. no of items in list == 1 */
            _first = NULL;
        content = last->content;
        _last = _last->prev;
        if (_last)
            _last->next = NULL;
        free(last);
        --(_count);
    }
    return content;
}

ListElement* List::find(void* content)
{
    ListElement* rc = NULL;
    if (_current != NULL)
        rc = _current;
    else
    {
        ListElement* current = NULL;
        /* find the content */
        while (next(&current) != NULL)
        {
            if (current->content == content)
            {
                rc = current;
                break;
            }
        }
        if (rc != NULL)
            _current = rc;
    }
    return rc;
}

void List::_append(void* content, ListElement* newel)
{
    newel->content = content;
    newel->next = NULL;
    newel->prev = _last;
    if (_first == NULL)
        _first = newel;
    else
        _last->next = newel;
    _last = newel;
    ++(_count);
}
