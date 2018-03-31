package com.mim.mimer.chats;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Toast;

import com.mim.database.sqliter;
import com.mim.mimer.Constant;
import com.mim.mimer.R;
import com.mim.mimer.loginer.loginActivity;
import com.mim.mimer.sender.Sender;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import yalantis.com.sidemenu.interfaces.Resourceble;
import yalantis.com.sidemenu.interfaces.ScreenShotable;
import yalantis.com.sidemenu.model.SlideMenuItem;
import yalantis.com.sidemenu.util.ViewAnimator;

public class ChatActivity extends AppCompatActivity implements ViewAnimator.ViewAnimatorListener {
    ListView msgListView;
    EditText inputText;
    Button send;
    private int id = 0;
    public static final String CLOSE = "Close";
    public static final String BUILDING = "Building";
    public static final String BOOK = "Book";
    public static final String PAINT = "Paint";
    public static final String CASE = "Case";
    public static final String SHOP = "Shop";
    public static final String PARTY = "Party";
    public static final String MOVIE = "Movie";
    private DrawerLayout drawerLayout;
    private ActionBarDrawerToggle drawerToggle;
    private List<SlideMenuItem> list = new ArrayList<>();
    private ViewAnimator viewAnimator;
    private LinearLayout linearLayout;
    private static boolean isExit = false;
    private static Sender sender = null;
    private MsgAdapter adapter;
    private List<Msg> msgList = new ArrayList<Msg>();
    private Handler mHandler = null;
    private sqliter sqler = null;
    private String chatTo = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat);
        //新页面接收数据
        Bundle bundle = this.getIntent().getExtras();
        chatTo = bundle.getString("ChatUser");
        setTitle(chatTo);
        msgListView = (ListView) findViewById(R.id.msg_list_view);
        inputText = (EditText) findViewById(R.id.input_text);
        send = (Button) findViewById(R.id.send);
        sender = loginActivity.getSender();
        // 初始化消息数据
        initMsgs();
        adapter = new MsgAdapter(ChatActivity.this, R.layout.activity_msg, msgList);
        msgListView.setAdapter(adapter);
        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //sender.Write(0,null);
                String content = inputText.getText().toString();
                if (null != sender && sender.isConnected()) {
                    sender.writer(content.length(), content.getBytes());
                }
                if (0 != content.length()) {
                    Msg msg = new Msg(content, Msg.From.TYPE_SENT, Msg.MsgType.PURE_TEXT);
                    if(null != sqler) {
                        String insert_sql = "INSERT INTO chat VALUES (" + (id++) + ", 'you', '" + chatTo + "', '" + msg.getString() + "')";
                        sqler.execute(insert_sql);
                    }
                    msgList.add(msg);
                    // 当有新消息时，刷新ListView中的显示
                    adapter.notifyDataSetChanged();
                    // 将ListView定位到最后一行
                    msgListView.setSelection(msgList.size());
                    // 清空输入框中的内容
                    inputText.setText("");
                }
            }
        });

        drawerLayout = (DrawerLayout) findViewById(R.id.chat_layout);
        drawerLayout.setScrimColor(Color.TRANSPARENT);
        linearLayout = (LinearLayout) findViewById(R.id.left_drawer);
        linearLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                drawerLayout.closeDrawers();
            }
        });

        setActionBar();
        createMenuList();
        viewAnimator = new ViewAnimator<>(this, list, null, drawerLayout, this);
        AttachChater();
        mHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what) {
                    case Constant.RECEIVED_DATA_OK:
                        // 当有新消息时，刷新ListView中的显示
                        adapter.notifyDataSetChanged();
                        // 将ListView定位到最后一行
                        msgListView.setSelection(msgList.size());
                        break;
                    case Constant.SENDED_DATA_OK:
//                        // 当有新消息时，刷新ListView中的显示
//                        adapter.notifyDataSetChanged();
//                        // 将ListView定位到最后一行
//                        msgListView.setSelection(msgList.size());
                        Toast.makeText(getApplicationContext(), "消息发送成功！", Toast.LENGTH_SHORT).show();
                        break;
                    default:
                        break;
                }
            }
        };
    }

    private void initMsgs() {
        String path = this.getDatabasePath("ignored").getParentFile().getAbsolutePath() + "/example.db";
        String path1 = this.getFilesDir().getPath() + "/example.db";
        sqler = new sqliter(path1, Constant.SQLITE_OPEN_READWRITE | Constant.SQLITE_OPEN_CREATE);
        boolean con = sqler.connect();
        if(true == con) {
            String version = sqler.getVersion();
            boolean create_bool = false;
            if (false == sqler.isExist("chat")) {
                create_bool = sqler.execute("CREATE TABLE chat ('id' INTEGER PRIMARY KEY, 'from_id' CHAR(16), 'to_id' CHAR(16), 'message' TEXT)");
            } else {
                sqler.executeQuery("SELECT * from chat");
                while (sqler.Next()) {
                    int id1 = sqler.getInt(0);
                    String from = new String(sqler.getString(1));
                    String to = new String(sqler.getString(2));
                    String msg = new String(sqler.getString(3));
                    id = id1;
//                    if (tto.equals(chatTo)) {
                        Msg msg1 = new Msg(msg, Msg.From.TYPE_SENT, Msg.MsgType.PURE_TEXT);
                        msgList.add(msg1);
//                    }
                }
            }
//        Msg msg1 = new Msg("Hello guy.", Msg.From.TYPE_RECEIVED);
//        msgList.add(msg1);
//        Msg msg2 = new Msg("Hello. Who is that?", Msg.From.TYPE_SENT);
//        msgList.add(msg2);
//        Msg msg3 = new Msg("This is Tom. Nice talking to you. ", Msg.From.TYPE_RECEIVED);
//        msgList.add(msg3);
        }else{
            Log.e("Chating", "connect databases is error");
            sqler = null;
        }

    }

    @Override
    public ScreenShotable onSwitch(Resourceble slideMenuItem, ScreenShotable screenShotable, int position) {
        switch (slideMenuItem.getName()) {
            case CLOSE:
                return screenShotable;
            default:
                return null;
        }
    }

    @Override
    public void disableHomeButton() {
        getSupportActionBar().setHomeButtonEnabled(false);
    }

    @Override
    public void enableHomeButton() {
        getSupportActionBar().setHomeButtonEnabled(true);
        drawerLayout.closeDrawers();

    }

    @Override
    public void addViewToContainer(View view) {
        linearLayout.addView(view);
    }

    private void createMenuList() {
        SlideMenuItem menuItem0 = new SlideMenuItem(CLOSE, R.drawable.icn_close);
        list.add(menuItem0);
        SlideMenuItem menuItem = new SlideMenuItem(BUILDING, R.drawable.icn_1);
        list.add(menuItem);
        SlideMenuItem menuItem2 = new SlideMenuItem(BOOK, R.drawable.icn_2);
        list.add(menuItem2);
        SlideMenuItem menuItem3 = new SlideMenuItem(PAINT, R.drawable.icn_3);
        list.add(menuItem3);
//        SlideMenuItem menuItem4 = new SlideMenuItem(ContentFragment.CASE, R.drawable.icn_4);
//        list.add(menuItem4);
//        SlideMenuItem menuItem5 = new SlideMenuItem(ContentFragment.SHOP, R.drawable.icn_5);
//        list.add(menuItem5);
//        SlideMenuItem menuItem6 = new SlideMenuItem(ContentFragment.PARTY, R.drawable.icn_6);
//        list.add(menuItem6);
//        SlideMenuItem menuItem7 = new SlideMenuItem(ContentFragment.MOVIE, R.drawable.icn_7);
//        list.add(menuItem7);
    }


    private void setActionBar() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setHomeButtonEnabled(true);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        drawerToggle = new ActionBarDrawerToggle(
                this,                  /* host Activity */
                drawerLayout,         /* DrawerLayout object */
                toolbar,  /* nav drawer icon to replace 'Up' caret */
                R.string.drawer_open,  /* "open drawer" description */
                R.string.drawer_close  /* "close drawer" description */
        ) {

            /** Called when a drawer has settled in a completely closed state. */
            public void onDrawerClosed(View view) {
                super.onDrawerClosed(view);
                linearLayout.removeAllViews();
                linearLayout.invalidate();
            }

            @Override
            public void onDrawerSlide(View drawerView, float slideOffset) {
                super.onDrawerSlide(drawerView, slideOffset);
                if (slideOffset > 0.6 && linearLayout.getChildCount() == 0)
                    viewAnimator.showMenuContent();
            }

            /** Called when a drawer has settled in a completely open state. */
            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
            }
        };
        drawerLayout.setDrawerListener(drawerToggle);
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        drawerToggle.syncState();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        drawerToggle.onConfigurationChanged(newConfig);
    }

    /*
    @Override
    public synchronized void onBackPressed() {
        if (!isExit) {
            isExit = true;
            Toast.makeText(getApplicationContext(), "测试中，后退键返回上一页！", Toast.LENGTH_SHORT).show();
        } else {
            isExit = false;
            this.finish();
        }
    }
    */

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (drawerToggle.onOptionsItemSelected(item)) {
            return true;
        }
        switch (item.getItemId()) {
            case R.id.action_settings:
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public ListView getMsgListView() {
        return msgListView;
    }

    public List<Msg> getMsgList() {
        return msgList;
    }

    public EditText getInputText() {
        return inputText;
    }

    public MsgAdapter getAdapter() {
        return adapter;
    }

    public void AttachChater()
    {
        if (sender != null) {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    sender.AttachChater(ChatActivity.this);
                }
            });
            thread.start();

        }
    }


    public Handler getmHandler() {
        return mHandler;
    }
}