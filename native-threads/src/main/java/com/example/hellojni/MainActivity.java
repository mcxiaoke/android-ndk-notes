/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.example.hellojni;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;
import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;
import com.mcxiaoke.ndk.threads.R;


public class MainActivity extends Activity {
    @Bind(R.id.threads_edit)
    EditText threadsEdit;

    @Bind(R.id.iterations_edit)
    EditText iterationsEdit;

    @Bind(R.id.start_button)
    Button startButton;

    @Bind(R.id.log_view)
    TextView logView;

    @Bind(R.id.mode_switch)
    Switch modeSwitch;

    @OnClick(R.id.start_button)
    void onStartClick(final View view) {
        int threads = getNumber(threadsEdit, 0);
        int iterations = getNumber(iterationsEdit, 0);
        if (threads > 0 && iterations > 0) {
            startThreads(threads, iterations);
        }
    }

    private Native mNative;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.act_main);
        ButterKnife.bind(this);
        mNative = new Native(this);
        mNative.nativeInit();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mNative.nativeFree();
    }


    public void onNativeMessage(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logView.append(message);
                logView.append("\n");
            }
        });
    }

    private void startThreads(int threads, int iterations) {
        logView.setText(null);
        if (modeSwitch.isChecked()) {
            logView.append("Mode: posixThreads\n");
            mNative.posixThreads(threads, iterations);
        } else {
            logView.append("Mode: javaThreads\n");
            javaThreads(threads, iterations);
        }
    }

    private void javaThreads(int threads, final int iterations) {
        for (int i = 0; i < threads; ++i) {
            final int id = i;
            new Thread() {
                @Override
                public void run() {
                    mNative.nativeWorker(id, iterations);
                }
            }.start();
        }
    }

    private static int getNumber(EditText t, int defaultValue) {
        int value;
        try {
            value = Integer.parseInt(t.getText().toString());
        } catch (NumberFormatException e) {
            value = defaultValue;
        }
        return value;
    }


}
