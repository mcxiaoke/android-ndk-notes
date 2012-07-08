package com.example.helloapp;

public interface StoreListener {
	
	public void onAlert(int value);
	public void onAlert(String value);
	public void onAlert(Color value);

}
