#ifndef _MIMER_STREAM_H
#define _MIMER_STREAM_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace mimer {

class Stream
{
public:
    Stream(FILE* fp = NULL);
    Stream(const char* filename, const char* mode);
    virtual ~Stream();
public:
    bool Open(const char* filename, const char* mode);
    static bool Rename(const char* oldname, const char* newname);
    static bool Create(const char* filename, const char* mode = "w+");
    static bool Remove(const char* filename);
    virtual bool Close();
    virtual size_t	Read(void *buffer, size_t size, size_t count);
    virtual size_t	Write(const void *buffer, size_t size, size_t count);
    virtual bool Seek(int offset, int origin);
    virtual bool Rewind();
    virtual int Tell();
    virtual int	Size();
    virtual bool Flush();
    virtual bool Eof();
    virtual int	Error();
    virtual bool PutC(char c);
    virtual int	GetC();
    virtual char* GetS(char *string, int n);
    virtual int	Scanf(const char *format, void* output);
protected:
    FILE *  _fp;
    bool    _isOpen;
};


class charStream{
private:
    char*  _stream;
    int    _size;
public:
    charStream():_stream(NULL),_size(0){}
    charStream(char* data){
        _size = sizeof(data);
        _stream = (char*)malloc(_size);
        memcpy(_stream,data,_size);
    }
    charStream(char* data, int size):_size(size){
        _stream = (char*)malloc(_size);
        memcpy(_stream,data,_size);
    }
    ~charStream(){
        if(_stream){
            free(_stream);
            _stream = NULL;
        }
    }
public:
    friend std::ostream & operator<<(std::ostream &out, const charStream &cs)
    {
#define HEXCHAR (unsigned int)(unsigned char)
        int i = 0;
        out << "byte stream: ";
        while (i < cs._size) {
           out << (HEXCHAR(cs._stream[i++])) << " ";
        }
        out << std::endl;
        return out;
    }
};
}
#endif // Stream.h
