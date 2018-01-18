#ifndef _MIMER_PROTOCOL_H
#define _MIMER_PROTOCOL_H
#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include "ellog.h"
#include "comdefine.h"
#include "MIMPacket.h"

namespace mimer {

#define tFMT(PTYPE) ((PTYPE)(_mqData->_packet))
#define BITS(FLAGS) (FLAGS.bits)
#define HEADERFLAG  (tFMT(pHeader)->bits)
#define CONNFLAG    (BITS(tFMT(pConnect)->flags))
#define CONAFLAG    (BITS(tFMT(pConnAck)->flags))
#define pVoid        reinterpret_cast<void*>

struct cmp_str
{
   bool operator()(const char *a, const char *b)
   {
      return (strcmp(a, b) < 0);
   }
};
#define CHARS     '0'
#define INTEGER   '1'
#define LIST1     '2'
#define LIST2     '3'
static std::map<std::string, char> meta =
{
    /* CONNECT */
    { "Protocol",  CHARS },
    { "version",   INTEGER },
    { "KAT",       INTEGER },
    { "clientID",  CHARS },
    { "username",  CHARS },
    { "password",  CHARS },
    { "willTopic", CHARS },
    { "willMsg",   CHARS },
    { "willRetain",INTEGER },
    /* CONNACK */
    { "clientID",  CHARS },
    { "sessionPresent", INTEGER },
    { "RC",        INTEGER },
    /* PUBLISH SUBSCRIBE SUBACK PUBACK PUBREC PUBREL PUBCOMP UNSUBACK UNSUBSCRIBE */
    { "packetId",  INTEGER },
    /* SUBSCRIBE UNSUBSCRIBE */
    { "topics",    LIST1 },
    /* PINGREQ */
    { "cstatus",   INTEGER },
    /* PUBLISH */
    { "topic",     CHARS },
    { "payload",   CHARS },
    /* SUBSCRIBE SUBACK */
    { "qoss",      LIST2  }
};

class PINGTimer;
class MIMProtocol
{
    typedef std::pair<std::string, void*> PAnalyzer;
    typedef std::map<const char*, const void*>  Analyzer;
public:
    UTIL_API MIMProtocol(Type mtype = SERVER);
    UTIL_API MIMProtocol(char* content, int mtype = 0);
    UTIL_API MIMProtocol(int ptype, int mtype = 0, int dried = 0, int dup = 0,int qos = 0);
    UTIL_API ~MIMProtocol();
public:
    inline const void* operator[](const char* key)
    {
        if(NULL != key){
            return _ctrler[key];
        }else{
            return NULL;
        }
    }
    UTIL_API callback* request(void * data, ssize_t& size);
    UTIL_API callback* response(void* data, ssize_t& size);
    UTIL_API void  setPtype(int ptype) { _ptype = ptype; }
    UTIL_API bool  analyzer(void* data, ssize_t& size);

    friend std::ostream & operator<<(std::ostream &out, const MIMProtocol &mp)
    {
        out << *(mp._mqData);
        Analyzer::const_iterator iter;
        ListSub* sublist = NULL;
        Subitor  itlist;
        ListQos* subqoss = NULL;
        Qositor  itqoss;
        for(iter = mp._ctrler.begin();iter != mp._ctrler.end(); ++iter)
        {
            switch (meta[std::string(iter->first)])
            {
            case CHARS:
                out << iter->first << " :\t" << (char*)iter->second << "\n";
                break;
            case INTEGER:
                out << iter->first << " :\t" << (int)iter->second << "\n";
                break;
            case LIST1:
                sublist = (ListSub*)(iter->second);
                itlist = sublist->begin();
                for(; itlist != sublist->end(); ++itlist){
                    out << *itlist << "\n";
                }
                break;
            case LIST2:
                subqoss = (ListQos*)(iter->second);
                itqoss = subqoss->begin();
                for(; itqoss != subqoss->end(); ++itqoss){
                    out << *itqoss << "\n";
                }
                break;
            default:
                break;
            }
        }
        return out;
    }
private:
    bool analyzer();
    bool controller();
    callback* ret(MIMPacket* pkt, void* data, ssize_t& size);
    callback* ret_err(int ptype, int errcode, void* data);
private:
    MIMPacket*  _mqData;    // packet's data
    int         _ptype;     // packet's type
    Type        _mtype;     // type
    Analyzer    _ctrler;    // save each packet's controller
    bool        _dried;
    Stream*     _stream;
    mim::ellog* _loger;
};

}//namespace mimer

#endif // MIMPROTOCOL_H
