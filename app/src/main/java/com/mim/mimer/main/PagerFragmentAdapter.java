package com.mim.mimer.main;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

public class PagerFragmentAdapter extends FragmentPagerAdapter {

    private List<Fragment> mList = new ArrayList<Fragment>();

    public PagerFragmentAdapter(FragmentManager fm, List<Fragment> list) {
        super(fm);
        this.mList = list;
    }

    @Override
    public Fragment getItem(int position) {
        return mList.get(position);
    }

    @Override
    public int getCount() {
        return mList != null ? mList.size() : 0;
    }
}