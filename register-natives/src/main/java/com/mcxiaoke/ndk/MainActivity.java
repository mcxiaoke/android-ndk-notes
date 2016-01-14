package com.mcxiaoke.ndk;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;

import java.io.File;

/**
 * Author: mcxiaoke
 * Date:  2016/1/13 22:34
 */
public class MainActivity extends Activity implements OnClickListener {
    /**
     * AVI file name edit.
     */
    private EditText fileNameEdit;

    /**
     * Player type radio group.
     */
    private RadioGroup playerRadioGroup;

    /**
     * Play button.
     */
    private Button playButton;

    /**
     * On create.
     *
     * @param savedInstanceState saved state.
     */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        fileNameEdit = (EditText) findViewById(R.id.file_name_edit);
        playerRadioGroup = (RadioGroup) findViewById(
                R.id.player_radio_group);

        playButton = (Button) findViewById(R.id.play_button);
        playButton.setOnClickListener(this);
    }

    /**
     * On click event handler.
     *
     * @param view view instance.
     */
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.play_button:
                onPlayButtonClick();
                break;
        }
    }

    /**
     * On play button click event handler.
     */
    private void onPlayButtonClick() {
        Intent intent;

        // Get the checked radio button id
        int radioId = playerRadioGroup.getCheckedRadioButtonId();

        // Choose the activity based on id
        switch (radioId) {
            case R.id.bitmap_player_radio:
                intent = new Intent(this, BitmapPlayerActivity.class);
                break;
            case R.id.opengl_player_radio:
                intent = new Intent(this, OpenGLPlayerActivity.class);
                break;
            case R.id.native_window_player_radio:
                intent = new Intent(this, NativeWindowPlayerActivity.class);
                break;
            default:
                throw new UnsupportedOperationException(
                        "radioId=" + radioId);
        }

        // Under the external storage
        File file = new File(Environment.getExternalStorageDirectory(),
                fileNameEdit.getText().toString());

        Log.d("DEBUG", "File: " + file);

        // Put AVI file name as extra
        intent.putExtra(AbstractPlayerActivity.EXTRA_FILE_NAME,
                file.getAbsolutePath());

        // Start the player activity
        startActivity(intent);
    }
}
