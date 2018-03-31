package com.mim.mimer.main;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;


import com.mim.mimer.R;

public class ItemAdapter extends RecyclerView.Adapter<ItemAdapter.MyViewHolder> implements View.OnClickListener {

    private List<ItemView> mList;
    private Fragment mContext;
    private OnItemClickListener mOnItemClickListener = null;

    //用一个集合将适配中创建的所有 holder对象存储到这个容器中，因为本类中有对holder中的控件创建了监听事件
    //如果不取消监听，可能会出现内存泄漏的问题，所以将holder保存，创建一个方法将绑定监听事件的控件的监听事件至空(个人观点)
    private List<MyViewHolder> mListHolder;

    //添加一个销毁的方法，这样可以加快回收，可以解决内存泄漏的问题，
    //可以用android studio 进行查看内存，多次开启和关闭程序，查看内存内存走势，
    //如果不填加次方法，那么内存会一直增加，并且点GC 内存也不会下降。
    //在activity的销毁方法中添加该方法，就解决了内存泄漏的方法。(这个方法不是最好的，但目前想到的解决方案只有这个，欢迎指点留言)
    public void onDestroy() {
        mList.clear();
        mList = null;
        for (int i = 0; i < mListHolder.size(); i++) {
            mListHolder.get(i).itemView.setOnClickListener(null);
        }
        mListHolder.clear();
        mListHolder = null;
    }


    public ItemAdapter(List<ItemView> list, Fragment context) {
        this.mContext = context;
        this.mList = list;
        this.mListHolder = new ArrayList<MyViewHolder>();
    }

    /**
     * 创建一个监听事件的接口；重要
     */
    public interface OnItemClickListener {
        void onClick(View v, int position);
    }

    /**
     * 外界进行调用该方法，为item设置点击事件；重要
     *
     * @param listener
     */
    public void setOnItemClickListener(OnItemClickListener listener) {
        this.mOnItemClickListener = listener;
    }


    @Override
    public ItemAdapter.MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(mContext.getContext()).inflate(R.layout.item_chlid, parent, false);
        //为每个item设置点击事件；
        view.setOnClickListener(this);
        return new MyViewHolder(view);
    }

    /**
     * 这个是继承View.OnClickListener 实现的方法; 重要
     *
     * @param v 当前被点击的v;
     */
    @Override
    public void onClick(View v) {
        //判断当前是否为item设置监听事件
        if (mOnItemClickListener != null) {
            //如果设置了，那么回调该方法，由于view的tag是object类型的，希望能回调到当前所显示到第几项item所以进行类型转换，希望有更好的方法请赐教；
            mOnItemClickListener.onClick(v,(int)v.getTag());
        }
    }

    @Override
    public void onBindViewHolder(ItemAdapter.MyViewHolder holder, int position) {
        //一定要设置这个。要不在回调方法里面获得不到当前点击的是第几个item;注意tag是object类型的；重要；
        final ItemView item = mList.get(position);
        holder.name.setText(item.getName());
        holder.sign.setText(item.getSign());
        holder.itemView.setTag(position);
    }

    @Override
    public int getItemCount() {
        return mList != null ? mList.size() : 0;
    }

    /**
     * 希望读者有良好的编码习惯，将ViewHolder类写成静态的.
     **/
    static class MyViewHolder extends RecyclerView.ViewHolder {
        public TextView name;
        public TextView sign;
        public View itemView;
        public MyViewHolder(View iView) {
            super(iView);
            itemView = iView;
            name = (TextView) iView.findViewById(R.id.child_name);
            sign = (TextView) iView.findViewById(R.id.child_sign);
        }
    }
}