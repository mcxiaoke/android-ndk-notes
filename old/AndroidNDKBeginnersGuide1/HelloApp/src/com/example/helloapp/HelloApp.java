package com.example.helloapp;

import java.util.Arrays;
import java.util.List;

import com.google.common.base.Function;
import com.google.common.base.Joiner;
import com.google.common.collect.Lists;
import com.google.common.primitives.Ints;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.SpinnerAdapter;
import android.widget.Toast;

public class HelloApp extends Activity implements OnClickListener,
		StoreListener {
	// TODO 学习到了132/143页

	private EditText mKeyEdit, mValueEdit;
	private Spinner mTypeSpinner;
	private Button mGetButton, mSetButton;
	private Store mStore;
	private ArrayAdapter<String> mTypeValues;
	private static final String types[] = { "Integer", "String", "Color" };

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		mKeyEdit = (EditText) findViewById(R.id.key);
		mValueEdit = (EditText) findViewById(R.id.value);
		mTypeSpinner = (Spinner) findViewById(R.id.type);
		mGetButton = (Button) findViewById(R.id.get);
		mSetButton = (Button) findViewById(R.id.set);

		mGetButton.setOnClickListener(this);
		mSetButton.setOnClickListener(this);

		mTypeValues = new ArrayAdapter<String>(this,
				android.R.layout.simple_spinner_item, types);
		mTypeValues
				.setDropDownViewResource(android.R.layout.simple_dropdown_item_1line);
		mTypeSpinner.setAdapter(mTypeValues);
		
		setTitle(NativeUtils.getTitle());
		mStore = new Store(this);

	}

	@Override
	protected void onStart() {
		super.onStart();
		mStore.initializeStore();
//		try {
//			mStore.setInteger("watcherCounter", 0);
//		} catch (StoreFullException e) {
//			error(e.toString());
//		}
	}

	@Override
	protected void onResume() {
		super.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onStop() {
		super.onStop();
		mStore.finalizeStore();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	private void onGetValue() {
		String key = mKeyEdit.getText().toString();
		String typename = (String) mTypeSpinner.getSelectedItem();
		StoreType type = (StoreType) StoreType.valueOf(typename);
		try {
			switch (type) {
			case Integer:
				mValueEdit.setText(Integer.toString(mStore.getInteger(key)));
				error("Integer value is "
						+ Integer.toString(mStore.getInteger(key)));
				break;
			case String:
				mValueEdit.setText(mStore.getString(key));
				error("String value is " + mStore.getString(key));
				break;
			case Color:
				mValueEdit.setText(mStore.getColor(key).toString());
				error("Color value is " + mStore.getColor(key).toString());
				break;
			case IntegerArray:
				mValueEdit.setText(Ints.join(";", mStore.getIntegerArray(key)));
				error("IntegerArray value is "
						+ Ints.join(";", mStore.getIntegerArray(key)));
				break;
			case ColorArray:
				mValueEdit.setText(Joiner.on(";").join(
						mStore.getColorArray(key)));
				error("ColorArray value is "
						+ Joiner.on(";").join(mStore.getColorArray(key)));
				break;
			default:
				break;
			}
		} catch (NotExistingKeyException e) {
			error(e.toString());
		} catch (InvalidTypeException e) {
			error(e.toString());
		}
	}

	private void onSetValue() {
		String key = mKeyEdit.getText().toString();
		String value = mValueEdit.getText().toString();
		String typename = (String) mTypeSpinner.getSelectedItem();
		StoreType type = (StoreType) StoreType.valueOf(typename);
		try {
			switch (type) {
			case Integer:
				mStore.setInteger(key, Integer.parseInt(value));
				break;
			case String:
				mStore.setString(key, value);
				break;
			case Color:
				mStore.setColor(key, new Color(value));
				break;
			case IntegerArray:
				mStore.setIntegerArray(key, Ints.toArray(stringToList(
						new Function<String, Integer>() {
							public Integer apply(String str) {
								return Integer.parseInt(str);
							}
						}, value)));
				break;
			case ColorArray:
				List<Color> colors = stringToList(
						new Function<String, Color>() {
							public Color apply(String str) {
								return new Color(str);
							}
						}, value);
				Color[] colorArray = colors.toArray(new Color[colors.size()]);
				mStore.setColorArray(key, colorArray);
				break;
			default:
				break;
			}
		} catch (StoreFullException e) {
			error(e.toString());
		}
	}

	private <TType> List<TType> stringToList(
			Function<String, TType> conversation, String value) {
		String[] array = value.split(";");
		List<String> list = Arrays.asList(array);
		return Lists.transform(list, conversation);
	}

	private void error(String text) {
		Toast.makeText(this, text, Toast.LENGTH_SHORT).show();
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		int id = v.getId();
		switch (id) {
		case R.id.get:
			onGetValue();
			break;
		case R.id.set:
			onSetValue();
			break;
		default:
			break;
		}
	}

	@Override
	public void onAlert(int value) {
		error(String.format("%1$d is not an allowed integer.", value));
	}

	@Override
	public void onAlert(String value) {
		error(String.format("%1$s is not an allowed string.", value));
	}

	@Override
	public void onAlert(Color value) {
		error(String.format("%1$s is not an allowed color.", value));
	}

}
