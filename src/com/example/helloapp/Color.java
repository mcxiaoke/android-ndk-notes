package com.example.helloapp;

public class Color {
	private int mColor;
	
	public Color(String color){
		mColor=android.graphics.Color.parseColor(color);
	}
	
	@Override
	public String toString(){
		return String.format("#%06x", mColor);
	}

}
