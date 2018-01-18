package com.mim.mimer.loginer;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.mim.mimer.chats.ChatActivity;
import com.mim.mimer.R;

public class loginer extends Activity {
//    static {
//        try {
//            System.loadLibrary("ellog");
//            System.loadLibrary("mimp");
//            System.loadLibrary("transmitter");
//            System.loadLibrary("uv");
//            System.loadLibrary("uvbase");
//        } catch (Throwable e) {
//            Log.d("zzzzz", "加载xx库异常 ：" + e.toString());
//        }
//    }
    private EditText id_login;
    private EditText password_login;
    private ImageView avatar_login;
    private CheckBox rememberpassword_login;
    private CheckBox auto_login;
    private TextView button_login;
    private SharedPreferences sp;
    private String idvalue;
    private String passwordvalue;
    private static final int PASSWORD_MIWEN = 0x81;
    private static boolean isExit = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        sp = this.getSharedPreferences("userInfo", Context.MODE_WORLD_READABLE);
        //找到相应的布局及控件
        setContentView(R.layout.loginer);
        id_login = (EditText) findViewById(R.id.login_id);
        password_login = (EditText) findViewById(R.id.login_password);
        avatar_login = (ImageView) findViewById(R.id.login_avatar);
        rememberpassword_login = (CheckBox) findViewById(R.id.login_rememberpassword);
        auto_login = (CheckBox) findViewById(R.id.login_autologin);
        button_login = (TextView) findViewById(R.id.login_button);

        if (sp.getBoolean("ischeck", false)) {
            rememberpassword_login.setChecked(true);
            id_login.setText(sp.getString("PHONEEDIT", ""));
            password_login.setText(sp.getString("PASSWORD", ""));
            //密文密码
            password_login.setInputType(PASSWORD_MIWEN);
            if (sp.getBoolean("auto_ischeck", false)) {
                auto_login.setChecked(true);
                Intent intent = new Intent(loginer.this, ChatActivity.class);
                startActivity(intent);
            }
        }

        button_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                id_login.getPaint().setFlags(0);
                idvalue = id_login.getText().toString();
                password_login.getPaint().setFlags(0);
                passwordvalue = password_login.getText().toString();


//                if (idvalue.equals("18428377130")&&passwordvalue.equals("1234567890")){
                if (rememberpassword_login.isChecked()) {
                    SharedPreferences.Editor editor = sp.edit();
                    editor.putString("USERID", idvalue);
                    editor.putString("PASSWORD", passwordvalue);
                    editor.commit();
                }
                Intent intent = new Intent(loginer.this, ChatActivity.class);
                startActivity(intent);
                finish();
//                }else{
//                    Toast.makeText(loginer.this, "手机号码或密码错误，请重新登录", Toast.LENGTH_SHORT).show();
//                }
            }
        });

        rememberpassword_login.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (rememberpassword_login.isChecked()) {
                    System.out.println("记住密码已选中");
                    sp.edit().putBoolean("ischeck", true).commit();
                } else {
                    System.out.println("记住密码没有选中");
                    sp.edit().putBoolean("ischeck", false).commit();
                }
            }
        });

        auto_login.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (auto_login.isChecked()) {
                    System.out.println("自动登录已选中");
                    sp.edit().putBoolean("auto_ischeck", true).commit();
                } else {
                    System.out.println("自动登录没有选中");
                    sp.edit().putBoolean("auto_ischeck", false).commit();
                }
            }
        });
    }

    @Override
    public synchronized void onBackPressed() {
        if (!isExit) {
            isExit = true;
            Toast.makeText(getApplicationContext(), "再按一次后退键退出主程序！", Toast.LENGTH_SHORT).show();
        } else {
            isExit = false;
            this.finish();
        }
    }
}