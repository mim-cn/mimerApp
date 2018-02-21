package com.mim.mimer;

/**
 * Created by skybosi on 2018/2/20.
 */

public class Constant {
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

}
