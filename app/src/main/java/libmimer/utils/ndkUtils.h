#ifndef  _NDK_UTILS_H
#define  _NDK_UTILS_H
#include <jni.h>
#include <malloc.h>
#include <string.h>
#include "log_android.h"

char *Jstring2CStr(JNIEnv *env, jstring jstr);
jstring CStr2Jstring(JNIEnv *env, const char *pat);
char* ConvertJByteaArrayToChars(JNIEnv *env, jbyteArray bytearray);
jbyteArray ConvertCharsToJByteaArray(JNIEnv *env, const char *ret, size_t len);
#endif