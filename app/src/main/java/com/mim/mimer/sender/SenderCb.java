package com.mim.mimer.sender;

/**
 * Created by skybosi on 2018/2/19.
 */

public interface SenderCb {
    // conncet
    public int success();
    public void error();
    // send
    public void onRead(int size, byte[] buf, byte Type);
    public void onWrite();
}
