package com.demo.livecamera;

import android.os.Bundle;
import android.app.Activity;

public class LiveCamera extends Activity {

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// setContentView(R.layout.main);
		setContentView(new CameraView(this));
	}

}
