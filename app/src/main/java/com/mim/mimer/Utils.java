package com.mim.mimer;

import android.util.Base64;

/**
 * Created by skybosi on 2018/4/1.
 */

public class Utils {
    // encode
    // byte[] to base64 String
    public static String encode2Base64(byte[] content){
        return Base64.encodeToString(content, Base64.DEFAULT);
    }

    // String to base64 String
    public static String encode2Base64(String content){
        return encode2Base64(content.getBytes());
    }

    //decode
    // base64 String to byte[]
    public static byte[] decode2Bytes(String content){
        return Base64.decode(content, Base64.DEFAULT);
    }

    // base64 String to String
    public static String decode2Str(String content){
        return new String(decode2Bytes(content));
    }

    // byte[] to Base64 byte[]
    public static byte[] decode2Bytes(byte[] content){
        return Base64.decode(content, Base64.DEFAULT);
    }

    // byte[] to String
    public static String decode2Str(byte[] content){
        return new String(decode2Bytes(content));
    }

    public static String escapeSql(String sql){
        return "'" +  sql + "'";
    }
}
