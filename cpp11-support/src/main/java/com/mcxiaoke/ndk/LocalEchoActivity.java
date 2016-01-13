package com.mcxiaoke.ndk;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import android.os.Bundle;
import android.widget.EditText;
import butterknife.Bind;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Author: mcxiaoke
 * Date:  2016/1/11 22:45
 */
public class LocalEchoActivity extends BaseActivity {

    @Bind(R.id.message_edit)
    EditText messageEdit;

    private Native mNative;


    @Override
    public void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mNative = new Native(this);
    }

    @Override
    protected int layoutId() {
        return R.layout.act_echo_local;
    }

    @Override
    protected void onStartButtonClicked() {
        String name = portEdit.getText().toString();
        String message = messageEdit.getText().toString();

        if ((name.length() > 0) && (message.length() > 0)) {
            String socketName;

            // If it is a filesystem socket, prepend the
            // application files directory
            if (isFilesystemSocket(name)) {
                File file = new File(getFilesDir(), name);
                socketName = file.getAbsolutePath();
            } else {
                socketName = name;
            }

            ServerTask serverTask = new ServerTask(socketName);
            serverTask.start();

            ClientTask clientTask = new ClientTask(socketName, message);
            clientTask.start();
        }
    }

    private boolean isFilesystemSocket(String name) {
        return name.startsWith("/");
    }

    private void startLocalClient(String name, String message)
            throws Exception {
        // Construct a local socket
        LocalSocket clientSocket = new LocalSocket();
        try {
            // Set the socket namespace
            LocalSocketAddress.Namespace namespace;
            if (isFilesystemSocket(name)) {
                namespace = LocalSocketAddress.Namespace.FILESYSTEM;
            } else {
                namespace = LocalSocketAddress.Namespace.ABSTRACT;
            }

            // Construct local socket address
            LocalSocketAddress address = new LocalSocketAddress(
                    name, namespace);

            // Connect to local socket
            logMessage("Connecting to " + name);
            clientSocket.connect(address);
            logMessage("Connected.");
            // Get message as bytes
            byte[] messageBytes = message.getBytes();
            // Send message bytes to the socket
            logMessage("Sending to the socket...");
            OutputStream outputStream = clientSocket.getOutputStream();
            outputStream.write(messageBytes);
            logMessage(String.format("Sent %d bytes: %s",
                    messageBytes.length, message));

            // Receive the message back from the socket
            logMessage("Receiving from the socket...");
            InputStream inputStream = clientSocket.getInputStream();
            int readSize = inputStream.read(messageBytes);

            String receivedMessage = new String(messageBytes,
                    0, readSize);
            logMessage(String.format("Received %d bytes: %s",
                    readSize, receivedMessage));

            // Close streams
            outputStream.close();
            inputStream.close();

        } finally {
            // Close the local socket
            clientSocket.close();
        }
    }


    private class ServerTask extends AbstractEchoThread {
        private String name;

        public ServerTask(String name) {
            this.name = name;
        }

        @Override
        protected void onBackground() {
            logMessage("Starting server.");
            try {
                mNative.nativeStartLocalServer(name);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }
            logMessage("Server Terminated.");
        }
    }

    private class ClientTask extends Thread {
        private final String name;
        private final String message;

        public ClientTask(String name, String message) {
            this.name = name;
            this.message = message;
        }

        @Override
        public void run() {
            logMessage("Starting client.");
            try {
                startLocalClient(name, message);
            } catch (Exception e) {
                logMessage(e.getMessage());
            }

            logMessage("Client terminated.");
        }
    }


}
