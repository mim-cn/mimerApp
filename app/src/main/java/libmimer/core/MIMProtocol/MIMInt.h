#ifndef _MIMER_INT_H
#define _MIMER_INT_H

#include <iostream>
using namespace std;

namespace mimer {

class MIMInt
{
private:
    char _data[2];
public:
    MIMInt(){_data[0] = 0, _data[1] = 0;}
    MIMInt(const int& data){_data[0] = data & 0xf, _data[1] = data & 0xf0;}
public:
    int operator[](int i)
    {
        if( i > 15 || i < 0 )
        {
            // cout << "索引超过最大值" <<endl;
            return (int)(_data[0] & 0x1);
        }
        return (int)(_data[i/8] & (0x1 << (i%8)));
    }
    friend ostream & operator<<(ostream &out, MIMInt &rhs)
    {
        out << (int)rhs._data[1] << " " << (int)rhs._data[0];
        return out;
    }
public:
    static int encode(char* mqlen,int length)
    {
        int rc = 0;
        do
        {
            char d = length % 128;
            length /= 128;
            /* if there are more digits to encode, set the top bit of this digit */
            if (length > 0)
                d |= 0x80;
            mqlen[rc++] = d;
        } while (length > 0);
        return rc;
    }
    static int decode(char* mqlen)
    {
        int multiplier = 1;
        int value = 0;
        int pos = 0;
        int encodedByte = 0;
        do{
            encodedByte = mqlen[pos++];
            value += (encodedByte & 127) * multiplier;
            multiplier *= 128;
            if (multiplier > 128*128*128){
                return -1;
            }
        }while (encodedByte & 128);
        return value;
    }
    static int decode(char* mqlen, int& length)
    {
        int multiplier = 1;
        int value = 0;
        length = 0;
        int encodedByte = 0;
        do{
            encodedByte = mqlen[length++];
            value += (encodedByte & 127) * multiplier;
            multiplier *= 128;
            if (multiplier > 128*128*128){
                return -1;
            }
        }while (encodedByte & 128);
        return value;
    }
    static int encode_len(int length)
    {
        if(length >= (0x00) && length <= (0x7F)){
            return 1;
        } else if(length >= (0x80) && length <= (0x3FFF)){
            return 2;
        } else if(length >= (0x4000) && length <= (0x1FFFFF)){
            return 3;
        } else if(length >= (0x200000) && length <= (0xFFFFFFF)){
            return 4;
        } else{
            return 0;
        }
    }
};

}//namespace MIMer

#endif // MIMINT_H
