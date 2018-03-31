#include <stdio.h>
#include <assert.h>
#include "MimCore.h"
using namespace mimer;
namespace mm {
    namespace MimCore {
        //tcp
        int  tTM::Relate(const char* addr, const int port,Type type)
        {
            Loop loop(false);
            this->userType = type;
            this->init(loop);
            LOGD("tTM is Relate addr: %s port: %d type: %s", addr, port, user(userType));
            switch (type)
            {
            case SERVER:
                this->bind(addr, port);
                this->wait();
                break;
            case CLIENT:
                this->connect(addr, port);
                break;
            case BOTH_SER:
                break;
            case BOTH_CLI:
                break;
            default:
                break;
            }
            loop.run(Loop::Default);
            return 0;
        }

        bool tTM::Create(const char* protocol)
        {
            _protocol = protocol;
            if (std::string(protocol) == "MIM1") {
                this->_monitor = new MIMProtocol(userType);
                LOGD("Create monitor %s is success", user(userType));
            }
            return true;
        }

        int  tTM::Unlink()
        {
            this->close();
            return 0;
        }

        int  tTM::Sendto(void* buf, ssize_t& count)
        {
            write((char*)buf,count);
            return count;
        }

        int  tTM::Recfrm(void* buf, ssize_t& count)
        {
            LOGD("tTM is Recfrm size: %d buf: %s", count, (char*)buf);
            return count;
        }

        //client need implement
        void tTM::OnConnected(mmerrno status) {
            assert(this->userType & 1);
            if (status != mmerrno::mmSuccess) {
                LOGE("tTM is OnConnected error: %d", status);
                _ndkCb->CallNativeMethod("error", "()V");
                return;
            }
            if (Create("MIM1")) {
                read_start();
                void* condata = NULL;
                ssize_t size = -1;
                // client connect, should send a connect packet to server
                _monitor->setPtype(CONNECT);
                Login* login = new Login();
                login->userName = "skybosi";
                login->passwd = "skybosi";
                login->willTopic = "test";
                login->willMsg = "test";
                condata = (void*)login;
                callback* cbd = _monitor->request(condata, size);
                this->login();
                if (cbd->data) {
                    Sendto(cbd->data, cbd->size);
                    _ndkCb->CallNativeMethod("success", "()I");
                    //_pinger = new clock(this, 1000, 3000);
                }
                else {
                    LOGE("User %s login in falied!!!", user(userType));
                }
            }
            else {
                LOGE("Create %s Monitor is error!!!", user(userType));
            }
        }

        //server need implement
        void tTM::doAccept(mmerrno status) {
            assert(!(this->userType & 1));
            if (status != mmerrno::mmSuccess) {
                LOGE("tTM is doAccept error: %d", status);
                return;
            }
            // create a new server for a client side service
            tTM* s2c = new tTM();
            s2c->set_packer(this->_packer);
            s2c->set_unpack(this->_unpack);
            s2c->init(loop());
            if (accept((Stream*)s2c) == 0) {
                char ip[17] = { 0 };
                int  port = 0;
                s2c->getpeerIpPort(ip, port);
                LOGD("tTM is doAccept client ip:%s port: %d", ip, port);
                // start to read data from a client
                if (s2c->Create("MIM1")) {
                    s2c->read_start();
                    LOGD("Server Monitor is runing ...");
                }
                else {
                    LOGE("Create %s Monitor is error!!!", user(userType));
                }
            }
            else {
                s2c->close();
                delete s2c;
            }
        }

