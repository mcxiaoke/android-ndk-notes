package com.example.helloapp;

public class Store {

	public native void initializeStore();

	public native void finalizeStore();

	public native synchronized int getInteger(String key)
			throws NotExistingKeyException, InvalidTypeException;

	public native synchronized void setInteger(String key, int value)
			throws StoreFullException;

	public native synchronized String getString(String key)
			throws NotExistingKeyException, InvalidTypeException;

	public native synchronized void setString(String key, String value)
			throws StoreFullException;

	public native synchronized Color getColor(String key)
			throws NotExistingKeyException, InvalidTypeException;

	public native synchronized void setColor(String key, Color color)
			throws StoreFullException;

	public native synchronized int[] getIntegerArray(String key)
			throws NotExistingKeyException;

	public native synchronized void setIntegerArray(String key, int[] intArray);

	public native synchronized Color[] getColorArray(String key)
			throws NotExistingKeyException;

	public native synchronized void setColorArray(String key, Color[] colorArray);

	static {
		System.loadLibrary("store");

	}

}
