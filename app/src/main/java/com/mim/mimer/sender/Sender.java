package com.mim.mimer.sender;

import android.util.Log;

public final class Sender {
    static {
        try {
            System.loadLibrary("mimer");
            System.loadLibrary("transmitter");
            System.loadLibrary("uv");
            System.loadLibrary("uvbase");
            System.loadLibrary("mimp");
        } catch (Throwable e) {
            Log.d("zzzzz", "加载xx库异常 ：" + e.toString());
        }
    }

    private String ip;
    private int port;
    private String token;
    private String passwd;
    private int passwdLen;

    public Sender(String ip, int port) {
        this.ip = ip;
        this.port = port;
    }

    public synchronized void finalize() {
        cfinalize();
    }

    /*
    试图连接服务
    * */
    public native boolean Relate(String ip, int port);

    /*
    链接成功,试图登录
    * */
    public native boolean Login(String token, String passwd, int passwdLen);

    /*
    标准写回调函数
    * */
    public native void Write(int nread, String buf);

    /*
    标准读回调函数
    * */
    public native void Read(int nread, String buf);

    private native void cfinalize();
}
