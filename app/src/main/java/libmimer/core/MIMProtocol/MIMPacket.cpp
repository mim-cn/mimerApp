#include "MIMPacket.h"
INITIALIZE_NULL_EASYLOGGINGPP
namespace mimer {

const char* MIMPacket::packet_names[] =
{
   "RESERVED", "CONNECT", "CONNACK", "PUBLISH", "PUBACK", "PUBREC", "PUBREL",
   "PUBCOMP", "SUBSCRIBE", "SUBACK", "UNSUBSCRIBE", "UNSUBACK",
   "PINGREQ", "PINGRESP", "DISCONNECT"
};

MIMPacket::MIMPacket(int type,int dried, int dup,int qos):
    _ptype(type),_size(0),_step(0),_dried(dried),_setrl(false)
{
    _loger = new mim::ellog("MIMPacket", "./logs");
    //printf("Packet Type:%s\n", packet_names[type]);
    // _loger->info("Packet Type:%v", packet_names[type]);
    switch (_ptype)
    {
    case CONNECT:
        _packet = (pConnect)_malloc(sizeof(Connect));
        memset(_packet,0,sizeof(Connect));
        pFMT(pConnect)->header.byte = 0;
        pFMT(pConnect)->header.bits.type = CONNECT;
        pFMT(pConnect)->Protocol = MIM_NAME;
        //MQNEW(pConnect,Protocol,MIM_NAME,4);
        pFMT(pConnect)->version = MIM_VER;
        _step += 3;
        /* add CONNECT Variable header, size (2-byte)*/
        _size += 2 + 4 + 1;
        break;
    case CONNACK:
        _packet = (pConnAck)_malloc(sizeof(ConnAck));
        memset(_packet,0,sizeof(ConnAck));
        pFMT(pConnAck)->header.byte = 0;
        pFMT(pConnAck)->header.bits.type = CONNACK;
        _step++;
        break;
    case PUBLISH:
        _packet = (pPublish)_malloc(sizeof(Publish));
        memset(_packet,0,sizeof(Publish));
        pFMT(pPublish)->header.byte = 0;
        pFMT(pPublish)->header.bits.type = PUBLISH;
        pFMT(pPublish)->header.bits.dup = dup;
        pFMT(pPublish)->header.bits.qos = qos;
        _step++;
        break;
    case PUBACK:
        _packet = (pPubAck)_malloc(sizeof(PubAck));
        memset(_packet,0,sizeof(PubAck));
        pFMT(pPubAck)->header.byte = 0;
        pFMT(pPubAck)->header.bits.type = PUBACK;
        _step++;
        break;
    case PUBREC:
        _packet = (pPubRec)_malloc(sizeof(PubRec));
        memset(_packet,0,sizeof(PubRec));
        pFMT(pPubRec)->header.byte = 0;
        pFMT(pPubRec)->header.bits.type = PUBREC;
        _step++;
        break;
    case PUBREL:
        _packet = (pPubRel)_malloc(sizeof(PubRel));
        memset(_packet,0,sizeof(PubRel));
        pFMT(pPubRel)->header.byte = 0;
        pFMT(pPubRel)->header.bits.type = PUBREL;
        pFMT(pPubRel)->header.bits.qos = 1;//default value
        _step++;
        break;
    case PUBCOMP:
        _packet = (pPubComp)_malloc(sizeof(PubComp));
        memset(_packet,0,sizeof(PubComp));
        pFMT(pPubComp)->header.byte = 0;
        pFMT(pPubComp)->header.bits.type = PUBCOMP;
        _step++;
        break;
    case SUBSCRIBE:
        _packet = (pSubscribe)_malloc(sizeof(Subscribe));
        memset(_packet,0,sizeof(Subscribe));
        pFMT(pSubscribe)->header.byte = 0;
        pFMT(pSubscribe)->header.bits.type = SUBSCRIBE;
        pFMT(pSubscribe)->header.bits.qos = 1;//default value
        _step++;
        break;
    case SUBACK:
        _packet = (pSubAck)_malloc(sizeof(SubAck));
        memset(_packet,0,sizeof(SubAck));
        pFMT(pSubAck)->header.byte = 0;
        pFMT(pSubAck)->header.bits.type = SUBACK;
        _step++;
        break;
    case UNSUBSCRIBE:
        _packet = (pUnsubscribe)_malloc(sizeof(Unsubscribe));
        memset(_packet,0,sizeof(Unsubscribe));
        pFMT(pUnsubscribe)->header.byte = 0;
        pFMT(pUnsubscribe)->header.bits.type = UNSUBSCRIBE;
        _step++;
        break;
    case UNSUBACK:
        _packet = (pUnsubAck)_malloc(sizeof(UnsubAck));
        memset(_packet,0,sizeof(UnsubAck));
        pFMT(pUnsubAck)->header.byte = 0;
        pFMT(pUnsubAck)->header.bits.type = UNSUBACK;
        _step++;
        break;
    case PINGREQ:
        _packet = (pPingReq)_malloc(sizeof(PingReq));
        memset(_packet,0,sizeof(PingReq));
        pFMT(pPingReq)->header.byte = 0;
        pFMT(pPingReq)->header.bits.type = PINGREQ;
        _step++;
        break;
    case PINGRESP:
        _packet = (pPingResp)_malloc(sizeof(PingResp));
        memset(_packet,0,sizeof(PingResp));
        pFMT(pPingResp)->header.byte = 0;
        pFMT(pPingResp)->header.bits.type = PINGRESP;
        _step++;
        break;
    case DISCONNECT:
        _packet = (pDisconnect)_malloc(sizeof(Disconnect));
        memset(_packet,0,sizeof(Disconnect));
        pFMT(pDisconnect)->header.byte = 0;
        pFMT(pDisconnect)->header.bits.type = DISCONNECT;
        _step++;
        break;
    default:
        //printf("error packet type\n");
        _loger->error("error packet type");
        break;
    }
    FixHeaderbits.retain = _dried; // dried flag
    if(0 == _dried && OnlyHeader){
        _size += 1;
    }
    _size += 1;
}

MIMPacket::~MIMPacket()
{
    if(_packet){
        switch (_ptype)
        {
        case CONNECT:
            //if(pFMT(pConnect)->Protocol){
            //    _free(pFMT(pConnect)->Protocol);
            //}
            if(pFMT(pConnect)->clientID){
                _free(pFMT(pConnect)->clientID);
            }
            if(pFMT(pConnect)->willTopic){
                _free(pFMT(pConnect)->willTopic);
            }
            if(pFMT(pConnect)->willMsg){
                _free(pFMT(pConnect)->willMsg);
            }
            if(pFMT(pConnect)->userName){
                _free(pFMT(pConnect)->userName);
            }
            if(pFMT(pConnect)->passwd){
                _free(pFMT(pConnect)->passwd);
            }
//            MQDEL(pConnect, Protocol);
//            MQDEL(pConnect, clientID);
//            MQDEL(pConnect, willTopic);
//            MQDEL(pConnect, willMsg);
//            MQDEL(pConnect, userName);
//            MQDEL(pConnect, passwd);
            break;
        case CONNACK:
            if(pFMT(pConnAck)->clientID){
                _free(pFMT(pConnAck)->clientID);
            }
//            MQDEL(pConnAck, clientID);
            break;
        case PUBLISH:
            if(pFMT(pPublish)->topic){
                _free(pFMT(pPublish)->topic);
            }
//            MQDEL(pPublish, topic);
            if(pFMT(pPublish)->payload){
                _free(pFMT(pPublish)->payload);
            }
//            MQDEL(pPublish, payload);
            break;
        case SUBSCRIBE:
            if(pFMT(pSubscribe)->topics){
                delete pFMT(pSubscribe)->topics;
            }
            if(pFMT(pSubscribe)->qoss){
                delete pFMT(pSubscribe)->qoss;
            }
            break;
        case SUBACK:
            if(pFMT(pSubAck)->qoss){
                delete pFMT(pSubAck)->qoss;
            }
            break;
        case UNSUBSCRIBE:
            if(pFMT(pUnsubscribe)->topics){
                delete pFMT(pUnsubscribe)->topics;
            }
            break;
        case UNSUBACK:
        case PINGREQ:
        case PINGRESP:
        case DISCONNECT:
        case PUBACK:
        case PUBREC:
        case PUBREL:
        case PUBCOMP:
            break;
        default:
            //printf("error packet type\n");
            _loger->error("error packet type");
            break;
        }
        _free(_packet);
        _packet = NULL;
    }
    if(_loger){
        delete _loger;
        _loger = NULL;
    }
}

void MIMPacket::initer(msgTypes type)
{
	Connect connect = INIT(CONNECT);
	ConnAck connack = INIT(CONNACK);
	Publish publish = INIT(PUBLISH);
	PubAck  puback  = INIT(PUBACK);
	PubRec  pubrec  = INIT(PUBREC);
	PubRel  pubrel  = INIT(PUBREL);
	PubComp pubcomp = INIT(PUBCOMP);
	Subscribe subsb = INIT(SUBSCRIBE);
	SubAck  suback  = INIT(SUBACK);
	Unsubscribe usb = INIT(UNSUBSCRIBE);
	UnsubAck unsack = INIT(UNSUBACK);
	PingReq pingreq = INIT(PINGREQ);
	PingResp presp  = INIT(PINGRESP);
	Disconnect disc = INIT(DISCONNECT);
	switch (type)
	{
	case CONNECT:
		_packet = (void*)&(connect);
		break;
	case CONNACK:
		_packet = (void*)&(connack);
		break;
	case PUBLISH:
		_packet = (void*)&(publish);
		break;
	case PUBACK:
		_packet = (void*)&(puback);
		break;
	case PUBREC:
		_packet = (void*)&(pubrec);
		break;
	case PUBREL:
		_packet = (void*)&(pubrel);
		break;
	case PUBCOMP:
		_packet = (void*)&(pubcomp);
		break;
	case SUBSCRIBE:
		_packet = (void*)&(subsb);
		break;
	case SUBACK:
		_packet = (void*)&(suback);
		break;
	case UNSUBSCRIBE:
		_packet = (void*)&(usb);
		break;
	case UNSUBACK:
		_packet = (void*)&(unsack);
		break;
	case PINGREQ:
		_packet = (void*)&(pingreq);
		break;
	case PINGRESP:
		_packet = (void*)&(presp);
		break;
	case DISCONNECT:
		_packet = (void*)&(disc);
		break;
	default:
		//printf("error packet type\n");
		_loger->error("error packet type");
		break;
	}
}

std::ostream & operator<<(std::ostream &out, const MIMPacket &mp)
{
    msgTypes ptype = (msgTypes)mp._ptype;
    ListSub* sublist = NULL;
    Subitor  itlist;
    ListQos* subqoss = NULL;
    Qositor  itqoss;
    void*    pData = mp._packet;
    out << "Packet Type:\t" << mp.packet_names[mp._ptype]
        << "["<< mp._ptype << "]\nPacket Size:\t" << mp._size
        << "\nPacket Dried:\t" << mp._dried;
    switch (ptype)
    {
    case CONNECT:
        out << "\nProtocol:\t"  << MIM_NAME << "\nversion:\t" << MIM_VER
            << "\nKATIMEOUT:\t" << FMT(pConnect,pData)->KAT
            << "\nclientID:\t"  << FMT(pConnect,pData)->clientID
            << "\nwillTopic:\t" << FMT(pConnect,pData)->willTopic
            << "\nwillMsg:\t"   << FMT(pConnect,pData)->willMsg
            << "\nuserName:\t"  << FMT(pConnect,pData)->userName
            << "\npassword:\t"  << FMT(pConnect,pData)->passwd
            << "\nMultiClient:\t" << mp.MultiConnect() << std::endl;
        break;
    case CONNACK:
        out << "\nReturn Code:\t" << FMT(pConnAck,pData)->rc
            << "\nis register:\t" << mp.signOk()
            << "\nReturn CID:\t"  << FMT(pConnAck,pData)->clientID
            << std::endl;
        break;
    case PUBLISH:
        out << "\nPub topic:\t"   << FMT(pPublish,pData)->topic
            << "\npacketID:\t"    << FMT(pPublish,pData)->packetId
            << "\npayload:\t"     << FMT(pPublish,pData)->payload << std::endl;
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        out << "\npacketId:\t" << FMT(pAck,pData)->packetId << std::endl;
        break;
    case SUBSCRIBE:
        sublist = FMT(pSubscribe,pData)->topics;
        subqoss = FMT(pSubscribe,pData)->qoss;
        itlist = sublist->begin();
        itqoss = subqoss->begin();
        for(; itlist != sublist->end() && itqoss != subqoss->end(); ++itlist,++itqoss)
        {
            out << "\nSubscribe:\t" << itlist->_content << "\tsize:" << itlist->_size
                << "\nSubQos:\t" << (int)*itqoss;
        }
        break;
    case SUBACK:
        subqoss = FMT(pSubAck,pData)->qoss;
        itqoss = subqoss->begin();
        for(; itqoss != subqoss->end(); ++itqoss)
        {
            out << "\nSubAckQos:\t" << (int)*itqoss;
        }
        break;
    case UNSUBSCRIBE:
        sublist = FMT(pUnsubscribe,pData)->topics;
        itlist = sublist->begin();
        for(; itlist != sublist->end(); ++itlist)
        {
            out << "\nUnsubscribe:\t" << itlist->_content << "\tsize:" << itlist->_size;
        }
        break;
    case PINGREQ:
        out << "\nClientStatus:\t" << mp.ClientStatus() << std::endl;
        break;
    case PINGRESP:
    case DISCONNECT:
        break;
    default:
        out << "error packet type\n";
        break;
    }
    return out << std::endl;
}

void MIMPacket::setKAT(Int kat)
{
    assert(_ptype == CONNECT);
    /* KAT size */
    _size += sizeof(int16_t);
    _step++;
    pFMT(pConnect)->KAT = kat;
    this->addRLsize();
}

void MIMPacket::setClientId(size_t size)
{
    char* clientId = uids(size);
    if(_ptype == CONNECT){
        if(_dried){
            // clientIDlen is always 16 byte,so needn't record
            _size += 16;
            pFMT(pConnect)->clientIDlen = 16;
            MQNEWS(pConnect,clientID,clientId,16);
        }else{
            // clientIDlen(2 byte) length size
            _size += (size+2);
            pFMT(pConnect)->clientIDlen = size;
            MQNEWS(pConnect,clientID,clientId,size);
        }
        _step++;
    }else if(_ptype == CONNACK){
        if(signOk()){
            if(NULL == clientId || 0 == size){
                _step--;
            }else{
                _size += 16;
                pFMT(pConnAck)->clientIDlen = 16;
                MQNEWS(pConnAck,clientID,clientId,16);
                /* clientIDlen is 16, so needn't add 1 Byte to measure clientID */
            }
        }
    }else{
        return;
    }
    _free(clientId);
    this->addRLsize();
}

void MIMPacket::setWill(const char* willtopic, const char* willmsg, size_t sizet, size_t sizem)
{
    assert(_ptype == CONNECT);
    if(pFMT(pConnect)->flags.bits.will){
        if(_dried){
            if(sizet > 0xff || sizem > 0xff){
                //printf("willtopic or willmsg too long must smaller 256!\n");
                _loger->error("willtopic or willmsg too long must smaller 256!");
                return;
            }
            _size += 2; //willTopiclen(1 byte) & willMsglen(1 byte) length size
        }else{
            _size += 4; //willTopiclen(2 byte) & willMsglen(2 byte) length size
        }
        _size += sizet;
        _size += sizem;
        pFMT(pConnect)->willTopiclen = sizet;
        MQNEWS(pConnect,willTopic,willtopic,sizet);
        pFMT(pConnect)->willMsglen = sizem;
        MQNEWS(pConnect,willMsg,willmsg,sizem);
    }else{
        pFMT(pConnect)->flags.bits.willRetain = 0;
    }
    _step += 2;
    this->addRLsize();
}

void MIMPacket::setUserName(const char* userName, size_t size)
{
    assert(_ptype == CONNECT);
    if(pFMT(pConnect)->flags.bits.username){
        if(_dried){
            if(size > 0xff){
                //printf("user name too long must smaller 256!\n");
                _loger->error("user name too long must smaller 256!");
                return;
            }
            _size++;    //userNamelen(1 byte) length size
        }else{
            _size += 2; //userNamelen(2 byte) length size
        }
        _size += size;
        pFMT(pConnect)->userNamelen = size;
        MQNEWS(pConnect,userName,userName,size);
    }
    _step++;
    this->addRLsize();
}

void MIMPacket::setPasswd(const char* passwd, size_t size)
{
    assert(_ptype == CONNECT);
    if(pFMT(pConnect)->flags.bits.password){
        if(_dried){
            if(size > 0xff){
                //printf("password too long must smaller 256!\n");
                _loger->error("password too long must smaller 256!");
                return;
            }
            _size++;    //passwdlen(1 byte) length size
        }else{
            _size += 2; //passwdlen(2 byte) length size
        }
        _size += size;
        pFMT(pConnect)->passwdlen = size;
        MQNEWS(pConnect,passwd,passwd,size);
    }
    _step++;
    this->addRLsize();
}

void MIMPacket::setSignUp()
{
    assert(_ptype == CONNECT);
    _step++;
    pFMT(pConnect)->clientIDlen = 16;
    pFMT(pConnect)->clientID = 0;
    pFMT(pConnect)->flags.bits.isregister = 1;
    _size += 16;  // default clientID 16 byte
    this->addRLsize();
}

void MIMPacket::setSignDel(const char* clientId, size_t size)
{
    if(_ptype != CONNECT){
        //printf("%s no clientId!\n", packet_names[_ptype]);
        _loger->error("%v  no clientId!", packet_names[_ptype]);
        return;
    }
    _step++;
    if(_dried){
        // clientIDlen is always 16 byte,so needn't record
        _size += 16;
        pFMT(pConnect)->clientIDlen = 16;
        MQNEWS(pConnect,clientID,clientId,16);
    }else{
        // clientIDlen(2 byte) length size
        _size += (size+2);
        pFMT(pConnect)->clientIDlen = size;
        MQNEWS(pConnect,clientID,clientId,size);
    }
    pFMT(pConnect)->flags.bits.isregister = 0;
    this->addRLsize();
}

void MIMPacket::setMultiConnect(int mc)
{
    if(_ptype != CONNECT){
        //printf("%s no clientId!\n", packet_names[_ptype]);
        _loger->error("%v  no clientId!", packet_names[_ptype]);
        return;
    }
    FixHeaderbits.dup = mc;
}

void MIMPacket::setRC(char rc)
{
    if(_ptype != CONNACK){
        //printf("%s no RC!\n", packet_names[_ptype]);
        _loger->error("%v  no RC!", packet_names[_ptype]);
        return;
    }
    _size += 1;
    _step++;
    pFMT(pConnAck)->rc = rc;
    this->addRLsize();
}

void MIMPacket::setFlags(_ubyte flags)
{
    if(!HasFlags){
        //printf("%s no flags!\n", packet_names[_ptype]);
        _loger->error("%v  no flags!", packet_names[_ptype]);
        return;
    }
    _size++;
    _step++;
    switch (_ptype) {
    case CONNECT:
        pFMT(pConnect)->flags.all = flags;
        break;
    case CONNACK:
        pFMT(pConnAck)->flags.all = flags;
        break;
    default:
        break;
    }
    this->addRLsize();
}

void MIMPacket::addTopics(char qos, const char* content, size_t size)
{
    switch (_ptype) {
    case PUBLISH:
        /* topics length's size */
        if(OFFICIAL_MIM){
            _size += 2;  // 2 byte
        }else{
            _size++;     // 1 byte
        }
        pFMT(pPublish)->topiclen = size;
        MQNEWS(pPublish,topic,content,size);
        _size += size;
        _step++;
        break;
    case SUBSCRIBE:
        /* topics length's size */
        if(_dried){
            _size++;     // 1 byte
        }else{
            _size += 2;  // 2 byte
        }
        if(NULL == pFMT(pSubscribe)->topics){
            pFMT(pSubscribe)->topics = new ListSub();
        }
        if(NULL == pFMT(pSubscribe)->qoss){
            pFMT(pSubscribe)->qoss = new ListQos();
        }
        (pFMT(pSubscribe)->topics)->push_back(sub_t(size,content));
        _size += size;
        (pFMT(pSubscribe)->qoss)->push_back(qos);
        /* qos size 1 byte */
        _size++;
        _step = QND;
        break;
    case SUBACK:
        /* topics length's size */
        if(NULL == pFMT(pSubAck)->qoss){
            pFMT(pSubAck)->qoss = new ListQos();
        }
        (pFMT(pSubAck)->qoss)->push_back(qos);
        /* qos size 1 byte */
        _size++;
        _step = QND;
        break;
    case UNSUBSCRIBE:
        /* topics length's size */
        if(_dried){
            _size++;     // 1 byte
        }else{
            _size += 2;  // 2 byte
        }
        if(NULL == pFMT(pUnsubscribe)->topics){
            pFMT(pUnsubscribe)->topics = new ListSub();
        }
        (pFMT(pUnsubscribe)->topics)->push_back(sub_t(size,content));
        _size += size;
        _step = QND;
        break;
    default:
        //printf("%s no topic!\n", packet_names[_ptype]);
        _loger->error("%v  no topic!", packet_names[_ptype]);
        return;
        break;
    }
    this->addRLsize();
}

void MIMPacket::setPayload(const char* payload, size_t size)
{
    if(_ptype != PUBLISH){
        //printf("%s no topic!\n", packet_names[_ptype]);
        _loger->error("%v  no topic!", packet_names[_ptype]);
        return;
    }
    _size += size;
    _step++;
    pFMT(pPublish)->payloadlen = size;
    MQNEWS(pPublish,payload,payload,size);
    this->addRLsize();
}

void MIMPacket::setPacketId(int packetId)
{
    assert(HasPktId);
    _size += sizeof(int16_t); // packetId's size
    _step++;
    switch (_ptype) {
    case PUBLISH://QoS > 0
        if(FixHeaderbits.qos > 0){
            pFMT(pPublish)->packetId = packetId;
        }else{
            _size -= sizeof(int16_t);
        }
        break;
    case PUBACK :
        pFMT(pPubAck)->packetId = packetId;
        break;
    case  PUBREC:
        pFMT(pPubRec)->packetId = packetId;
        break;
    case  PUBREL:
        pFMT(pPubRel)->packetId = packetId;
        break;
    case  PUBCOMP:
        pFMT(pPubComp)->packetId = packetId;
        break;
    case  SUBSCRIBE:
        pFMT(pSubscribe)->packetId = packetId;
        break;
    case  SUBACK:
        pFMT(pSubAck)->packetId = packetId;
        break;
    case  UNSUBSCRIBE:
        pFMT(pUnsubscribe)->packetId = packetId;
        break;
    case  UNSUBACK:
        pFMT(pUnsubAck)->packetId = packetId;
        break;
    default:
        break;
    }
    this->addRLsize();
}

void MIMPacket::setPingStatus(int pstatus)
{
    if(_ptype != PINGREQ){
        //printf("%s no ping status!\n", packet_names[_ptype]);
        _loger->error("%v no ping status!", packet_names[_ptype]);
        return;
    }
    pFMT(pPingReq)->header.bits.qos = pstatus;
}

bool MIMPacket::encode(char* packet)
{
    if(NULL == packet){
        //printf("packet is not a enough memory!\n");
        _loger->error("packet is not a enough memory");
        return false;
    }
    if(!finish()){
        //printf("packet is not finish, please check each part of this packet type: %s!\n", packet_names[_ptype]);
        _loger->error("packet is not finish, please check each part of this packet type: %v!", packet_names[_ptype]);
        return false;
    }

    int     cursor = 0;            /* record byte stream current's position */
    int16_t packetId = 0;          /* use to save 2 byte length packet id */
    int16_t commonshort = 0;       /* use to save 2 byte length while _dried == 0 */
    char    commonchar = 0;        /* use to save 1 byte length while _dried == 1 */

    /* use at ptype = SUBSCRIBE SUBACK */
    ListSub* sublist = NULL;
    Subitor itlist;
    ListQos* subqoss = NULL;
    Qositor itqoss;

    /* part-1 fixed header */
    packet[cursor++] = FixHeaderbyte;

    /* part-2 Remaining Length: packet size cannot !!! */
    if(CannotDried){
        /* add packet's size after encode, size MIMInt::encode's return */
        cursor += MIMInt::encode(&packet[cursor], _size-1);
        _loger->debug("CannotDried Remaining Length: packet size : %v", cursor);
    }else{
        if(!_dried){
            if(OnlyHeader){
                /* Only Header, RL == 0 */
                packet[cursor++] = 0 + 1;
                _loger->debug("OnlyHeader Remaining Length: packet size : %v", 1);
            }else{
                /* fix RL, RL == 2 */
                packet[cursor++] = 2 + 1;
                _loger->debug("fix RL Remaining Length: packet size: %v", 3);
            }
        }else{
            _loger->debug("dried op=> Remaining Length");
            //1. OnlyHeader: only fix header, delete 1-byte RL
            //2. FixRL:  fix 1-byte RL type, delete 1-byte RL
            if(OnlyHeader){
                /* Only Header, RL == 0 */
                // packet[cursor++] = 0 + 1;
            }else{
                /* fix RL, RL == 2 */
                // packet[cursor++] = 2 + 1;
            }
        }
    }

    /* part-3 Variable header & payload */
    switch (_ptype)
    {
    case PUBLISH:// TODO
        /* Variable header */
        /* topic name */
        if(OFFICIAL_MIM){
            /* topic name's size 2 byte */
            commonshort = (int16_t)pFMT(pPublish)->topiclen;
            memcpy(&packet[cursor], &commonshort, 2);
            cursor += 2;
            /* topic's content */
            memcpy(&packet[cursor], pFMT(pPublish)->topic, commonshort);
            cursor += commonshort;
            _loger->debug("PUBLISH: OFFICIAL_MIM topiclen: %v, topic content: %v", commonshort, pFMT(pPublish)->topic);
        }else{
            /* topic name's size 1 byte */
            commonchar = (char)pFMT(pPublish)->topiclen;
            //memcpy(&packet[cursor++], &commonchar, 1);
            packet[cursor++] = commonchar;
            /* topic's content */
            memcpy(&packet[cursor], pFMT(pPublish)->topic, commonchar);
            cursor += commonchar;
            _loger->debug("PUBLISH: topiclen: %v, topic content: %v", commonchar, pFMT(pPublish)->topic);
        }
        /* packet ID qos not 0 */
        if(0 != FixHeaderbits.qos){
            packetId = (int16_t)pFMT(pPublish)->packetId;
            memcpy(&packet[cursor], &packetId, 2);
            cursor += 2;
            // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
            _loger->debug("PUBLISH: packetId: %v, qos: %v", packetId, FixHeaderbits.qos);
        }
        /* payload qos, NOTE: payloadlen is not save at packet data, can use RL calculation while decode */
        commonshort = pFMT(pPublish)->payloadlen;
        memcpy(&packet[cursor], pFMT(pPublish)->payload , commonshort);
        cursor += commonshort;
        _loger->debug("PUBLISH: payloadlen: %v, payload: %v", commonshort, pFMT(pPublish)->payload);
        break;
    case SUBSCRIBE:// TODO
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pSubscribe)->packetId;
        memcpy(&packet[cursor], (char*)&packetId, 2);
        cursor += 2;
        _loger->debug("SUBSCRIBE: packetId: %v", packetId);
        /* payload */
        /* List of topic and qos */
        sublist = pFMT(pSubscribe)->topics;
        subqoss = pFMT(pSubscribe)->qoss;
        itlist = sublist->begin();
        itqoss = subqoss->begin();
        if(_dried){
            for(; itlist != sublist->end() && itqoss != subqoss->end(); ++itlist,++itqoss)
            {
                /* each topic's size 1 byte */
                commonchar = (char)(itlist->_size);
                //memcpy(&packet[cursor++], &commonchar, 1);
                packet[cursor++] = commonchar;
                /* each topic's content */
                memcpy(&packet[cursor], itlist->_content,commonchar);
                cursor += commonchar;
                /* each qos's */
                packet[cursor++] = *itqoss;
                _loger->debug("SUBSCRIBE: dried topic's size: %v, topic's content :%v , qos's: %v ", commonchar, itlist->_content, (int)*itqoss);
            }
        }else{
            for(; itlist != sublist->end() && itqoss != subqoss->end(); ++itlist,++itqoss)
            {
                /* each topic's size 2 byte */
                commonshort = (int16_t)(itlist->_size);
                memcpy(&packet[cursor], &commonshort, 2);
                cursor += 2;
                /* each topic's content */
                memcpy(&packet[cursor], itlist->_content,commonshort);
                cursor += commonshort;
                /* each qos's */
                packet[cursor++] = *itqoss;
                _loger->debug("SUBSCRIBE: topic's size: %v, topic's content :%v , qos's: %v ", commonshort, itlist->_content, (int)*itqoss);
            }
        }
        break;
    case SUBACK:// TODO
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pSubAck)->packetId;
        memcpy(&packet[cursor], (char*)&packetId, 2);
        cursor += 2;
        _loger->debug("SUBACK: packetId: %v", packetId);
        /* payload */
        /* List of subscribe return qos */
        subqoss = pFMT(pSubAck)->qoss;
        itqoss = subqoss->begin();
        for(; itqoss != subqoss->end();++itqoss)
        {
            /* each qos's */
            packet[cursor++] = *itqoss;
            _loger->debug("SUBACK: qos's: %v", (int)*itqoss);
        }
        break;
    case CONNECT:// TODO
        /* Variable header */
        /* add CONNECT Variable header, size (2-byte)*/
        packet[cursor++] = 0;
        packet[cursor++] = 4;
        /* add CONNECT MIM Protocol, size(4-byte) */
        memcpy(&packet[cursor], pFMT(pConnect)->Protocol,4);
        cursor += 4;
        _loger->debug("CONNECT: Protocol: %v, size: %v", pFMT(pConnect)->Protocol, 4);
        /* add CONNECT MIM version, size(1-byte) */
        packet[cursor++] = pFMT(pConnect)->version;
        /* add CONNECT flags, size(1-byte) */
        packet[cursor++] = pFMT(pConnect)->flags.all;
        /* add CONNECT KAT, size(2-byte) */
        memcpy(&packet[cursor], &pFMT(pConnect)->KAT,2);
        cursor += 2;
        _loger->debug("CONNECT: version: %v, CONNECT flags: %v, CONNECT KAT:%v", (int)pFMT(pConnect)->version, pFMT(pConnect)->flags.all, pFMT(pConnect)->KAT);
        /* payload */        
        if (_dried){
            /* 1. client ID */
            memcpy(&packet[cursor], pFMT(pConnect)->clientID, pFMT(pConnect)->clientIDlen);
            // memcpy(&packet[cursor], &pFMT(pConnect)->clientID, 16);
            cursor += pFMT(pConnect)->clientIDlen;
            _loger->debug("CONNECT: dried client ID: %v", pFMT(pConnect)->clientID);
            if(pFMT(pConnect)->flags.bits.will){
                /* 2. Will Topic */
                /* Will Topic's size 1 byte */
                commonchar = (char)pFMT(pConnect)->willTopiclen;
                //memcpy(&packet[cursor++], &commonchar, 1);
                packet[cursor++] = commonchar;
                /* Will Topic's content */
                memcpy(&packet[cursor], pFMT(pConnect)->willTopic, commonchar);
                cursor += commonchar;
                /* 3. Will message */
                /* Will message's size 1 byte */
                commonchar = (char)pFMT(pConnect)->willMsglen;
                //memcpy(&packet[cursor++], &commonchar, 1);
                packet[cursor++] = commonchar;
                /* Will message's content */
                memcpy(&packet[cursor], pFMT(pConnect)->willMsg, commonchar);
                cursor += commonchar;
                _loger->debug("CONNECT: dried Will Topic size: %v, Will Topic's content:%v, Will message size:%v,  Will message:%v",
                    commonchar,pFMT(pConnect)->willTopic, commonchar, pFMT(pConnect)->willMsg);
            }
            if(pFMT(pConnect)->flags.bits.username){
                /* 4. User Name */
                /* UserName's size 1 byte */
                commonchar = (char)pFMT(pConnect)->userNamelen;
                //memcpy(&packet[cursor++], &commonchar, 1);
                packet[cursor++] = commonchar;
                /* UserName's content */
                memcpy(&packet[cursor], pFMT(pConnect)->userName, commonchar);
                cursor += commonchar;
                _loger->debug("CONNECT: dried UserName: %v", pFMT(pConnect)->userName);
                if(pFMT(pConnect)->flags.bits.password){
                    /* 5. password */
                    /* password's size 1 byte */
                    commonchar = (char)pFMT(pConnect)->passwdlen;
                    //memcpy(&packet[cursor++], &commonchar, 1);
                    packet[cursor++] = commonchar;
                    /* password's content */
                    memcpy(&packet[cursor], pFMT(pConnect)->passwd, commonchar);
                    cursor += commonchar;
                    _loger->debug("CONNECT: dried password: %v", pFMT(pConnect)->passwd);
                }
            }
        }else{
            // TODO
            /* 1. client ID */
            /* clientIDlen size (2 byte) */
            commonshort = (int16_t)pFMT(pConnect)->clientIDlen;
            memcpy(&packet[cursor], &commonshort, 2);
            cursor += 2;
            /* clientID's content */
            memcpy(&packet[cursor], pFMT(pConnect)->clientID, commonshort);
            cursor += commonshort;
            _loger->debug("CONNECT: client ID: %v", pFMT(pConnect)->clientID);
            if(pFMT(pConnect)->flags.bits.will){
                /* 2. Will Topic */
                commonshort = (int16_t)pFMT(pConnect)->willTopiclen;
                memcpy(&packet[cursor], &commonshort, 2);
                /* willTopiclen size (2 byte) */
                cursor += 2;
                /* Will Topic's content */
                memcpy(&packet[cursor], pFMT(pConnect)->willTopic, commonshort);
                cursor += commonshort;
                /* 3. Will message */
                commonshort = (int16_t)pFMT(pConnect)->willMsglen;
                memcpy(&packet[cursor], &commonshort, 2);
                /* willMsglen size (2 byte) */
                cursor += 2;
                /* Will message's content */
                memcpy(&packet[cursor], pFMT(pConnect)->willMsg,commonshort);
                cursor += commonshort;
                _loger->debug("CONNECT: Will Topic size: %v, Will Topic's content:%v, Will message size:%v,  Will message:%v",
                    commonshort, (char*)pFMT(pConnect)->willTopic, commonshort, (char*)pFMT(pConnect)->willMsg);
            }
            if(pFMT(pConnect)->flags.bits.username){
                /* 4. User Name */
                commonshort = (int16_t)pFMT(pConnect)->userNamelen;
                memcpy(&packet[cursor], &commonshort, 2);
                /* userNamelen size (2 byte) */
                cursor += 2;
                /* UserName's content */
                memcpy(&packet[cursor], pFMT(pConnect)->userName,commonshort);
                cursor += commonshort;
                _loger->debug("CONNECT: UserName: %v", pFMT(pConnect)->userName);
                if(pFMT(pConnect)->flags.bits.password){
                    /* 5. password */
                    commonshort = (int16_t)pFMT(pConnect)->passwdlen;
                    memcpy(&packet[cursor], &commonshort, 2);
                    /* passwdlen size (2 byte) */
                    cursor += 2;
                    /* password's content */
                    memcpy(&packet[cursor], pFMT(pConnect)->passwd,commonshort);
                    cursor += commonshort;
                    _loger->debug("CONNECT: password: %v", pFMT(pConnect)->passwd);
                }
            }
        }
        break;
    case CONNACK:
        /* Variable header */
         packet[cursor++] = pFMT(pConnAck)->flags.all;
         packet[cursor++] = pFMT(pConnAck)->rc;
         _loger->debug("CONNECT: Variable header: %v, RC: %v", pFMT(pConnAck)->flags.all, pFMT(pConnAck)->rc);
         /* payload */
         if(signOk()){
             if(0 == pFMT(pConnAck)->clientIDlen || NULL == pFMT(pConnAck)->clientID){
                 //printf("is register, but no client ID return\n");
                 _loger->error("is register, but no client ID return");
                 return false;
             }else{
                 /* 1. client ID */
                 pFMT(pConnAck)->clientIDlen = 16;
                 memcpy(&packet[cursor], pFMT(pConnAck)->clientID, pFMT(pConnAck)->clientIDlen);
                 // memcpy(&packet[cursor], &pFMT(pConnAck)->clientID, 16);
                 cursor += pFMT(pConnAck)->clientIDlen;
                 _loger->debug("CONNECT: register ok, client ID: %v", pFMT(pConnAck)->clientID);
             }
         }
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pAck)->packetId;
        memcpy(&packet[cursor], (char*)&packetId, 2);
        // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
        cursor += 2;
        _loger->debug("%v: packet ID: %v", msgTypestr[_ptype], packetId);
        break;
    case UNSUBSCRIBE:// TODO
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pUnsubscribe)->packetId;
        memcpy(&packet[cursor], (char*)&packetId, 2);
        cursor += 2;
        _loger->debug("UNSUBSCRIBE: packet ID: %v", packetId);
        // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
        /* payload */
        /* List of unsubscribe topic*/
        sublist = pFMT(pUnsubscribe)->topics;
        itlist = sublist->begin();
        if(_dried){
            for(; itlist != sublist->end(); ++itlist)
            {
                /* each topic's size 1 byte */
                commonchar = (char)(itlist->_size);
                //memcpy(&packet[cursor++], &commonchar, 1);
                packet[cursor++] = commonchar;
                /* each topic's content */
                memcpy(&packet[cursor], itlist->_content,commonchar);
                cursor += commonchar;
                _loger->debug("UNSUBSCRIBE: dried topic's content: %v", itlist->_content);
            }
        }else{
            for(; itlist != sublist->end(); ++itlist)
            {
                /* each topic's size 2 byte */
                commonshort = (int16_t)(itlist->_size);
                memcpy(&packet[cursor], &commonshort, 2);
                cursor += 2;
                /* each topic's content */
                memcpy(&packet[cursor], itlist->_content,commonshort);
                cursor += commonshort;
                _loger->debug("UNSUBSCRIBE: topic's content: %v", itlist->_content);
            }
        }
        break;
    case PINGREQ:
    case PINGRESP:
    case DISCONNECT:
        //printf("packet type don't have Variable header && payload!\n");
        _loger->debug("%v packet type don't have Variable header && payload!", msgTypestr[_ptype]);
        break;
    default:
        //printf("error packet type\n");
        _loger->error("error packet type");
        break;
    }
    return true;
}

