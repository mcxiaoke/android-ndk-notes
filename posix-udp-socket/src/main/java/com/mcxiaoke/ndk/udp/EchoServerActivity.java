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
package com.mcxiaoke.ndk.udp;

import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class EchoServerActivity extends BaseActivity {

    @Override
    public void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public boolean onCreateOptionsMenu(final Menu menu) {
        menu.add(0, 0, 0, "Open Client");
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(final MenuItem item) {
        if (item.getItemId() == 0) {
            startActivity(new Intent(this, EchoClientActivity.class));
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected int layoutId() {
        return R.layout.act_echo_server;
    }

    @Override
    protected void onStartClicked() {
        int port = getPort();
        ServerThread thread = new ServerThread(port);
        thread.start();
    }

    private class ServerThread extends AbstractEchoThread {
        private final int port;

        public ServerThread(int port) {
            this.port = port;
        }


        @Override
        protected void onBackground() {
            logMessage("Starting server.");
            try {
                mNative.nativeStartUdpServer(port);
            } catch (Exception e) {
                e.printStackTrace();
                logMessage(e.getMessage());
            }
            logMessage("Server terminated.");
        }

        @Override
        protected void onPostExecute() {
            super.onPostExecute();
        }
    }
}
