package com.mim.mimer.main;

import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.view.KeyEvent;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

import com.mim.mimer.R;
/**
 * Created by Coder-pig on 2015/8/28 0028.
 */
public class PageManager extends AppCompatActivity implements RadioGroup.OnCheckedChangeListener,
        ViewPager.OnPageChangeListener {

    //UI Objects
    private TextView txt_topbar;
    private RadioGroup rg_tab_bar;
    private RadioButton rb_chat;
    private RadioButton rb_contact;
    private RadioButton rb_better;
    private RadioButton rb_setting;
    public LinearLayout Chat_List_Layout;
    public LinearLayout Contact_List_Layout;
    private ViewPager vpager;
    FragmentManager mFragmentManager;
    private PagerFragmentAdapter mAdapter;
    private static boolean isExit = false;

    //几个代表页面的常量
    public static final int PAGE_ONE = 0;
    public static final int PAGE_TWO = 1;
    public static final int PAGE_THREE = 2;
    public static final int PAGE_FOUR = 3;
    List<Fragment> mFragmentList = new ArrayList<Fragment>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mFragmentManager = getSupportFragmentManager();
        setContentView(R.layout.activity_main);
        initFragmetList();
        mAdapter = new PagerFragmentAdapter(mFragmentManager, mFragmentList);
        bindViews();
        rb_chat.setChecked(true);
}

    private void bindViews() {
        txt_topbar = (TextView) findViewById(R.id.txt_topbar);
        rg_tab_bar = (RadioGroup) findViewById(R.id.rg_tab_bar);
        rb_chat = (RadioButton) findViewById(R.id.rb_chat);
        rb_contact = (RadioButton) findViewById(R.id.rb_contact);
//        rb_better = (RadioButton) findViewById(R.id.rb_better);
//        rb_setting = (RadioButton) findViewById(R.id.rb_setting);
        rg_tab_bar.setOnCheckedChangeListener(this);

        vpager = (ViewPager) findViewById(R.id.vpager);
        Chat_List_Layout = (LinearLayout) findViewById(R.id.chat_list_page);
        Contact_List_Layout = (LinearLayout)findViewById(R.id.contact_list_page);
        vpager.setAdapter(mAdapter);
        vpager.setCurrentItem(PAGE_ONE);
        vpager.addOnPageChangeListener(this);
    }

    public void initFragmetList() {
        Fragment chat = new ChatPage();
        Fragment contact = new ContactPage();
        mFragmentList.add(chat);
        mFragmentList.add(contact);
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        switch (checkedId) {
            case R.id.rb_chat:
                vpager.setCurrentItem(PAGE_ONE);
                txt_topbar.setText("聊天");
                break;
            case R.id.rb_contact:
                vpager.setCurrentItem(PAGE_TWO);
                txt_topbar.setText("通讯录");
                break;
        }
    }


    //重写ViewPager页面切换的处理方法
    @Override
    public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
    }

    @Override
    public void onPageSelected(int position) {
    }

    @Override
    public void onPageScrollStateChanged(int state) {
        //state的状态有三个，0表示什么都没做，1正在滑动，2滑动完毕
        if (state == 2) {
            switch (vpager.getCurrentItem()) {
                case PAGE_ONE:
                    rb_chat.setChecked(true);
                    break;
                case PAGE_TWO:
                    rb_contact.setChecked(true);
                    break;
            }
        }
    }
/*
    public synchronized void onBackPressed() {
        if (!isExit) {
            isExit = true;
            Toast.makeText(getApplicationContext(), "后退键返回上一页！", Toast.LENGTH_SHORT).show();
        } else {
            isExit = false;
            this.finish();
        }
    }
*/
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (isExit) {
                this.finish();

            } else {
                Toast.makeText(this, "再按一次退出", Toast.LENGTH_SHORT).show();
                isExit = true;
                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        isExit = false;
                    }
                }, 2000);
            }
            return true;
        }

        return super.onKeyDown(keyCode, event);
    }
}
