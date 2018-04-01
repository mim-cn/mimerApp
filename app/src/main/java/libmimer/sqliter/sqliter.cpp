//
// Created by skybosi on 2018/3/25.
//
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sqlite3.h>
#include <Column.h>
#include <Database.h>
#include <SQLiteCpp.h>
#include <VariadicBind.h>
#include <jni.h>
#include "log_android.h"
#include "ndkUtils.h"

static JavaVM *g_JavaVM = NULL;
static SQLite::Database* db = NULL;
static SQLite::Statement* query = NULL;

/*
   * Class:     com_mim_database_sqliter
   * Method:    connect
   * Signature: (Ljava/lang/String;I)Z
   */
JNIEXPORT jboolean JNICALL connect(JNIEnv *env, jobject obj, jstring db_name, jint mode)
{
    try{
        LOGD("connect database %s mode: %d", Jstring2CStr(env, db_name), mode);
        db = new SQLite::Database(Jstring2CStr(env, db_name), mode);
        return true;
    }
    catch (std::exception& e)
    {
        LOGE("connect database %s is exception: %s", Jstring2CStr(env, db_name), e.what());
        return false; // unexpected error : exit the example program
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    version
   * Signature: ()Ljava/lang/String;
   */
JNIEXPORT jstring JNICALL version(JNIEnv *env, jobject obj)
{
    LOGE("version: %s", SQLITE_VERSION);
    return env->NewStringUTF(SQLITE_VERSION);//   CStr2Jstring(env, SQLITE_VERSION);
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    tableCheck
   * Signature: (Ljava/lang/String;)Z
   */
JNIEXPORT jboolean JNICALL tableCheck(JNIEnv * env, jobject obj, jstring tb_name)
{
    try{
        if(db){
            return db->tableExists(Jstring2CStr(env, tb_name));;
        }else{
            return false;
        }
    }
    catch (std::exception& e)
    {
        LOGE("tableCheck %s is exception: %s", Jstring2CStr(env, tb_name), e.what());
        return false; // unexpected error : exit the example program
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    executeSql
   * Signature: (Ljava/lang/String;)Z
   */
JNIEXPORT jboolean JNICALL executeSql(JNIEnv * env, jobject obj, jstring ssql)
{
    try{
        if(db){
            char* sql = Jstring2CStr(env, ssql);
            LOGD("executeSql query: %s", sql);
            //db->exec(sql);
            if(query){
                LOGD("delete old execute query: %s", sql);
                delete query;
                query = NULL;
            }
            query = new SQLite::Statement(*db, sql);
            return true;
        }else{
            LOGE("execute query failed, database is NULL");
            return false;
        }
    }
    catch (std::exception& e)
    {
        LOGE("execute query failed, database is exception: %s", e.what());
        return false; // unexpected error : exit the example program
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    exec
   * Signature: (Ljava/lang/String;)Z
   */
JNIEXPORT jboolean JNICALL exec(JNIEnv * env, jobject obj, jstring ssql)
{
    try{
        if(db){
            char* sql = Jstring2CStr(env, ssql);
            int rn = db->exec(sql);
            LOGD("exec query: %s %d", sql, rn);
            return true;
        }else{
            LOGE("execute query failed, database is NULL");
            return false;
        }
    }
    catch (std::exception& e)
    {
        LOGE("execute query failed, database is exception: %s", e.what());
        return false; // unexpected error : exit the example program
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    next
   * Signature: ()Z
   */
JNIEXPORT jboolean JNICALL next(JNIEnv *env, jobject obj)
{
    try{
        if(db && query){
            return query->executeStep();
        }else{
            LOGE("next: database or query is NULL");
            return false;
        }
    }
    catch (std::exception& e)
    {
        LOGE("next: database or query is is exception: %s", e.what());
        return false; // unexpected error : exit the example program
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnBool
   * Signature: (I)Z
   */
JNIEXPORT jboolean JNICALL getColumnBoolByIndex(JNIEnv * env, jobject obj, jint index)
{
    if(db && query){
        return query->getColumn(index).getInt();
    }else{
        LOGE("getColumnBoolByIndex: database or query is NULL");
        return false;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnByte
   * Signature: (I)B
   */
JNIEXPORT jbyte JNICALL getColumnByteByIndex(JNIEnv *env, jobject obj, jint index)
{
    if(db && query){
        return query->getColumn(index).getInt();
    }else{
        LOGE("getColumnByteByIndex: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnChar
   * Signature: (I)C
   */
JNIEXPORT jchar JNICALL getColumnCharByIndex(JNIEnv * env, jobject obj, jint index)
{
    if(db && query){
        return (char)query->getColumn(index).getInt();
    }else{
        LOGE("getColumnCharByIndex: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnInt
   * Signature: (I)I
   */
JNIEXPORT jint JNICALL getColumnIntByIndex(JNIEnv * env, jobject obj, jint index)
{
    if(db && query){
        return query->getColumn(index).getInt();
    }else{
        LOGE("getColumnIntByIndex: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnLong
   * Signature: (I)J
   */
JNIEXPORT jlong JNICALL getColumnLongByIndex(JNIEnv * env, jobject obj, jint index)
{
    if(db && query){
        return query->getColumn(index).getInt64();
    }else{
        LOGE("getColumnLongByIndex: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnDouble
   * Signature: (I)D
   */
JNIEXPORT jdouble JNICALL getColumnDoubleByIndex(JNIEnv * env, jobject obj, jint index)
{
    if(db && query){
        return query->getColumn(index).getDouble();
    }else{
        LOGE("getColumnDoubleByIndex: database or query is NULL");
        return 0;
    }
}

/*
 * Class:     com_mim_database_sqliter
 * Method:    getColumnStringByIndex
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL getColumnStringByIndex(JNIEnv *env, jobject obj, jint index)
{
    if(db && query){
        const char* ret = query->getColumn(index).getString().c_str();
        LOGE("getColumnStringByIndex: %s", ret);
        size_t len  = strlen(ret);
        return ConvertCharsToJByteaArray(env, ret, len);
    }else{
        LOGE("getColumnStringByIndex: database or query is NULL");
        return NULL;
    }
}

/*
 * Class:     com_mim_database_sqliter
 * Method:    getColumnTextByIndex
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL getColumnTextByIndex(JNIEnv * env, jobject obj, jint index)
{
    if(db && query){
        const char* ret = query->getColumn(index).getText();
        size_t len  = strlen(ret);
        return ConvertCharsToJByteaArray(env, ret, len);
    }else{
        LOGE("getColumnTextByIndex: database or query is NULL");
        return NULL;
    }
}

/*
 * Class:     com_mim_database_sqliter
 * Method:    getColumnBlobByIndex
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL getColumnBlobByIndex(JNIEnv *env, jobject obj, jint index)
{
    if(db && query){
        // return CStr2Jstring(env, query->getColumn(index).getText().c_str());
    }else{
        LOGE("getColumnBlobByIndex: database or query is NULL");
        return NULL;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnBool
   * Signature: (Ljava/lang/String;)Z
   */
JNIEXPORT jboolean JNICALL getColumnBool(JNIEnv * env, jobject obj, jstring fname)
{
     if(db && query){
         return query->getColumn(Jstring2CStr(env, fname)).getInt();
     }else{
         LOGE("getColumnBool: database or query is NULL");
         return false;
     }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnByte
   * Signature: (Ljava/lang/String;)B
   */
JNIEXPORT jbyte JNICALL getColumnByte(JNIEnv *env, jobject obj, jstring fname)
{
    if(db && query){
        return query->getColumn(Jstring2CStr(env, fname)).getInt();
    }else{
        LOGE("getColumnByte: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnChar
   * Signature: (Ljava/lang/String;)C
   */
JNIEXPORT jchar JNICALL getColumnChar(JNIEnv *env, jobject obj, jstring fname)
{
    if(db && query){
        return (char)query->getColumn(Jstring2CStr(env, fname)).getInt();
    }else{
        LOGE("getColumnChar: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnInt
   * Signature: (Ljava/lang/String;)I
   */
JNIEXPORT jint JNICALL getColumnInt(JNIEnv * env, jobject obj, jstring fname)
{
    if(db && query){
        return query->getColumn(Jstring2CStr(env, fname)).getInt();
    }else{
        LOGE("getColumnInt: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnLong
   * Signature: (Ljava/lang/String;)J
   */
JNIEXPORT jlong JNICALL getColumnLong(JNIEnv *env, jobject obj, jstring fname)
{
    if(db && query){
        return query->getColumn(Jstring2CStr(env, fname)).getInt64();
    }else{
        LOGE("getColumnLong: database or query is NULL");
        return 0;
    }
}

/*
   * Class:     com_mim_database_sqliter
   * Method:    getColumnDouble
   * Signature: (Ljava/lang/String;)D
   */
JNIEXPORT jdouble JNICALL getColumnDouble(JNIEnv * env, jobject onj, jstring fname)
{
    if(db && query){
        return query->getColumn(Jstring2CStr(env, fname)).getDouble();
    }else{
        LOGE("getColumnDoubleByIndex: database or query is NULL");
        return 0;
    }
}

/*
 * Class:     com_mim_database_sqliter
 * Method:    getColumnString
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL getColumnString(JNIEnv *env, jobject obj, jstring fname)
{
    if(db && query){
        const char* ret = query->getColumn(Jstring2CStr(env, fname)).getString().c_str();
        size_t len  = strlen(ret);
        return ConvertCharsToJByteaArray(env, ret, len);
    }else{
        LOGE("getColumnString: database or query is NULL");
        return NULL;
    }
}

/*
 * Class:     com_mim_database_sqliter
 * Method:    getColumnText
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL getColumnText(JNIEnv *env, jobject obj, jstring fname)
{
    if(db && query){
        if(db && query){
            const char* ret = query->getColumn(Jstring2CStr(env, fname)).getText();
            size_t len  = strlen(ret);
            return ConvertCharsToJByteaArray(env, ret, len);
        }else{
            LOGE("getColumnText: database or query is NULL");
            return NULL;
        }
    }else{
        LOGE("getColumnText: database or query is NULL");
        return NULL;
    }
}


/*
 * Class:     com_mim_database_sqliter
 * Method:    getColumnBlob
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray getColumnBlob(JNIEnv *env, jobject obj, jstring fname)
{
    if(db && query){
    }else{
        LOGE("getColumnBlob: database or query is NULL");
        return NULL;
    }
}

// 指定要注册的类，对应完整的java类名
#define JNIREG_CLASS "com/mim/database/sqliter"

// Java和JNI函数的绑定表
static JNINativeMethod method_table[] = {
    {"connect", "(Ljava/lang/String;I)Z", (void *)connect},
    {"version", "()Ljava/lang/String;", (void *)version},
    {"tableCheck", "(Ljava/lang/String;)Z", (void *)tableCheck},
    {"executeSql", "(Ljava/lang/String;)Z", (void *)executeSql},
    {"exec", "(Ljava/lang/String;)Z", (void *)exec},
    {"next", "()Z", (void *)next},
    {"getColumnBoolByIndex", "(I)Z", (void *)getColumnBoolByIndex},
    {"getColumnByteByIndex", "(I)B", (void *)getColumnByteByIndex},
    {"getColumnCharByIndex", "(I)C", (void *)getColumnCharByIndex},
    {"getColumnIntByIndex", "(I)I", (void *)getColumnIntByIndex},
    {"getColumnLongByIndex", "(I)J", (void *)getColumnLongByIndex},
    {"getColumnDoubleByIndex", "(I)D", (void *)getColumnDoubleByIndex},
    {"getColumnStringByIndex", "(I)[B", (void *)getColumnStringByIndex},
    {"getColumnTextByIndex", "(I)[B", (void *)getColumnTextByIndex},
    {"getColumnBlobByIndex", "(I)[B", (void *)getColumnBlobByIndex},
    {"getColumnBool", "(Ljava/lang/String;)Z", (void *)getColumnBool},
    {"getColumnByte", "(Ljava/lang/String;)B", (void *)getColumnByte},
    {"getColumnChar", "(Ljava/lang/String;)C", (void *)getColumnChar},
    {"getColumnInt", "(Ljava/lang/String;)I", (void *)getColumnInt},
    {"getColumnLong", "(Ljava/lang/String;)J", (void *)getColumnLong},
    {"getColumnDouble", "(Ljava/lang/String;)D", (void *)getColumnDouble},
    {"getColumnString", "(Ljava/lang/String;)[B", (void *)getColumnString},
    {"getColumnText", "(Ljava/lang/String;)[B", (void *)getColumnText},
    {"getColumnBlob", "(Ljava/lang/String;)[B", (void *)getColumnBlob}};


// 注册native方法到java中
static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods, int numMethods)
{
    LOGD("==== registerNativeMethods ===\n");
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        LOGD("registerNativeMethods FindClass:%d\n", JNI_FALSE);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0)
    {
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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGD("==== JNI_OnLoad ===\n");
    g_JavaVM = vm;
    JNIEnv *env = NULL;
    jint result = -1;
    jint rt = -1;
    if ((rt = vm->GetEnv((void **)&env, JNI_VERSION_1_4)) != JNI_OK)
    {
        LOGD("JNI_OnLoad GetEnv %d\n", rt);
        return result;
    }
    if (!(rt = register_ndk_load(env)))
    {
        LOGE("JNI_OnLoad registerNativeMethods %d\n", rt);
    }
    // 返回jni的版本
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    LOGD("==== JNI_OnUnload ===\n");
}