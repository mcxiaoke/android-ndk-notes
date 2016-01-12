package com.mcxiaoke.ndk.udp;

import android.os.Bundle;
import android.widget.EditText;
import butterknife.Bind;

/**
 * Author: mcxiaoke
 * Date:  2016/1/11 22:45
 */
public class EchoClientActivity extends BaseActivity {
    @Bind(R.id.ip_edit)
    EditText ipEdit;

    @Bind(R.id.message_edit)
    EditText messageEdit;


    @Override
    public void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected int layoutId() {
        return R.layout.act_echo_client;
    }

    @Override
    protected void onStartClicked() {
        String ip = ipEdit.getText().toString();
        int port = getPort();
        String message = messageEdit.getText().toString();
        if (ip.length() != 0 && message.length() != 0) {
            ClientThread thread = new ClientThread(ip, port, message);
            thread.start();
        }
    }

    private class ClientThread extends AbstractEchoThread {
        private final String ip;
        private final int port;
        private final String message;

        public ClientThread(String ip, int port, String message) {
            this.ip = ip;
            this.port = port;
            this.message = message;
        }

        @Override
        protected void onBackground() {
            logMessage("Starting Client.");
            try {
                mNative.nativeStartUdpClient(ip, port, message);
            } catch (Exception e) {
                e.printStackTrace();
                logMessage(e.getMessage());
            }
            logMessage("Client terminated.");
        }
    }
}
