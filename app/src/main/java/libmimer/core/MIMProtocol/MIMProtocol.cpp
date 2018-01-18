#include "MIMProtocol.h"
namespace mimer {

MIMProtocol::MIMProtocol(Type mtype):_mqData(NULL),_ptype(-1),_mtype(mtype),_dried(0),_stream(NULL)
{
    _loger = new mim::ellog("MIMProtocol", "./logs");
}

MIMProtocol::MIMProtocol(char* content,int mtype):_mtype((Type)mtype),_dried(0),_stream(NULL)
{
    _loger = new mim::ellog("MIMProtocol", "./logs");
    _mqData = new MIMPacket(MIMPacket::type(content[0]));
    if(_mqData->decode(content)){
        _ptype = _mqData->type();
        _dried = tFMT(pHeaders)->header.bits.retain;
        _loger->debug("MIMPacket type: %v", _mqData->types());
    }else{
        _loger->error("MIMPacket type: %v decode failed", _mqData->types());
        printf("%s", "MIMpacket decode error!!!\n");
    }
    analyzer();
}

MIMProtocol::MIMProtocol(int ptype, int mtype, int dried, int dup,int qos):
    _ptype(ptype),_mtype((Type)mtype),_dried(dried),_stream(NULL)
{
    _loger = new mim::ellog("MIMProtocol", "./logs");
    _mqData = new MIMPacket(ptype, dried, dup, qos);
}

MIMProtocol::~MIMProtocol()
{
    if(_mqData) {
        delete _mqData;
        _mqData = NULL;
    }
    if(_stream){
        delete _stream;
        _stream = NULL;
    }
    if (_loger) {
        delete _loger;
        _loger = NULL;
    }
}

bool MIMProtocol::analyzer()
{
    if(0 >= _ptype || 15 <= _ptype /*|| false == _mqData->finish()*/){
        _loger->error("MIMPacket type: %v packet type: %v", _mqData->types(), _ptype);
        printf("%s", "Invalid packet type or incomplete packet!!!\n");
        return false;
    }
    switch (_ptype)
    {
    case PUBLISH:
        _ctrler["pubQOS"]   = pVoid(HEADERFLAG.qos);
        _ctrler["packetId"] = pVoid(tFMT(pPublish)->packetId);
        _ctrler["topic"]    = (tFMT(pPublish)->topic);
        _ctrler["payload"]  = (tFMT(pPublish)->payload);
        break;
    case SUBSCRIBE:
        _ctrler["packetId"] = pVoid(tFMT(pSubscribe)->packetId);
        _ctrler["topics"]   = (tFMT(pSubscribe)->topics);
        _ctrler["qoss"]     = (tFMT(pSubscribe)->qoss);
        break;
    case SUBACK:
        _ctrler["packetId"] = pVoid(tFMT(pSubAck)->packetId);
        _ctrler["qoss"]     = (tFMT(pSubAck)->qoss);
        break;
    case CONNECT:
        _ctrler["Protocol"] = (tFMT(pConnect)->Protocol);
        _ctrler["version"]  = pVoid(tFMT(pConnect)->version);
        _ctrler["KAT"]      = pVoid(tFMT(pConnect)->KAT);
        _ctrler["clientID"] = (tFMT(pConnect)->clientID);
        /* connect flags byte */
        if(CONNFLAG.will){
            // get willTopic and willMsg TODO
            _ctrler["willTopic"] = (tFMT(pConnect)->willTopic);
            _ctrler["willMsg"]   = (tFMT(pConnect)->willMsg);
            _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            /*
            if(CONNFLAG.willRetain){
                // must save will message after publish will message TODO
                _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            }else{
                // delete will message after publish will message TODO
                _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            }
            */
        }
        /* get username and password then Verify it, if false return error code */
        if(CONNFLAG.username && CONNFLAG.password){
            _ctrler["username"] = (tFMT(pConnect)->userName);
            _ctrler["password"] = (tFMT(pConnect)->passwd);
        }else{
            // is test user
            _ctrler["username"] = "skybosi";
            _ctrler["password"] = "skybosi";
        }
        if(CONNFLAG.isregister){
            // create a new comer
            _ctrler["newer"] = "1111111111111111";
        }
        _ctrler["multicon"] = pVoid(tFMT(pHeaders)->header.bits.dup);
        break;
    case CONNACK:
        if(CONAFLAG.isregister){
            _ctrler["clientID"] = (tFMT(pConnAck)->clientID);
        }
        if(CONAFLAG.sessionPresent){
            _ctrler["sessionPresent"] = pVoid(CONAFLAG.sessionPresent);
        }
        _ctrler["RC"] = pVoid(tFMT(pConnAck)->rc);
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        _ctrler["packetId"] = pVoid(tFMT(pAck)->packetId);
        break;
    case UNSUBSCRIBE:
        _ctrler["packetId"] = pVoid(tFMT(pUnsubscribe)->packetId);
        _ctrler["topics"]   = (tFMT(pUnsubscribe)->topics);
        break;
    case PINGREQ:
        _ctrler["cstatus"] =  pVoid(_mqData->ClientStatus());
        break;
    case PINGRESP:
    case DISCONNECT:
        //_ctrler["header"] = (tFMT(pHeaders)->header);
        _loger->warn("%v ONLY HEADER!!", MIMPacket::packet_names[_ptype]);
        break;
    default:
        printf("error packet type\n");
        break;
    }
    return true;
}

bool MIMProtocol::controller()
{
    _loger->debug("MIMProtocol type: %v", _mtype);
    return true;
}

callback* MIMProtocol::response(void * data, ssize_t& size)
{
    assert(_mtype == SERVER);
    // server (_ptype is get from client)
    MIMPacket* repk = NULL;
    switch (_ptype)
    {
    case PUBLISH:
        if (1 == (int)_ctrler["pubQOS"]) {
            repk = new MIMPacket(PUBACK);
            repk->setPacketId(1000);
        }else if(1 == (int)_ctrler["pubQOS"]) {
            repk = new MIMPacket(PUBREC);
            repk->setPacketId(1000);
            repk = new MIMPacket(PUBREL);
            repk->setPacketId(1000);
            repk = new MIMPacket(PUBCOMP);
            repk->setPacketId(1000);
        }else {
            repk = new MIMPacket(PUBACK);
            repk->setPacketId(1000);
        }
        break;
    case SUBSCRIBE:
        repk = new MIMPacket(SUBACK);
        repk->setPacketId(1);
        repk->addTopics(1);
        repk->addTopics(2);
        break;
    case SUBACK:
        _loger->warn("%v NO SUPPORT RESPONSE!!!", MIMPacket::packet_names[_ptype]);
        return ret_err(_ptype, NO_SUPPORT_RESP, data);
        break;
    case CONNECT:
        repk = new MIMPacket(CONNACK);
        cackflags caf;
        caf.bits.reserved = 0;
        caf.bits.sessionPresent = 0;
        caf.bits.isregister = 1;
        repk->setFlags(caf.all);
        repk->setRC(1);
        repk->setClientId();
        break;
    case CONNACK:
        _loger->warn("%v NO SUPPORT RESPONSE!!!", MIMPacket::packet_names[_ptype]);
        return ret_err(_ptype, NO_SUPPORT_RESP, data);
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        _loger->warn("%v NO SUPPORT RESPONSE!!!", MIMPacket::packet_names[_ptype]);
        return ret_err(_ptype, NO_SUPPORT_RESP, data);
        break;
    case UNSUBSCRIBE:
        repk->setPacketId(1000);
        break;
    case PINGREQ:
        repk = new MIMPacket(PINGRESP);
        // PINGRESP only header
        _loger->warn("%v only header!!!", MIMPacket::packet_names[_ptype]);
        break;
    case PINGRESP:
        _loger->warn("%v NO SUPPORT RESPONSE!!!", MIMPacket::packet_names[_ptype]);
        return ret_err(_ptype, NO_SUPPORT_RESP, data);
        break;
    case DISCONNECT:
        // close connection
        break;
    default:
        printf("error packet type\n");
        break;
    }
    return ret(repk, data, size);
}

callback* MIMProtocol::request(void* data, ssize_t& size)
{
    assert(_mtype == CLIENT);
    // client
    MIMPacket* reqpk = NULL;
    Login* login = NULL;
    switch (_ptype)
    {
    case PUBLISH:
        reqpk = new MIMPacket(PUBLISH);
        reqpk->addTopics(0, "test", strlen("test"));
        reqpk->setPacketId(1);
        reqpk->setPayload((char*)data, (size_t)size);
        break;
    case SUBSCRIBE:
        reqpk = new MIMPacket(SUBSCRIBE);
        reqpk->setPacketId(1);
        reqpk->addTopics(0, "hello", 6);
        reqpk->addTopics(0, "world", 5);
        reqpk->addTopics(0, "This ", 5);
        reqpk->addTopics(0, "is ", 3);
        reqpk->addTopics(0, "a ", 2);
        reqpk->addTopics(1, "Test!", 5);
        break;
    case SUBACK:
        _loger->warn("%v NO SUPPORT REQUEST!!!", MIMPacket::packet_names[_ptype]);
        return ret_err(_ptype, NO_SUPPORT_REQ, data);
        break;
    case CONNECT:
        if (data == NULL) {
            return NULL;
        }
        login = (Login*)data;
        reqpk = new MIMPacket(CONNECT);
        connflags cf;
        cf.bits.cleanstart = 1;
        cf.bits.isregister = 0;
        cf.bits.password = 1;
        cf.bits.username = 1;
        cf.bits.will = 1;
        cf.bits.willQoS = 0;
        cf.bits.willRetain = 0;
        reqpk->setFlags(cf.all);
        reqpk->setKAT(10);
        reqpk->setClientId();
        reqpk->setWill(login->willTopic, login->willMsg, strlen(login->willTopic), strlen(login->willMsg));
        reqpk->setUserName(login->userName, strlen(login->userName));
        reqpk->setPasswd(login->passwd, strlen(login->passwd));
        reqpk->setMultiConnect();
        break;
    case CONNACK:
        // connect success
        if (1 == (int)_ctrler["RC"]) {
            reqpk = new MIMPacket(PUBLISH);
            reqpk->addTopics(0, "list", strlen("list"));
            reqpk->setPacketId(1);
            reqpk->setPayload("get list", strlen("get list"));
        }
        // connect falied
        else {
            _loger->error("connect error %v", (int)_ctrler["RC"]);
            return ret_err(_ptype, (int)_ctrler["RC"], data);
        }
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
        reqpk = new MIMPacket(_ptype);
        reqpk->setPacketId(1000);
        break;
    case UNSUBACK:
        _loger->warn("%v NO SUPPORT REQUEST!!!", MIMPacket::packet_names[_ptype]);
        return ret_err(_ptype, NO_SUPPORT_REQ, data);
        break;
    case UNSUBSCRIBE:
        reqpk = new MIMPacket(UNSUBSCRIBE);
        reqpk->setPacketId(1);
        reqpk->addTopics(0, "good", 5);
        reqpk->addTopics(0, "bye", 4);
        break;
    case PINGREQ:
        reqpk = new MIMPacket(PINGREQ);
        reqpk->setPingStatus(PING_WRITING);
        break;
    case PINGRESP:
        _loger->warn("%v NO SUPPORT REQUEST!!!", MIMPacket::packet_names[_ptype]);
        return ret_err(_ptype, NO_SUPPORT_REQ, data);
        break;
    case DISCONNECT:
        reqpk = new MIMPacket(DISCONNECT);
        // disconnect only header
        break;
    default:
        printf("error packet type\n");
        break;
    }
    return ret(reqpk, data, size);
}

callback* MIMProtocol::ret(MIMPacket* pkt, void* data, ssize_t& size)
{
    if (!pkt) {
        return NULL;
    }
    size = pkt->size();
    data = (char*)malloc(size);
    memset(data, 0, size);
    callback* rtdata = new callback();
    if (!pkt->encode((char*)data)) {
        rtdata->data = NULL;
        rtdata->size = -1;
        rtdata->errcode = PACKET_ENCODE_ERROR;
    }
    else {
        rtdata->data = data;
        rtdata->size = size;
        rtdata->errcode = PACKET_ENCODE_OK;
    }
    std::cout << charStream((char*)pkt->data(), size);
    return rtdata;
}

callback* MIMProtocol::ret_err(int ptype, int errcode, void* data)
{
    if (0 >= ptype || 15 <= ptype /*|| false == _mqData->finish()*/) {
        _loger->error("MIMPacket type: %v packet type: %v", _mqData->types(), _ptype);
        return NULL;
    }
    data = malloc(sizeof(callback));
    ((callback*)data)->data = NULL;
    ((callback*)data)->errcode = errcode;
    ssize_t size = ((callback*)data)->size = sizeof(callback);
    return (callback*)data;
}

bool MIMProtocol::analyzer(void* data, ssize_t& size)
{
    char* content = (char*)data;
    _mqData = new MIMPacket(MIMPacket::type(content[0]));
    if (_mqData->decode(content)) {
        _ptype = _mqData->type();
        _dried = tFMT(pHeaders)->header.bits.retain;
        _loger->debug("MIMPacket type: %v", _mqData->types());
    }
    else {
        _loger->error("MIMPacket type: %v decode failed", _mqData->types());
        printf("%s", "MIMpacket decode error!!!\n");
        return false;
    }
    return analyzer();
}

}//namespace mimer
