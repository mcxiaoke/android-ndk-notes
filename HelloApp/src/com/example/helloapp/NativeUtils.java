package com.example.helloapp;

public final class NativeUtils {
	public static native String getTitle();
	public native long getTime();
	
	static{
		System.loadLibrary("hello");
	}
}
