package com.example.hellojni;

import android.widget.TextView;

/**
 * Author: mcxiaoke
 * Date:  2016/1/10 10:03
 */
public class UidHandler extends AsyncUidProvider {
    private final TextView tv;

    public UidHandler(TextView tv) {
        this.tv = tv;
    }

    @Override
    public void onUid(final long uid) {
        tv.append(String.format("\nUID: %d", uid));
    }
}
