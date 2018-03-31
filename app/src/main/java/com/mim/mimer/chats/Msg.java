package com.mim.mimer.chats;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class Msg {
    public enum From {
        TYPE_RECEIVED, TYPE_SENT
    }
    public enum MsgType{
        PURE_TEXT, JSON_TEXT, HTML_TEXT, RICH_TEXT,
        IMAGE, IMAGE3D, AUDIO, VIDEO
    }

    private byte[]  content;
    private From    from;
    private MsgType type;

    public Msg(byte[] content, From from, MsgType type) {
        this.content = content;
        this.from = from;
        this.type = type;
    }

    public Msg(String content, From from, MsgType type) {
        this.content = content.getBytes();
        this.from = from;
        this.type = type;
    }

    public Msg(String content, From from) {
        this.content = content.getBytes();
        this.from = from;
        this.type = MsgType.PURE_TEXT;
    }
    public byte[]  getContent() {
        return content;
    }
    public int     getSize()    { return content.length; }
    public String  getString()  { return new String(content); }
    public MsgType getType()    { return type; }
    public From    getFrom()    { return from; }
    public byte[]  getData()    { return content;}
    public JSONObject getJson() throws JSONException {
        try {
            if (MsgType.JSON_TEXT == type) {
                return new JSONObject(new String(content));
            } else {
                return null;
            }
        }catch (Exception ex){
            return null;
        }
    }
}