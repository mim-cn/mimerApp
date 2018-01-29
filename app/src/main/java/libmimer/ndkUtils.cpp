#include "ndkUtils.h"

char *Jstring2CStr(JNIEnv *env, jstring jstr)
{
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
    LOGD("Jstring2CStr jstr:%s char* %s\n", jstr, rtn);
    return rtn;
}


jstring CStr2Jstring(JNIEnv *env, const char *pat)
{
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID mID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray((jsize) strlen(pat));
    env->SetByteArrayRegion(bytes, 0, (jsize) strlen(pat), (jbyte *) pat); //将char* 转换为byte数组
    jstring encoding = env->NewStringUTF("GB2312");
    LOGD("CStr2Jstring char* %s\n", pat);
    return (jstring) env->NewObject(strClass, mID, bytes, encoding);
}