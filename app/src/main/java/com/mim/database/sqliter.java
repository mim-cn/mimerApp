package com.mim.database;


import android.util.Log;

/**
 * Created by skybosi on 2018/3/25.
 */

public class sqliter {
    static {
        try {
            System.loadLibrary("sqlite");
        } catch (Throwable e) {
            Log.d("SQLITE", "加载xx库异常 ：" + e.toString());
        }
    }
    /*
    public enum MODE{
        SQLITE_OPEN_READONLY(1),
        SQLITE_OPEN_READWRITE(2),
        SQLITE_OPEN_CREATE(4),
        SQLITE_OPEN_URI(64),
        SQLITE_OPEN_MEMORY(128);

        private final int value;
        //构造方法必须是private或者默认
        private MODE(int value) {
            this.value = value;
        }

        public MODE valueOf(int value) {
            switch (value) {
                case 1:
                    return MODE.SQLITE_OPEN_READONLY;
                case 2:
                    return MODE.SQLITE_OPEN_READWRITE;
                case 4:
                    return MODE.SQLITE_OPEN_CREATE;
                case 64:
                    return MODE.SQLITE_OPEN_URI;
                case 128:
                    return MODE.SQLITE_OPEN_MEMORY;
                default:
                    return null;
            }
        }
        public int getValue(){ return  this.value;}
    }
    */
    private String db_name;
    private String table_name;
    private int    mode;
    private boolean isConnected = false;

    public sqliter(String db_name, int mode) {
        this.db_name = db_name;
        this.mode = mode;
        this.table_name = "";
    }

    public boolean isConnected() {
        return isConnected;
    }

    public boolean connect(){
        return (isConnected = this.connect(this.db_name, this.mode));
    }

    public synchronized boolean executeQuery(String sql){
        return executeSql(sql);
    }

    public boolean Next(){
        return next();
    }

    public boolean execute(String sql){
        return exec(sql);
    }

    public byte[] getBlob(int index)
    {
        return getColumnBlobByIndex(index);
    }

    public byte[] getText(int index)
    {
        return getColumnTextByIndex(index);
    }

    public byte[] getString(int index)
    {
        return getColumnStringByIndex(index);
    }

    public boolean getBoolean(int index)
    {
        return getColumnBoolByIndex(index);
    }

    public byte getByte(int index)
    {
        return getColumnByteByIndex(index);
    }

    public char getChar(int index)
    {
        return getColumnCharByIndex(index);
    }

    public short getShort(int index)
    {
        return (short) getColumnIntByIndex(index);
    }

    public int getInt(int index)
    {
        return getColumnIntByIndex(index);
    }

    public long getLong(int index)
    {
        return getColumnLongByIndex(index);
    }

    public float getFloat(int index)
    {
        return (float) getColumnDoubleByIndex(index);
    }

    public double getDouble(int index)
    {
        return getColumnDoubleByIndex(index);
    }

    public byte[] getBlob(String var1)
    {
        return getColumnBlob(var1);
    }

    public byte[] getText(String var1)
    {
        return getColumnText(var1);
    }

    public byte[] getString(String var1)
    {
        return getColumnString(var1);
    }

    public boolean getBoolean(String var1)
    {
        return getColumnBool(var1);
    }

    public byte getByte(String var1)
    {
        return getColumnByte(var1);
    }

    public char getChar(String var1)
    {
        return getColumnChar(var1);
    }

    public short getShort(String var1)
    {
        return (short) getColumnInt(var1);
    }

    public int getInt(String var1)
    {
        return getColumnInt(var1);
    }

    public long getLong(String var1)
    {
        return getColumnLong(var1);
    }

    public float getFloat(String var1)
    {
        return (float) getColumnDouble(var1);
    }

    public double getDouble(String var1)
    {
        return getColumnDouble(var1);
    }

    public boolean isExist(){
        return tableCheck(this.table_name);
    }

    public boolean isExist(String tb_name){
        return tableCheck(tb_name);
    }

    public String getVersion(){
        return version();
    }

    /**
     * 试图连接数据库
     * @param db_name
     * @return
     */
    private native boolean connect(String db_name, int mode);

    /**
     * 数据库版本
     * @return
     */
    private native String version();

    /**
     * 检查数据库中数据表是否存在
     * @return
     */
    private native boolean tableCheck(String tb_name);

    /**
     * 执行查询语句
     * @return
     */
    private native boolean executeSql(String sql);

    /**
     * 仅仅是执行sql语句，不关注结果
     * @return
     */
    private native boolean exec(String sql);

    /**
     * 检测是否还有下一条记录
     * @return
     */
    private native boolean next();

    /**
     * 根据字段的位置下标，获取执行结果中字段的结果
     * @param index
     * @return
     */

    private native boolean getColumnBoolByIndex(int index);
    private native byte    getColumnByteByIndex(int index);
    private native char    getColumnCharByIndex(int index);
    private native int     getColumnIntByIndex(int index);
    private native long    getColumnLongByIndex(int index);
    private native double  getColumnDoubleByIndex(int index);
    private native byte[]  getColumnStringByIndex(int index);
    private native byte[]  getColumnTextByIndex(int index);
    private native byte[]  getColumnBlobByIndex(int index);
    /**
     * 根据字段名称，获取执行结果中字段的结果
     * @param fname
     * @return
     */
    private native boolean getColumnBool(String fname);
    private native byte    getColumnByte(String fname);
    private native char    getColumnChar(String fname);
    private native int     getColumnInt(String fname);
    private native long    getColumnLong(String fname);
    private native double  getColumnDouble(String fname);
    private native byte[]  getColumnString(String fname);
    private native byte[]  getColumnText(String fname);
    private native byte[]  getColumnBlob(String fname);
}
