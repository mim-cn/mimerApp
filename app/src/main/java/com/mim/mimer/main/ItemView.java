package com.mim.mimer.main;

public class ItemView {
    private String name;
    private String sign;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSign() {
        return sign;
    }

    public void setSign(String sign) {
        this.sign = sign;
    }

    public ItemView(String name, String sign) {
        this.name = name;
        this.sign = sign;
    }

    public ItemView() {
    }
}
