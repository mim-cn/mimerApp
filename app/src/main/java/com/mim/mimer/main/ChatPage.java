package com.mim.mimer.main;

/**
 * Created by skybosi on 2018/3/24.
 */


import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.mim.mimer.R;
import com.mim.mimer.chats.ChatActivity;
import com.mim.mimer.loginer.loginActivity;

import java.util.ArrayList;
import java.util.List;

public class ChatPage extends Fragment {

    View mView;
    private RecyclerView mListView;
    private ItemAdapter adapter;
    private List<ItemView> list;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        if (mView == null) {
            mView = inflater.inflate(R.layout.chat_page, null);
            //初始化数据
            initData();
            mListView = (RecyclerView) mView.findViewById(R.id.chat_recycler_view);
            mListView.setLayoutManager(new LinearLayoutManager(mView.getContext()));
            adapter = new ItemAdapter(list, this);
            mListView.setAdapter(adapter);
        }
        ((TextView) mView.findViewById(R.id.chatTextView)).setText("聊天界面");
//        View item_child = inflater.inflate(R.layout.item_chlid, null);
//        ImageView item_icon = (ImageView)item_child.findViewById(R.id.item_icon);
//        item_icon.setImageDrawable(getResources().getDrawable((R.drawable.mimer)));
        ItemAdapter.OnItemClickListener listener = null;
        listener = new ItemAdapter.OnItemClickListener() {
            @Override
            public void onClick(View v, int position) {
                Toast.makeText(mView.getContext(), "点击了。。。。" + position, Toast.LENGTH_SHORT).show();
                TextView ChatUser = (TextView)v.findViewById(R.id.child_name);
                Intent intent = new Intent(mView.getContext(), ChatActivity.class);
                //用Bundle携带数据
                Bundle bundle = new Bundle();
                String user_name = ChatUser.getText().toString();
                bundle.putString("ChatUser", user_name);
                intent.putExtras(bundle);
                startActivity(intent);
            }
        };
        adapter.setOnItemClickListener(listener);
        return mView;
    }


    private void initData() {
        list = new ArrayList<ItemView>();
        ItemView cb0 = new ItemView("user1", "123");
        ItemView cb1 = new ItemView("user2", "123");
        ItemView cb2 = new ItemView("user3", "123");
        list.add(cb0);
        list.add(cb1);
        list.add(cb2);
    }
}