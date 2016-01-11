package com.example.hellojni;

import com.mcxiaoke.ndk.tcp.BaseActivity;

/**
 * Author: mcxiaoke
 * Date:  2016/1/10 21:12
 */
public class Native {
    private BaseActivity activity;

    public Native(BaseActivity mainActivity) {
        this.activity = mainActivity;
    }

    static {
        System.loadLibrary("echo");
    }

    public void logMessage(final String message) {
        activity.logMessage(message);
    }

    public native void nativeStartTcpServer(int port) throws Exception;

    public native void nativeStartTcpClient(String ip, int port, String message) throws Exception;


    public native void nativeStartUdpServer(int port) throws Exception;

    public native void nativeStartUdpClient(String ip, int port, String message)
            throws Exception;

}

