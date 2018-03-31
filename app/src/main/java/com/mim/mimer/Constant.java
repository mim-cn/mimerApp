package com.mim.mimer;

/**
 * Created by skybosi on 2018/2/20.
 */

public class Constant {
    //////////////////////////Database Mode ////////////////////////////////

    public static final int SQLITE_OPEN_READONLY = 1;
    public static final int SQLITE_OPEN_READWRITE = 2;
    public static final int SQLITE_OPEN_CREATE = 4;
    public static final int SQLITE_OPEN_URI = 64;
    public static final int SQLITE_OPEN_MEMORY = 128;
    //////////////////////////server information//////////////////////////////
    /**
     * server daemon
     */
    public static final String SERVER_DAEMON = "123.207.108.63";
    /**
     * server mimer port
     */
    public static final int SERVER_PORT = 9130;
    /////////////////////////////error code///////////////////////////////////
    /**
     * connect server success
     */
    public static final int CONN_SERVER_OK = 0;
    /**
     * connect server error
     */
    public static final int CONN_SERVER_ERROR = 1;
    /**
     * connect server success,and check username failed
     */
    public static final int CONN_USERNAME_ERROR = 2;
    /**
     * connect server success,and check password failed
     */
    public static final int CONN_PASSWORD_ERROR = 3;
    /**
     * check input view is empty
     */
    public static final int INPUT_USERPWD_ERROR = 4;

    /**
     * read data from other peer
     */
    public static final int RECEIVED_DATA_OK = 100;
    /**
     * write data to other peer
     */
    public static final int SENDED_DATA_OK = 101;
}
