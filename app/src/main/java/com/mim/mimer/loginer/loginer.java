package com.mim.mimer.loginer;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.mim.mimer.Constant;
import com.mim.mimer.chats.ChatActivity;
import com.mim.mimer.R;
import com.mim.mimer.sender.Sender;

public class loginer extends Activity {
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
    private static Sender sender = null;
    private Handler mHandler = null;

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
        //密文密码
        password_login.setInputType(PASSWORD_MIWEN);
        if (sp.getBoolean("ischeck", false)) {
            rememberpassword_login.setChecked(true);
            id_login.setText(sp.getString("PHONEEDIT", ""));
            password_login.setText(sp.getString("PASSWORD", ""));
            if (sp.getBoolean("auto_ischeck", false)) {
                auto_login.setChecked(true);
                isExit = true;
                if (checkNetStatus(getApplicationContext())) {
                    initConnect();
                    //sender.Login(idvalue, passwordvalue, passwordvalue.length());
                }
//                Intent intent = new Intent(loginer.this, ChatActivity.class);
//                startActivity(intent);
            }
        }

        button_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                id_login.getPaint().setFlags(0);
                idvalue = id_login.getText().toString();
                password_login.getPaint().setFlags(0);
                passwordvalue = password_login.getText().toString();
                if(idvalue.length() <= 0 || passwordvalue.length() <= 0){
                       mHandler.sendEmptyMessage(Constant.INPUT_USERPWD_ERROR);
                }else {
                    if (checkNetStatus(getApplicationContext())) {
                        initConnect();
                        //sender.Login(idvalue, passwordvalue, passwordvalue.length());
                    }
                }
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
        mHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what) {
                    case Constant.CONN_SERVER_OK:
                        Toast.makeText(getApplicationContext(), "连接服务器成功！", Toast.LENGTH_SHORT).show();
                        if (rememberpassword_login.isChecked()) {
                            SharedPreferences.Editor editor = sp.edit();
                            editor.putString("USERID", idvalue);
                            editor.putString("PASSWORD", passwordvalue);
                            editor.commit();
                        }
                        Intent intent = new Intent(loginer.this, ChatActivity.class);
                        startActivity(intent);
                        break;
                    case Constant.CONN_SERVER_ERROR:
                        Toast.makeText(getApplicationContext(), "连接服务器失败！", Toast.LENGTH_SHORT).show();
                        break;
                    case Constant.INPUT_USERPWD_ERROR:
                        Toast.makeText(getApplicationContext(), "请输入账户或注册！", Toast.LENGTH_SHORT).show();
                        break;
                    default:
                        break;
                }
            }
        };
        checkNetStatus(getApplicationContext());
    }

    boolean checkNetStatus(Context context) {
        System.out.println("网络状态发生变化");
        //检测API是不是小于23，因为到了API23之后getNetworkInfo(int networkType)方法被弃用
        if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.LOLLIPOP) {
            //获得ConnectivityManager对象
            ConnectivityManager connMgr = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);

            //获取ConnectivityManager对象对应的NetworkInfo对象
            //获取WIFI连接的信息
            NetworkInfo wifiNetworkInfo = connMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
            //获取移动数据连接的信息
            NetworkInfo dataNetworkInfo = connMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
            if (wifiNetworkInfo.isConnected() && dataNetworkInfo.isConnected()) {
                Toast.makeText(context, "WIFI已连接,移动数据已连接", Toast.LENGTH_SHORT).show();
                return true;
            } else if (wifiNetworkInfo.isConnected() && !dataNetworkInfo.isConnected()) {
                Toast.makeText(context, "WIFI已连接,移动数据已断开", Toast.LENGTH_SHORT).show();
                return false;
            } else if (!wifiNetworkInfo.isConnected() && dataNetworkInfo.isConnected()) {
                Toast.makeText(context, "WIFI已断开,移动数据已连接", Toast.LENGTH_SHORT).show();
                return true;
            } else {
                Toast.makeText(context, "WIFI已断开,移动数据已断开", Toast.LENGTH_SHORT).show();
                return false;
            }
            //API大于23时使用下面的方式进行网络监听
        } else {
            System.out.println("API level 大于23");
            //获得ConnectivityManager对象
            ConnectivityManager connMgr = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            //获取所有网络连接的信息
            Network[] networks = connMgr.getAllNetworks();
            //用于存放网络连接信息
            StringBuilder sb = new StringBuilder();
            if (0 == networks.length) {
                Toast.makeText(context, "网络异常, 请检查网络设置", Toast.LENGTH_SHORT).show();
                return false;
            }
            //通过循环将网络信息逐个取出来
            for (int i = 0; i < networks.length; i++) {
                //获取ConnectivityManager对象对应的NetworkInfo对象
                NetworkInfo networkInfo = connMgr.getNetworkInfo(networks[i]);
                sb.append(networkInfo.getTypeName() + " connect is " + networkInfo.isConnected());
            }
            // Toast.makeText(context, sb.toString(),Toast.LENGTH_SHORT).show();
            return true;
        }
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

    private void initConnect() {
        if (sender == null) {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    sender = new Sender(Constant.SERVER_DAEMON, Constant.SERVER_PORT);
                    sender.AttachLoginer(loginer.this);
                    sender.Relate(Constant.SERVER_DAEMON, Constant.SERVER_PORT);
                }
            });
            thread.start();
        } else {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    sender.AttachLoginer(loginer.this);
                    sender.Relate(Constant.SERVER_DAEMON, Constant.SERVER_PORT);
                }
            });
            thread.start();
        }
    }

    public Handler getmHandler() {
        return mHandler;
    }
}