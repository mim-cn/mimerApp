#include "transmitter.h"
#include "com_mim_mimer_sender_Sender.h"
#include "log_android.h"
#include "ndkUtils.h"

using namespace mimer;
mm::Transmitter::tTM* client = NULL;

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    connect
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_mim_mimer_sender_Sender_Relate(JNIEnv *env, jobject obj, jstring ip, jint port) {
    client = new mm::Transmitter::tTM();
    const char *ipc = Jstring2CStr(env, ip);
    int rt = client->Relate(ipc, port, CLIENT);
    return true;
}

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    Login
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_mim_mimer_sender_Sender_Login(JNIEnv * env, jobject obj, jstring token, jstring passwd, jint passwdLen)
{

}

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    Write
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_mim_mimer_sender_Sender_Write(JNIEnv * env, jobject obj, jint nread, jstring buf)
{

}

/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    Read
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_mim_mimer_sender_Sender_Read(JNIEnv * env, jobject obj, jint nread, jstring buf)
{

}
/*
 * Class:     com_mim_mimer_sender_Sender
 * Method:    cfinalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_mim_mimer_sender_Sender_cfinalize(JNIEnv * env , jobject obj )
{

}