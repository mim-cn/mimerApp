#include <jni.h>
#include "log_android.h"
#include "MimCore.h"
#include "ndkUtils.h"

static mm::MimCore::tTM*   client = NULL;
static JavaVM* g_JavaVM = NULL;
/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    connect
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Relater(JNIEnv *env, jobject obj, jstring ip, jint port) {
    client = new mm::MimCore::tTM(g_JavaVM, env, obj);
    const char *ipc = Jstring2CStr(env, ip);
    client->Relate(ipc, port, CLIENT);
    LOGD("Relater IP:%s PORT: %d\n", ipc, port);
    return true;
}

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    Login
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Loginer(JNIEnv * env, jobject obj, jstring token, jstring passwd, jint passwdLen)
{

}

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    Write
 * Signature: (I[B)V
 */
JNIEXPORT void JNICALL Writer(JNIEnv * env, jobject obj, jint size, jbyteArray buf)
{
    LOGD("===Writer===\n");
    char *cbuf = ConvertJByteaArrayToChars(env, buf);
    client->writer(size, cbuf);
}

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    Read
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Reader(JNIEnv * env, jobject obj, jint nread, jstring buf)
{

}
/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    cfinalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL finalizer(JNIEnv * env , jobject obj)
{

}

// 指定要注册的类，对应完整的java类名
#define JNIREG_CLASS "com/mim/mimer/sender/Sender"

// Java和JNI函数的绑定表
static JNINativeMethod method_table[] = {
        { "Relate","(Ljava/lang/String;I)Z", (void*)Relater },
        { "Login","(Ljava/lang/String;Ljava/lang/String;I)Z", (void*)Loginer },
        { "Write","(I[B)V", (void*)Writer },
        { "Read", "(ILjava/lang/String;)V", (void*)Reader },
        { "cfinalize", "()V", (void*)finalizer }
};

// 注册native方法到java中
static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods)
{
    LOGD("==== registerNativeMethods ===\n");
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGD("registerNativeMethods FindClass:%d\n", JNI_FALSE);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGD("registerNativeMethods RegisterNatives:%d\n", JNI_FALSE);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

int register_ndk_load(JNIEnv *env)
{
    // 调用注册方法
    return registerNativeMethods(env, JNIREG_CLASS, method_table, sizeof(method_table) / sizeof(method_table[0]));
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGD("==== JNI_OnLoad ===\n");
    g_JavaVM = vm;
    JNIEnv* env = NULL;
    jint result = -1;
    jint rt = -1;
    if ((rt = vm->GetEnv((void**) &env, JNI_VERSION_1_4)) != JNI_OK)
    {
        LOGD("JNI_OnLoad GetEnv %d\n", rt);
        return result;
    }
    if( !(rt = register_ndk_load(env))){
        LOGE("JNI_OnLoad registerNativeMethods %d\n", rt);
    }
    // 返回jni的版本
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
{
    LOGD("==== JNI_OnUnload ===\n");
}