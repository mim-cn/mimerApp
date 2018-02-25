#pragma once
#ifndef _MIMCORE_H
#define _MIMCORE_H
#include "mmloop_utils.h"
#include "mmloop_files.h"
#include "mmloop.h"
#include "MIMPacket.h"
#include "mmloop_net.h"
#include "MIMProtocol.h"
#include "comdefine.h"
#include "log_android.h"
#include "ndkUtils.h"
#include <jni.h>
using namespace mm::uvbase;
using namespace mimer;
typedef  TCP  baseT;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::TCP*/
typedef  UDP  baseU;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::UDP*/
typedef  MIMProtocol baseP;

namespace mm {
    namespace MimCore {
        class MM_LOOP_API tTM;
        class MM_LOOP_API uTM;
        /*
            interface of MimCore
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
            - 传输器接口
            0. 联系 Relate   : 上层获取一个传输器
            1. 打包 Packet   : 将上层的原数据打包，为发送准备(private)
            2. 发送 Sendto   : 发送包装好的数据包
            3. 接收 Recfrm   : 发送包装好的数据包
            4. 拆包 Unpack   : 将上层的原数据打包，为发送准备(private)
            5. 取关 Unlink   : 本次结束，取消与传输器的联系
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
        class NDKCallback
        {
            public:
                NDKCallback(JavaVM* vm, JNIEnv *env, jobject obj):_JavaVM(vm),_env(env), _obj(obj) {
                    LOGD("NDKCallback init....\n");
                }
                virtual ~NDKCallback() {
                    LOGD("NDKCallback uninit....\n");
                }
            public:
                jvalue CallMethodByName(const char*method, const char* sign, int size, char* data, int type)
                {
                    LOGD("CallMethodByName: %s method: %s", sign, method);
                    // va_list args;
                    jvalue result;
                    if (_env->EnsureLocalCapacity(2) == JNI_OK)
                    {
                        jclass clazz = _env->GetObjectClass(_obj);
                        if (NULL == clazz) {
                            LOGE("Find %s error", "com/mim/mimer/sender/Sender");
                            result.z = false;
                            return result;
                        }
                        jmethodID mid = _env->GetMethodID(clazz, method, sign);
                        if (NULL == mid) {
                            LOGE("Get method: %s sign: %s", method, sign);
                            result.z = false;
                            return result;
                        }
                        _env->CallVoidMethod(_obj, mid, size,
                            ConvertCharsToJByteaArray(_env, data, size), type);
                        /*
                        if (mid) {
                            //_env->CallVoidMethod(_obj, mid, 6, ConvertCharsToJByteaArray(_env,"hello",6), 0);

                            const char *p = sign;
                            while ( *p != ')')p++;
                            p++;
                            LOGD(" Method: %s args: %s", *p, args);
                            va_start(args, sign);
                            _env->CallVoidMethod(_obj, mid, args);

                            switch (*p)
                            {
                                case 'V':
                                    _env->CallVoidMethod(_obj, mid, args);
                                    break;
                                case '[':
                                case 'L':
                                    result.l = _env->CallObjectMethodV(_obj, mid, args);
                                    break;
                                case 'Z':
                                    result.z = _env->CallBooleanMethodV(_obj, mid, args);
                                    break;
                                case 'B':
                                    result.b = _env->CallByteMethodV(_obj, mid, args);
                                    break;
                                case 'C':
                                    result.b = _env->CallCharMethodV(_obj, mid, args);
                                    break;
                                case 'S':
                                    result.s = _env->CallShortMethodV(_obj, mid, args);
                                    break;
                                case 'I':
                                    result.i = _env->CallIntMethodV(_obj, mid, args);
                                    break;
                                case 'J':
                                    result.j = _env->CallLongMethodV(_obj, mid, args);
                                    break;
                                case 'F':
                                    result.f = _env->CallFloatMethodV(_obj, mid, args);
                                    break;
                                case 'D':
                                    result.d = _env->CallDoubleMethodV(_obj, mid, args);
                                    break;
                                default:
                                    _env->FatalError("illegal sign");
                                    break;
                            }
                            va_end(args);
                        }*/
                        _env->DeleteLocalRef(clazz);
                    }
                    return result;
                }
                bool CallNativeMethod(const char* method, const char* sign)
                {
                    LOGD("CallNativeMethod JNIEnv: %p method: %s sign: %s\n", _env, method, sign);
                    if (NULL == _env){
                        LOGE("JNIEnv %p is invalid", _env);
                        if(_JavaVM && _JavaVM->AttachCurrentThread(&_env, NULL) != JNI_OK)
                        {
                            LOGD("%s: AttachCurrentThread() failed", __FUNCTION__);
                            return false;
                        }
                    }
                    if (NULL == _obj) {
                        LOGE("Object jniHandleObject: %p",_obj);
                        return false;
                    }else{
                        LOGD("Object jniHandleObject: %p",_obj);
                    }
                    jclass jniHandle = _env->GetObjectClass(_obj);
                    if (NULL == jniHandle) {
                        LOGE("Find %s error", "com/mim/mimer/sender/Sender");
                        return false;
                    }
                    jmethodID methodID = _env->GetMethodID(jniHandle, method, sign);
                    if (NULL == methodID) {
                        LOGE("Get method: %s sign: %s", method, sign);

                        return false;
                    }
                    LOGD("Call method...");
                    _env->CallVoidMethod(_obj, methodID);
                    /*
                    LOGD("free Local reference");
                    _env->DeleteLocalRef(jniHandle);
                    _env->DeleteLocalRef(jniHandleObject);
                    if(_JavaVM){
                        LOGD("DetachCurrentThread...");
                        _JavaVM->DetachCurrentThread();
                        _env = NULL;
                    }*/
                    return true;
                }
            private:
                JavaVM*  _JavaVM = NULL;
                JNIEnv*  _env = NULL;
                jobject  _obj = NULL;
        };
        class tTM :public ITM, public baseT
        {
            public:
            tTM(JavaVM* vm = NULL, JNIEnv *env = NULL, jobject obj = NULL){
                _ndkCb = new NDKCallback(vm, env, obj);
            }
            ~tTM() {
                if(_ndkCb){
                    delete(_ndkCb);
                    _ndkCb = NULL;
                }
            }
            friend class clock;
        public:
            virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
            virtual bool Create(const char* protocol = "MIM1");
            virtual int  Unlink();
            /* post */
            virtual int  Sendto(void* buf, ssize_t& count);
            /* get */
            virtual int  Recfrm(void* buf, ssize_t& count);
            /* java call c++ to write to stram */
            void writer(ssize_t size, char *buf);
        private:
            /* will be implement a client */
            virtual void OnConnected(mmerrno status);
            /* will be implement a server */
            virtual void doAccept(mmerrno status);
            /* I/O ,if read a data,will be save a buf*/
            virtual void OnRead(ssize_t nread, const char *buf);
            /* I/O ,if write a data*/
            virtual void OnWrote(mmerrno status);
        protected:
            virtual void reader() {}
            virtual void login()  {}
        private:
            Type userType;
            clock*  _pinger;
            NDKCallback* _ndkCb;
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
                        // _tmer->_loger->error("tTM %v OnTimer failed code: %v!!!", user(_tmer->userType), cbd->errcode);
                    }
                }
            }
        private:
            tTM * _tmer = NULL;
        };
    }

}
#endif
