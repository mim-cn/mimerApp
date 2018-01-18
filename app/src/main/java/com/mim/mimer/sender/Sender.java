package com.mim.mimer.sender;

import android.util.Log;

public final class Sender {
    static {
        try {
            System.loadLibrary("mimer");
            System.loadLibrary("transmitter");
            System.loadLibrary("uv");
            System.loadLibrary("uvbase");
            System.loadLibrary("ellog");
            System.loadLibrary("mimp");
        } catch (Throwable e) {
            Log.d("zzzzz", "加载xx库异常 ：" + e.toString());
        }
    }

    private String ip;
    private int port;
    public Sender(String ip, int port) {
        this.ip = ip;
        this.port = port;
    }

    public synchronized void finalize() {
        cfinalize();
    }

    public native boolean connect(String ip, int port);

    private native void cfinalize();
}
