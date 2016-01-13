package com.mcxiaoke.socket.local;

/**
 * Author: mcxiaoke
 * Date:  2016/1/13 20:52
 */
public class Native {

    private LocalEchoActivity mActivity;

    public Native(LocalEchoActivity activity) {
        mActivity = activity;
    }

    public void logMessage(String message) {
        mActivity.logMessage(message);
    }


    public native void nativeStartLocalServer(String name)
            throws Exception;

    static {
        System.loadLibrary("socket");
    }
}
