package com.mcxiaoke.ndk;

import android.app.Activity;
import android.app.AlertDialog;

import java.io.IOException;

/**
 * Author: mcxiaoke
 * Date:  2016/1/13 22:39
 */
public abstract class AbstractPlayerActivity extends Activity {
    /**
     * AVI file name extra.
     */
    public static final String EXTRA_FILE_NAME =
            "com.apress.aviplayer.EXTRA_FILE_NAME";

    /**
     * AVI video file descriptor.
     */
    protected long avi = 0;

    /**
     * On start.
     */
    protected void onStart() {
        super.onStart();

        // Open the AVI file
        try {
            avi = Native.open(getFileName());
        } catch (IOException e) {
            new AlertDialog.Builder(this)
                    .setTitle(R.string.error_alert_title)
                    .setMessage(e.getMessage())
                    .show();
        }
    }

    /**
     * On stop.
     */
    protected void onStop() {
        super.onStop();

        // If the AVI video is open
        if (0 != avi) {
            // Close the file descriptor
            Native.close(avi);
            avi = 0;
        }
    }

    /**
     * Gets the AVI video file name.
     *
     * @return file name.
     */
    protected String getFileName() {
        return getIntent().getExtras().getString(EXTRA_FILE_NAME);
    }
}
