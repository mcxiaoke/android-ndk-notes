package com.example.helloapp;

public class Color {
	private int mColor;

	@Override
	public boolean equals(Object o) {
		if (this == o) {
			return true;
		}
		if (o == null) {
			return false;
		}
		if (getClass() != o.getClass()) {
			return false;
		}
		Color color = (Color) o;
		return (mColor == color.mColor);
	}

	@Override
	public int hashCode() {
		return mColor;
	}

	public Color(String color) {
		mColor = android.graphics.Color.parseColor(color);
	}

	@Override
	public String toString() {
		return String.format("#%06x", mColor);
	}

}