int  MIMPacket::decode(char* packet)
{
    if(NULL == packet || NULL ==  _packet){
        _loger->error("decode: packet is null");
        return 0;
    }
    Header fixhead;
    int    cursor = 0;           /* record byte stream current's position */
    int    rlSize = 0;           /* Remaining Length + Remaining Length's size */
    int    payloadSize = 0;      /* payload size if payload is exist */
    int    commonshort = 0;      /* use to save 2 byte length while _dried == 0 */
    int    commonchar  = 0;

    /* use at ptype = SUBSCRIBE SUBACK */
    char*  content = NULL;

    /* part-1 fixed header */
    fixhead.byte = packet[cursor++];
    FixHeaderbyte = fixhead.byte;
    _dried = fixhead.bits.retain;
    _ptype = fixhead.bits.type;
    _step  = 1;

    /* part-2  Remaining Length: packet size */
    int  prefixByte = 0;         /* save a content size's byte */
    int16_t  packetID = 0;
    if(CannotDried){
        rlSize = MIMInt::decode(&packet[cursor],prefixByte);
        _size = rlSize + 1; /* 1 is fix header's size */
        cursor += prefixByte;
        _loger->debug("CannotDried Remaining Length: packet size : %v", _size);
    }else{
        if(!_dried){
            if(OnlyHeader){
                /* Only Header, RL == 0 + RL'size(1 byte) */
                _size = packet[cursor++];
            }else{
                /* fix RL, RL == 2 + RL'size(1 byte) */
                _size = packet[cursor++];
            }
            _size++; /* 1 is fix header's size */
        }
        else{
            //1. OnlyHeader: only fix header, delete 1-byte RL value = 0
            //2. FixRL:  fix 1-byte RL type, delete 1-byte RL value = 2
            if(OnlyHeader){
                /* Only Header, RL == 0 */
                // packet[cursor++] = 0 + 1;
                _size = 1;
            }else{
                /* fix RL, RL == 2 */
                // packet[cursor++] = 2 + 1;
                _size = 1;
            }
        }
        _loger->debug("Dried Remaining Length: packet size : %v", _size);
    }
    _step++;

    /* part-3 Variable header & payload */
    switch (_ptype)
    {
    case PUBLISH:
        /* Variable header */
        /* topic name */
        if(OFFICIAL_MIM){
            /* topic name's size(2 byte) */
            memcpy(&commonshort,&packet[cursor],2);
            pFMT(pPublish)->topiclen = commonshort;
            cursor += 2;
            /* topic name's content */
            MQNEWS(pPublish,topic,&packet[cursor],commonshort);
            cursor += commonshort;
            _step++;
            _loger->debug("PUBLISH: OFFICIAL topic name's size: %v topic content : %v", 2, (char*)pFMT(pPublish)->topic);
        }else{
            /* topic name  */
            commonchar = pFMT(pPublish)->topiclen = packet[cursor++];
            MQNEWS(pPublish,topic,&packet[cursor],commonchar);
            cursor += commonchar;
            _step++;
            _loger->debug("PUBLISH: Dried topic name's size: %v topic content : %v", 1, (char*)pFMT(pPublish)->topic);
        }
        /* packet ID qos not 0 */
        if(0 != FixHeaderbits.qos){
            memcpy(&packetID,&packet[cursor],2);
            pFMT(pPublish)->packetId = packetID;
            cursor += 2;
            _step++;
            _loger->debug("PUBLISH: packetID : %v", packetID);
        }
        /* payload qos */
        payloadSize = _size - cursor;
        pFMT(pPublish)->payloadlen = payloadSize;
        MQNEWS(pPublish,payload,&packet[cursor],pFMT(pPublish)->payloadlen);
        cursor += payloadSize;
        _step++;
        _loger->debug("PUBLISH: payload size : %v payload :%v", payloadSize, (char*)pFMT(pPublish)->payload);
        break;
    case SUBSCRIBE:
        /* Variable header */
        /* packet ID */
        memcpy(&packetID,&packet[cursor],2);
        pFMT(pSubscribe)->packetId = packetID;
        cursor += 2;
        _loger->debug("SUBSCRIBE: packetID : %v", packetID);
        /* payload */
        /* List of topic and qos */
        payloadSize = rlSize - cursor;
        pFMT(pSubscribe)->topics = new ListSub();
        pFMT(pSubscribe)->qoss = new ListQos();
        if(_dried){
            while(cursor <= rlSize)
            {
                /* each topic's size */
                commonchar = packet[cursor++];
                content = (char*)_malloc(commonchar);
                memcpy(content,&packet[cursor],commonchar);
                cursor += commonchar;
                (pFMT(pSubscribe)->topics)->push_back(sub_s(commonchar,content));
                _free(content);
                /* each qos's */
                (pFMT(pSubscribe)->qoss)->push_back(packet[cursor++]);
                _loger->debug("SUBSCRIBE: Dried topic size : %v topic content: %v qos:%v", commonchar, content, packet[cursor-1]);
            }
        }else{
            while(cursor <= rlSize)
            {
                /* each topic's size */
                memcpy(&commonshort,&packet[cursor],2);
                cursor += 2;
                content = (char*)_malloc(commonshort);
                memcpy(content,&packet[cursor],commonshort);
                cursor += commonshort;
                (pFMT(pSubscribe)->topics)->push_back(sub_s(commonshort,content));
                _free(content);
                /* each qos's */
                (pFMT(pSubscribe)->qoss)->push_back(packet[cursor++]);
                _loger->debug("SUBSCRIBE: topic size : %v topic content: %v qos:%v", commonshort, content, packet[cursor - 1]);
            }
        }
        break;
    case SUBACK:
        /* Variable header */
        /* packet ID */
        memcpy(&packetID,&packet[cursor],2);
        pFMT(pSubAck)->packetId = packetID;
        cursor += 2;
        _loger->debug("SUBACK: packetID : %v", packetID);
        /* payload */
        /* List of subscribe return qos */
        pFMT(pSubAck)->qoss = new ListQos();
        payloadSize = rlSize - cursor;
        while(cursor <= rlSize)
        {
            /* each qos's */
            (pFMT(pSubAck)->qoss)->push_back(packet[cursor++]);
            _loger->debug("SUBACK: qos : %v", packet[cursor-1]);
        }
        break;
    case CONNECT:
        /* Variable header */
        /* add CONNECT Variable header, size (2-byte)*/
        /* packet[cursor++] = 0;
           packet[cursor++] = 4; */
        cursor += 2;
        /* add CONNECT MIM Protocol, size(4-byte) */
        pFMT(pConnect)->Protocol = MIM_NAME;
        //MQNEW(pConnect,Protocol,&packet[cursor],4);
        cursor += 4;
        /* add CONNECT MIM version, size(1-byte) */
        pFMT(pConnect)->version = packet[cursor++];
        /* add CONNECT flags, size(1-byte) */
        pFMT(pConnect)->flags.all = packet[cursor++];
        /* add CONNECT KAT, size(2-byte) */
        memcpy(&pFMT(pConnect)->KAT,&packet[cursor],2);
        cursor += 2;
        _step += 4;
        _loger->debug("CONNECT: Protocol : %v version: %v flags: %v KAT: %v", MIM_NAME, (int)pFMT(pConnect)->version, pFMT(pConnect)->flags.all, (int)pFMT(pConnect)->KAT);
        //pFMT(pConnect)->KAT = atoi(packetId);
        /* payload */
        if (_dried){
            /* 1. client ID */
            pFMT(pConnect)->clientIDlen = 16;
            MQNEWS(pConnect,clientID,&packet[cursor],16);
            cursor += 16;
            _step++;
            _loger->debug("CONNECT: Dried clientID : %v ", (char*)pFMT(pConnect)->clientID);
            if(pFMT(pConnect)->flags.bits.will){
                /* 2. Will Topic */
                commonchar = pFMT(pConnect)->willTopiclen = packet[cursor++];
                MQNEWS(pConnect,willTopic,&packet[cursor],commonchar);
                cursor += commonchar;
                _step++;
                /* 3. Will message */
                commonchar = pFMT(pConnect)->willMsglen = packet[cursor++];
                MQNEWS(pConnect,willMsg,&packet[cursor],commonchar);
                cursor += commonchar;
                _step += 2;
                _loger->debug("CONNECT: Dried Will Topic : %v Will message: %v", (char*)pFMT(pConnect)->willTopic, (char*)pFMT(pConnect)->willMsg);
            }
            if(pFMT(pConnect)->flags.bits.username){
                /* 4. User Name */
                commonchar = pFMT(pConnect)->userNamelen = packet[cursor++];
                MQNEWS(pConnect,userName,&packet[cursor],commonchar);
                cursor += commonchar;
                _step++;
                _loger->debug("CONNECT: Dried user name: %v", (char*)pFMT(pConnect)->userName);
                if(pFMT(pConnect)->flags.bits.password){
                    /* 5. password */
                    commonchar = pFMT(pConnect)->passwdlen = packet[cursor++];
                    MQNEWS(pConnect,passwd,&packet[cursor],commonchar);
                    cursor += commonchar;
                    _step++;
                    _loger->debug("CONNECT: Dried password: %v", (char*)pFMT(pConnect)->passwd);
                }
            }
        }else{
            // TODO
            /* 1. client ID */
            /* clientIdlen(2 byte) size */
            memcpy(&commonshort,&packet[cursor],2);
            pFMT(pConnect)->clientIDlen = commonshort;
            cursor += 2;
            MQNEWS(pConnect, clientID,&packet[cursor],commonshort);
            cursor += commonshort;
            _step++;
            _loger->debug("CONNECT: clientID : %v ", (char*)pFMT(pConnect)->clientID);
            if(pFMT(pConnect)->flags.bits.will){
                /* 2. Will Topic */
                /* willTopiclen(2 byte) size */
                memcpy(&commonshort,&packet[cursor],2);
                pFMT(pConnect)->willTopiclen = commonshort;
                cursor += 2;
                MQNEWS(pConnect,willTopic,&packet[cursor],commonshort);
                cursor += commonshort;
                _step++;
                /* 3. Will message */
                /* willMsglen(2 byte) size */
                memcpy(&commonshort,&packet[cursor],2);
                pFMT(pConnect)->willMsglen = commonshort;
                cursor += 2;
                MQNEWS(pConnect,willMsg,&packet[cursor],commonshort);
                cursor += commonshort;
                _step++;
                _loger->debug("CONNECT: Will Topic : %v Will message: %v", (char*)pFMT(pConnect)->willTopic, (char*)pFMT(pConnect)->willMsg);
            }
            if(pFMT(pConnect)->flags.bits.username){
                /* 4. User Name */
                /* userNamelen(2 byte) size */
                memcpy(&commonshort,&packet[cursor],2);
                pFMT(pConnect)->userNamelen = commonshort;
                cursor += 2;
                MQNEWS(pConnect,userName,&packet[cursor],commonshort);
                cursor += commonshort;
                _step++;
                _loger->debug("CONNECT: user name: %v", (char*)pFMT(pConnect)->userName);
                if(pFMT(pConnect)->flags.bits.password){
                    /* 5. password */
                    /* userNamelen(2 byte) size */
                    memcpy(&commonshort,&packet[cursor],2);
                    pFMT(pConnect)->passwdlen = commonshort;
                    cursor += 2;
                    MQNEWS(pConnect,passwd,&packet[cursor],commonshort);
                    cursor += commonshort;
                    _step++;
                    _loger->debug("CONNECT: password: %v", (char*)pFMT(pConnect)->passwd);
                }
            }
        }
        break;
    case CONNACK:
        /* Variable header */
        pFMT(pConnAck)->flags.all = packet[cursor++];
        pFMT(pConnAck)->rc = packet[cursor++];
        _loger->debug("CONNACK: flag: %v rc: %v", pFMT(pConnAck)->flags.all, (int)pFMT(pConnAck)->rc);
        /* payload */
        if(signOk()){
            /* 1. client ID */
            pFMT(pConnAck)->clientIDlen = 16;
            MQNEWS(pConnAck,clientID,&packet[cursor],16);
            cursor += 16;
            _loger->debug("CONNACK: is register clientID: %v", (char*)pFMT(pConnAck)->clientID);
        }
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        /* Variable header */
        /* packet ID */
        memcpy(&packetID,&packet[cursor],2);
        pFMT(pAck)->packetId = packetID;
        cursor += 2; // packetId's size
        _loger->debug("%v: packetID: %v", msgTypestr[_ptype], packetID);
        break;
    case UNSUBSCRIBE:
        /* Variable header */
        /* packet ID */
        memcpy(&packetID,&packet[cursor],2);
        pFMT(pUnsubscribe)->packetId = packetID;
        cursor += 2;
        _loger->debug("UNSUBSCRIBE: packetID: %v", packetID);
        /* payload */
        /* List of unsubscribe topic*/
        payloadSize = rlSize - cursor;
        pFMT(pUnsubscribe)->topics = new ListSub();
        if(_dried){
            while(cursor <= rlSize)
            {
                /* each topic's size */
                commonchar = packet[cursor++];
                content = (char*)_malloc(commonchar);
                memcpy(content,&packet[cursor],commonchar);
                cursor += commonchar;
                (pFMT(pUnsubscribe)->topics)->push_back(sub_s(commonchar,content));
                _free(content);
                _loger->debug("UNSUBSCRIBE: Dried topic size: %v topic content: %v", commonchar, content);
            }
        }else{
            while(cursor <= rlSize)
            {
                /* each topic's size */
                memcpy(&commonshort,&packet[cursor],2);
                cursor += 2;
                content = (char*)_malloc(commonshort);
                memcpy(content,&packet[cursor],commonshort);
                cursor += commonshort;
                (pFMT(pUnsubscribe)->topics)->push_back(sub_s(commonshort,content));
                _free(content);
                _loger->debug("UNSUBSCRIBE: topic size: %v topic content: %v", commonshort, content);
            }
        }
        break;
    case PINGREQ:
    case PINGRESP:
    case DISCONNECT:
        //printf("packet type don't have Variable header && payload!\n");
        _loger->debug("%v packet type don't have Variable header && payload!", msgTypestr[_ptype]);
        break;
    default:
        //printf("error packet type\n");
        _loger->error("error packet type");
        break;
    }
    return _size;
}

}//namespace mimer
