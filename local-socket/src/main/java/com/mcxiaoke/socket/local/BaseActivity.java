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
package com.mcxiaoke.socket.local;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;


public abstract class BaseActivity extends Activity {


    @Bind(R.id.port_edit)
    EditText portEdit;

    @Bind(R.id.start_button)
    Button startButton;

    @Bind(R.id.log_scroll)
    ScrollView logScroll;

    @Bind(R.id.log_view)
    TextView logView;

    @OnClick(R.id.start_button)
    void onStartClick(final View view) {
        onStartButtonClicked();
    }

    protected abstract int layoutId();

    protected abstract void onStartButtonClicked();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layoutId());
        ButterKnife.bind(this);
        logMessage("Local IP:\n" + Utils.getLocalIpAddress());
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    protected int getPort() {
        return Integer.valueOf(portEdit.getText().toString());
    }

    public void logMessage(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logMessageDirect(message);
            }
        });
    }

    protected void logMessageDirect(final String message) {
        logView.append(message);
        logView.append("\n");
        logScroll.fullScroll(View.FOCUS_DOWN);
    }

    protected abstract class AbstractEchoThread extends Thread {
        private final Handler mHandler;

        public AbstractEchoThread() {
            mHandler = new Handler();
        }

        protected void onPreExecute() {
            startButton.setEnabled(false);
            logView.setText("");
        }

        @Override
        public synchronized void start() {
            onPreExecute();
            super.start();
        }

        @Override
        public void run() {
            onBackground();
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    onPostExecute();
                }
            });
        }

        protected abstract void onBackground();

        protected void onPostExecute() {
            startButton.setEnabled(true);
        }


    }


}
