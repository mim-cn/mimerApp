#include "../core/transmitter/transmitter.h"
#include "com_mim_mimer_sender_Sender.h"
#include <android/log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "MIMER", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "MIMER", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , "MIMER", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , "MIMER", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "MIMER", __VA_ARGS__)
using namespace mimer;
using namespace mm::Transmitter;

char *Jstring2CStr(JNIEnv *env, jstring jstr) {
    LOGD("============================Jstring2CStr IP:%s\n", jstr);
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1); //new char[alen+1];
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);

    return rtn;
}
/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    connect
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean
JNICALL Java_com_mim_mimer_sender_Sender_connect(JNIEnv *env, jobject obj, jstring ip, jint port) {
    LOGD("============================connect IP:%s  PORT:%d\n", ip, port);
    tTM client;
    const char *ipc = Jstring2CStr(env, ip);
    LOGD("============================connect IPC:%s  PORT:%d\n", ipc, port);
    int rt = client.Relate(ipc, 9130, CLIENT);
    LOGD("============================connect Relate:%d \n", rt);
    return true;
}

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    cfinalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_mim_mimer_sender_Sender_cfinalize
(JNIEnv * env , jobject obj )
{

}