        //client or server need implement, When data comes in, it is called
        void tTM::OnRead(ssize_t nread, const char *buf)
        {
            LOGD("tTM is OnRead size: %d context: %s", nread, (char*)buf);
            if (nread < 0) {
                LOGE("%s Read error %d", user(userType),nread);
                close();
                return;
            }
            void* getdata = (void*)buf;
            ssize_t size = nread;
            Recfrm(getdata, size);
            if (this->userType & 1) { //Type::CLIENT ||  Type::BOTH_CLI
                _monitor->setPtype(MIMPacket::type(buf[0]));
                if (_monitor->analyzer(getdata, size)) {
                    void* reqdata = NULL;
                    callback* cbd = _monitor->request(reqdata, size);
                    if (cbd->data) {
                        _ndkCb->CallMethodByName("onRead", "(I[BB)V", (int)cbd->size, (const char*)cbd->data, 0);
                        Sendto(cbd->data, cbd->size);
                    }
                    else {
                        _ndkCb->CallMethodByName("onRead", "(I[BB)V", 6, "hello", 0);
                        LOGE("tTM %s request failed code: %d!!!", user(userType), cbd->errcode);
                    }
                }
                else {
                    LOGE("tTM %s is get packet analyzer failed!!!", user(userType));
                }
            }
            else { //Type::SERVER ||  Type::BOTH_SER
                _monitor->setPtype(MIMPacket::type(buf[0]));
                if (_monitor->analyzer(getdata, size)) {
                    void* reqdata = NULL;
                    callback* cbd = _monitor->response(reqdata, size);
                    if (cbd->data) {
                        Sendto(cbd->data, cbd->size);
                    }
                    else {
                        LOGE("tTM %s response failed code: %d!!!", user(userType), cbd->errcode);
                    }
                }
                else {
                    LOGE("tTM %s is get packet analyzer failed!!!", user(userType));
                }
            }
        }

        /* java call c++ to write to stram */
        void tTM::writer(ssize_t size, char *buf) {
            _monitor->setPtype(PUBLISH);
            callback* cbd = _monitor->request(buf, size);
            if(cbd->data){
                Sendto(cbd->data, cbd->size);
            }
            else {
                LOGE("tTM %s request failed code: %d!!!", user(userType), cbd->errcode);
            }
        }

        //client or client need implement, when it needs to send data, it id called
        void tTM::OnWrote(mmerrno status) {
            LOGD("tTM is OnWrote... %d", status);
            if (this->userType & 1) { //Type::CLIENT ||  Type::BOTH_CLI
                LOGD("tTM is OnWrote client...");
                _ndkCb->CallNativeMethod("onWrite", "()V");
            }
            else { //Type::SERVER ||  Type::BOTH_SER
                LOGD("tTM is OnWrote server");
                _ndkCb->CallNativeMethod("onWrite", "()V");
            }
        }

        //udp
        int  uTM::Relate(const char* addr, const int port, Type type)
        {
            Loop loop(false);
            this->userType = type;
            this->_addr = addr;
            this->_port = port;
            this->init(loop);
            LOGD("uTM is Relate addr: %s port: %d type :%s", addr, port, user(userType));
            switch (type)
            {
            case SERVER:
                this->bind(addr, port, UDP::Reuseaddr);
                this->recv_start();
                break;
            case CLIENT:
                this->bind(addr, port,UDP::Reuseaddr);
                this->recv_start();
                this->Sendto((void*)"hello",6);
                break;
            case BOTH_SER:
                break;
            case BOTH_CLI:
                break;
            default:
                break;
            }
            loop.run(Loop::Default);
            return 0;
        }

        int  uTM::Unlink()
        {
            this->close();
            return 0;
        }

        int  uTM::Sendto(void* buf, size_t count)
        {
            LOGD("uTM is Sendto size: %d buf: %s", count, (char*)buf);
            send((char*)buf, count, _addr,_port);
            return count;
        }

        int  uTM::Recfrm(void* buf, size_t count)
        {
            // printf("%s recv data:%s,len = %d\n", user(userType), (char*)buf, count);
            LOGD("uTM is Recfrm size: %d buf: %s", count, (char*)buf);
            return count;
        }

        void uTM::OnSent(mmerrno status)
        {
            if (status != mmerrno::mmSuccess) {
                LOGE("uTM is OnSent Send error %d\n", status);
                return;
            }
        }

        void uTM::OnReceived(ssize_t nread, const char *buf, const struct sockaddr *addr, unsigned flags)
        {
            sockaddr_in* psin = (sockaddr_in*)addr;
            // printf("Recv from ip:%s,port:%d\n", inet_ntoa(psin->sin_addr), ntohs(psin->sin_port));
            LOGD("uTM is OnReceived from ip:%s port:%d size: %d buf: %s", inet_ntoa(psin->sin_addr), ntohs(psin->sin_port), nread, (char*)buf);
            // printf("recv data:%s,len = %d\n", buf, nread);
            Sendto((void*)buf, nread);
            //send4(buf,nread,"192.168.100.90",1800);
        }
    }
}
