package com.mim.mimer.sender;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

import com.mim.mimer.loginer.loginCB;
import com.mim.mimer.loginer.loginer;
import com.mim.mimer.Constant;

public final class Sender implements Parcelable, loginCB {
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
    private loginer lger;

    public Sender(String ip, int port) {
        this.ip = ip;
        this.port = port;
    }

    public boolean AttachLoginer(loginer lger){
        if(null == lger) {
            return false;
        }else{
            this.lger = lger;
        }
        return true;
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

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        // 序列化过程：必须按成员变量声明的顺序进行封装
        dest.writeString(ip);
        dest.writeInt(port);
        dest.writeString(token);
        dest.writeString(passwd);
        dest.writeInt(passwdLen);
    }

    public static final Parcelable.Creator<Sender> CREATOR = new Creator<Sender>() {

        @Override
        public Sender createFromParcel(Parcel source) {
            String ip = source.readString();
            int port = source.readInt();
            Sender sender = new Sender(ip, port);
            return sender;
        }

        @Override
        public Sender[] newArray(int size) {
            return new Sender[size];
        }
    };


    @Override
    public int success() {
        System.out.println("connect success！");
        lger.getmHandler().sendEmptyMessage(Constant.CONN_SERVER_OK);
        return 0;
    }

    @Override
    public void error() {
        System.out.println("connect error！");
        lger.getmHandler().sendEmptyMessage(Constant.CONN_SERVER_ERROR);
    }
}
