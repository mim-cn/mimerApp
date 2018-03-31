package com.mim.mimer.sender;

import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

import com.mim.mimer.chats.ChatActivity;
import com.mim.mimer.chats.Msg;
import com.mim.mimer.loginer.loginActivity;
import com.mim.mimer.Constant;

import java.io.Serializable;

public final class Sender implements Serializable, SenderCb {
    static {
        try {
            System.loadLibrary("mimer");
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
    private loginActivity lger = null;
    private ChatActivity chater = null;
    private boolean isConnected = false;

    public Sender(String ip, int port) {
        this.ip = ip;
        this.port = port;
    }

    public boolean AttachLoginer(loginActivity lger) {
        if (null == lger) {
            return false;
        } else {
            this.lger = lger;
        }
        return true;
    }

    public boolean AttachChater(ChatActivity chater) {
        if (null == chater) {
            return false;
        } else {
            this.chater = chater;
        }
        return true;
    }

    public void writer(int size, byte[] buf)
    {
        Write(size, buf);
    }

    public boolean isConnected(){return isConnected; }

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
    public native void Write(int nread, byte[] buf);

    /*
    标准读回调函数
    * */
    public native void Read(int nread, String buf);

    private native void cfinalize();

    @Override
    public int success() {
        isConnected = true;
        System.out.println("connect success！");
        lger.getmHandler().sendEmptyMessage(Constant.CONN_SERVER_OK);
        return 0;
    }

    @Override
    public void error() {
        System.out.println("connect error！");
        lger.getmHandler().sendEmptyMessage(Constant.CONN_SERVER_ERROR);
    }

    @Override
    public void onRead(int size, byte[] buf, byte type) {
        if (null != this.chater) {
            String content = new String(buf);
            float a = (float) Math.random();
            Msg msg1 = new Msg(content + a , Msg.From.TYPE_RECEIVED, Msg.MsgType.values()[type]);
            chater.getMsgList().add(msg1);
            chater.getmHandler().sendEmptyMessage(Constant.RECEIVED_DATA_OK);
        } else {
            System.out.println("onRead error, chater is null！");
        }
    }

    @Override
    public void onWrite() {
        if (null != this.chater) {
            chater.getmHandler().sendEmptyMessage(Constant.SENDED_DATA_OK);
        } else {
            System.out.println("onWrite error, chater is null！");
        }
    }
}