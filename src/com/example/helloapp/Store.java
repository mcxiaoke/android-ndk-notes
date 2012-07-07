package com.example.helloapp;

public class Store {

	public native int getInteger(String key) throws NotExistingKeyException,
			InvalidTypeException;

	public native void setInteger(String key, int value)
			throws StoreFullException;

	public native String getString(String key) throws NotExistingKeyException,
			InvalidTypeException;

	public native void setString(String key, String value)
			throws StoreFullException;

	public native Color getColor(String key) throws NotExistingKeyException,
			InvalidTypeException;

	public native void setColor(String key, Color color)
			throws StoreFullException;
	
	public native int[] getIntegerArray(String key) throws NotExistingKeyException;
	
	public native void setIntegerArray(String key, int[] intArray);
	
	public native Color[] getColorArray(String key) throws NotExistingKeyException;
	
	public native void setColorArray(String key, Color[] colorArray);

	static {
		System.loadLibrary("store");

	}

}
