#include "Stream.h"

namespace mimer{

Stream::Stream(FILE* fp)
{
    _fp = fp;
    _isOpen = (bool)(0 == fp);
}

Stream::Stream(const char* filename, const char* mode):_fp(NULL),_isOpen(false)
{
    Open(filename,mode);
}

Stream::~Stream()
{
    Close();
}

bool Stream::Open(const char* filename, const char* mode)
{
    if (_fp) return false;	// Can't re-open without closing first
    _fp = fopen(filename, mode);
    if (!_fp) return false;
    _isOpen = true;
    return true;
}

bool Stream::Rename(const char* oldname, const char* newname)
{
    if(rename(oldname, newname) == 0)
        return true;
    return false;
}

bool Stream::Create(const char* filename, const char* mode)
{
    FILE* fp = fopen(filename, mode);
    if(fp){
        fclose(fp);
        return true;
    }
    return false;
}

bool Stream::Remove(const char* filename)
{
    if(remove(filename) == 0)
        return true;
    return false;
}

bool Stream::Close()
{
    int iErr = 0;
    if ((_fp) && (_isOpen)){
        iErr = fclose(_fp);
        _fp = NULL;
    }
    return (bool)(iErr==0);
}

size_t Stream::Read(void *buffer, size_t size, size_t count)
{
    if (!_fp) return 0;
    return fread(buffer, size, count, _fp);
}

size_t Stream::Write(const void *buffer, size_t size, size_t count)
{
    if (!_fp) return 0;
    return fwrite(buffer, size, count, _fp);
}

bool Stream::Seek(int offset, int origin)
{
    if (!_fp) return false;
    return (bool)(fseek(_fp, offset, origin) == 0);
}

bool Stream::Rewind()
{
    if (!_fp) return false;
    rewind(_fp);
    return true;
}

int Stream::Tell()
{
    if (!_fp) return 0;
    return ftell(_fp);
}

int	Stream::Size()
{
    if (!_fp) return -1;
    int pos,size;
    pos = ftell(_fp);
    fseek(_fp, 0, SEEK_END);
    size = ftell(_fp);
    fseek(_fp, pos,SEEK_SET);
    return size;
}

bool Stream::Flush()
{
    if (!_fp) return false;
    return (bool)(fflush(_fp) == 0);
}

bool Stream::Eof()
{
    if (!_fp) return true;
    return (bool)(feof(_fp) != 0);
}

int	Stream::Error()
{
    if (!_fp) return -1;
    return ferror(_fp);
}

bool Stream::PutC(char c)
{
    if (!_fp) return false;
    return (bool)(fputc(c, _fp) == c);
}

int	Stream::GetC()
{
    if (!_fp) return EOF;
    return getc(_fp);
}

char* Stream::GetS(char *string, int n)
{
    if (!_fp) return NULL;
    return fgets(string,n,_fp);
}

int	Stream::Scanf(const char *format, void* output)
{
    if (!_fp) return EOF;
    return fscanf(_fp, format, output);
}

}