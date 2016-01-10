package com.example.hellojni;

import android.util.Log;

/**
 * Author: mcxiaoke
 * Date:  2016/1/10 21:12
 */
public class Native {
    private MainActivity main;

    public Native(MainActivity mainActivity) {
        this.main = mainActivity;
    }

    static {
        System.loadLibrary("threads");
    }

    public native void nativeInit();

    public native void nativeFree();

    public native void nativeWorker(int id, int iterations);

    public void onNativeMessage(final String message) {
        Log.d("Native", "onNativeMessage() " + message);
        this.main.onNativeMessage(message);
    }

}
