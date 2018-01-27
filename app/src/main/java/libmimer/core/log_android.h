#ifndef _ANDROID_LOG_H_
#define _ANDROID_LOG_H_
#include <android/log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "MIMER", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "MIMER", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , "MIMER", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , "MIMER", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "MIMER", __VA_ARGS__)

#endif