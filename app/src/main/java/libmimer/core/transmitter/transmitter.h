#pragma once
#ifndef _TRANSMITTER_H
#define _TRANSMITTER_H
#include "mmloop_utils.h"
#include "mmloop_files.h"
#include "mmloop.h"
#include "MIMPacket.h"
#include "mmloop_net.h"
#include "MIMProtocol.h"
#include "comdefine.h"

using namespace mm::uvbase;
using namespace mimer;
typedef  TCP  baseT;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::TCP*/
typedef  UDP  baseU;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::UDP*/
typedef  MIMProtocol baseP;

namespace mm {
    namespace Transmitter {
        class MM_LOOP_API tTM;
        class MM_LOOP_API uTM;
        /*
            interface of Transmitter
        */
        class clock;
        class Stdio;
        class ITM
        {
            /*
            protected:
                ITM():_packer(NULL),_unpack(NULL) {}
                ~ITM() {}
            */
        public:
            typedef void* (packer)(void*, ssize_t&);
            typedef void* (unpack)(void*, ssize_t&);
        public:
            /*
            - �������ӿ�
            0. ��ϵ Relate   : �ϲ��ȡһ��������
            1. ��� Packet   : ���ϲ��ԭ���ݴ����Ϊ����׼��(private)
            2. ���� Sendto   : ���Ͱ�װ�õ����ݰ�
            3. ���� Recfrm   : ���Ͱ�װ�õ����ݰ�
            4. ��� Unpack   : ���ϲ��ԭ���ݴ����Ϊ����׼��(private)
            5. ȡ�� Unlink   : ���ν�����ȡ���봫��������ϵ
            */
            virtual int   Relate(const char* addr, const int port, Type type = SERVER) = 0;
            virtual bool  Create(const char* protocol = "MIM1") = 0;
            virtual int   Unlink() = 0;
            virtual void* Packer(void* data, ssize_t& size) {
                if (_packer) {
                    return _packer(data, size);
                }
                return data;
            }
            virtual void* Unpack(void* data, ssize_t& size) {
                if (_unpack) {
                    return _unpack(data, size);
                }
                return data;
            }
            virtual int   Sendto(void* buf, ssize_t& count) = 0;
            virtual int   Recfrm(void* buf, ssize_t& count) = 0;
        public:
            void set_packer(packer* method) { _packer = method; }
            void set_unpack(unpack* method) { _unpack = method; }
        protected:
            packer* _packer = NULL;
            unpack* _unpack = NULL;
            baseP * _monitor = NULL;
            const char* _protocol = "";
        };

        class tTM :public ITM, public baseT
        {
            /*
            public:
                tTM() {}
                ~tTM() {}
            */
            friend class clock;
            friend class Stdio;
        public:
            virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
            virtual bool Create(const char* protocol = "MIM1");
            virtual int  Unlink();
            /* post */
            virtual int  Sendto(void* buf, ssize_t& count);
            /* get */
            virtual int  Recfrm(void* buf, ssize_t& count);
        private:
            /* will be implement a client */
            virtual void OnConnected(mmerrno status);
            /* will be implement a server */
            virtual void doAccept(mmerrno status);
            /* I/O ,if read a data,will be save a buf*/
            virtual void OnRead(ssize_t nread, const char *buf);
            /* I/O ,if write a data*/
            virtual void OnWrote(mmerrno status);
        private:
            Type userType;
            clock*  _pinger;
            Stdio*  _stder;
        };
        typedef tTM TTM;

        class uTM :public ITM, public baseU
        {
            /*
            public:
                uTM() {}
                ~uTM() {}
            */
        public:
            virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
            virtual int  Unlink();
            /* post */
            virtual int  Sendto(void* buf, size_t count);
            /* get */
            virtual int  Recfrm(void* buf, size_t count);
        private:
            virtual void OnSent(mmerrno status);
            //    virtual void OnAllocate(UDP *self, size_t suggested_size, uv_buf_t *buf) {}
            virtual void OnReceived(ssize_t nread, const char *buf, const struct sockaddr *addr, unsigned flags);
        private:
            Type userType;
            const char* _addr;
            int  _port;
        };
        typedef uTM UTM;

        class clock : public Timer
        {
        public:
            clock(tTM* tmer, uint64_t timeout = 1000, uint64_t repeat = 1000) :_tmer(tmer){
                //Loop loop(false);
                Loop loop = _tmer->loop();
                this->init(loop);
                this->start(timeout, repeat);
                //loop.run(Loop::Default);
            }
            ~clock() {}
            virtual void OnTimer()
            {
                if (_tmer) {
                    void* data = NULL;
                    ssize_t size = -1;
                    _tmer->_monitor->setPtype(PINGREQ);
                    callback* cbd = _tmer->_monitor->request(data, size);
                    if (cbd->data) {
                        _tmer->Sendto(cbd->data, cbd->size);
                    }
                    else {
                        _tmer->_loger->error("tTM %v OnTimer failed code: %v!!!", user(_tmer->userType), cbd->errcode);
                    }
                }
            }
        private:
            tTM * _tmer = NULL;
        };

        class Stdio : public File
        {
        public:
            Stdio(tTM* tmer, Loop& loop) :File(loop),_tmer(tmer) {}
            ~Stdio() {}
        public:
            Stdio & operator =(int fb)
            {
                this->setfb(fb);
            }
            virtual void OnWrite(int result)
            {
                printf("OnWrite result = %d\n", result);
            }
            virtual void OnOpen(int result)
            {
                printf("open end! result = %d\n", result);
                char* buf = "1234";
                this->write(buf, 4, -1);
            }

            virtual void OnClose(int result)
            {
                printf("OnClose!\n");
                this->clean();
            }

            virtual void OnRead(char* data, int len)
            {
                if (_tmer && len > 0) {
                    ssize_t size = len;
                    void* postdata = (void*)data;
                    _tmer->_monitor->setPtype(PUBLISH);
                    callback* cbd = _tmer->_monitor->request(postdata, size);
                    if (cbd->data) {
                        _tmer->Sendto(cbd->data, size);
                    }
                    else {
                        _tmer->_loger->error("tTM %v request failed code: %v!!!", user(_tmer->userType), cbd->errcode);
                    }
                }
            }
        private:
            tTM * _tmer = NULL;
        };
    }

}
#endif
