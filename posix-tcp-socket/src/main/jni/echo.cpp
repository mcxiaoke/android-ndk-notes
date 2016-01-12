/*
* @Author: mcxiaoke
* @Date:   2016-01-11 21:04:57
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-12 22:28:46
*/

// JNI
#include <jni.h>
// NULL
#include <stdio.h>
// va_list, vsnprintf
#include <stdarg.h>
// errno
#include <errno.h>
// strerror_r, memset
#include <string.h>
// socket, bind, getsockname, listen, accept, recv, send, connect
#include <sys/types.h>
#include <sys/socket.h>
// sockaddr_un
#include <sys/un.h>
// htons, sockaddr_in
#include <netinet/in.h>
// inet_ntop
#include <arpa/inet.h>
// close, unlink
#include <unistd.h>

// offsetof
#include <stddef.h>
// little endian
// #include <sys/endian.h>

#include "nlog.h"
#include "com_example_hellojni_Native.h"


// Max log message length
#define MAX_LOG_MESSAGE_LENGTH 256
// Max data buffer size
#define MAX_BUFFER_SIZE 80

// send message to Java
static void LogMessage(JNIEnv* env, jobject obj, const char* format, ...) {
    static jmethodID methodID = NULL;
    if (NULL == methodID) {
        jclass clazz = env->GetObjectClass(obj);
        methodID = env->GetMethodID(clazz, "logMessage", "(Ljava/lang/String;)V");
        env->DeleteLocalRef(clazz);
    }
    if (NULL != methodID) {
        char buffer[MAX_LOG_MESSAGE_LENGTH];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, MAX_LOG_MESSAGE_LENGTH, format, ap);
        va_end(ap);

        jstring message = env->NewStringUTF(buffer);
        if (NULL != message) {
            env->CallVoidMethod(obj, methodID, message);
            env->DeleteLocalRef(message);
        }
    }
}

// throw a exception
static void ThrowException(JNIEnv* env, const char* className, const char* message) {
    jclass clazz = env->FindClass(className);
    if (NULL != clazz) {
        env->ThrowNew(clazz, message);
        env->DeleteLocalRef(clazz);
    }
}

// throw errno exception
static void ThrowErrnoException(JNIEnv* env, const char* className, int errnum) {
    char buffer[MAX_LOG_MESSAGE_LENGTH];
    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MESSAGE_LENGTH)) {
        strerror_r(errno, buffer, MAX_LOG_MESSAGE_LENGTH);
        ThrowException(env, className, buffer);
    }
}

// create a tcp socket
static int NewTcpSocket(JNIEnv* env, jobject obj) {
    LogMessage(env, obj, "Constructing a new TCP socket...");
    int tcpSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == tcpSocket) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    return tcpSocket;
}

// create a udp socket
static int NewTcpSocket(JNIEnv* env, jobject obj) {
    LogMessage(env, obj, "Constructing a new UDP socket...");
    int udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 == udpSocket) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    return udpSocket;
}

// bind socket to port
static void BindSocketToPort(JNIEnv* env, jobject obj, int sd, unsigned short port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    LogMessage(env, obj, "Binding to %s:%hu.", inet_ntoa(address.sin_addr), port);
    if (-1 == bind(sd, (struct sockaddr*) &address, sizeof(address))) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
}

// get binded socket port
static unsigned short GetSocketPort(JNIEnv* env, jobject obj, int sd) {
    unsigned short port = 0;
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    if (-1 == getsockname(sd, (struct sockaddr*)&address, &addressLength)) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        port = ntohs(address.sin_port);
        LogMessage(env, obj, "Binded to random port %hu", port);
    }
    return port;
}

// listen
static void ListenOnSocket(JNIEnv* env, jobject obj, int sd, int backlog) {
    LogMessage(env, obj, "Listening on socket with a backlog of %d pending connections.", backlog);
    if (-1 == listen(sd, backlog)) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
}

// log the ip and port
static void LogAddress(JNIEnv* env, jobject obj, const char* message, const struct sockaddr_in* address) {
    char ip[INET_ADDRSTRLEN];
    if (NULL == inet_ntop(PF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN)) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        unsigned short port = ntohs(address->sin_port);
        LogMessage(env, obj, "%s %s:%hu", message, ip, port);
    }
}

// block and waits incoming client on socket
static int AcceptOnSocket(JNIEnv* env, jobject obj, int sd) {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    LogMessage(env, obj, "Waiting for a client connection...");
    int clientSocket = accept(sd, (struct sockaddr*) &address, &addressLength);
    if (-1 == clientSocket) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        LogMessage(env, obj, "Client connection from %s:%hu."
                   , inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        struct sockaddr_in serv, guest;
        char serv_ip[20];
        char guest_ip[20];
        socklen_t serv_len = sizeof(serv);
        socklen_t guest_len = sizeof(guest);
        getsockname(clientSocket, (struct sockaddr *)&serv, &serv_len);
        getpeername(clientSocket, (struct sockaddr *)&guest, &guest_len);
        inet_ntop(AF_INET, &serv.sin_addr, serv_ip, sizeof(serv_ip));
        inet_ntop(AF_INET, &guest.sin_addr, guest_ip, sizeof(guest_ip));
        LogMessage(env, obj, "Server %s:%d\nClient %s:%d\n", serv_ip,
                   ntohs(serv.sin_port), guest_ip, ntohs(guest.sin_port));
    }
    return clientSocket;
}

// receive data from socket
static ssize_t ReceiveFromSocket(JNIEnv* env, jobject obj, int sd, char* buffer, size_t bufferSize) {
    LogMessage(env, obj, "Receiving from socket...");
    ssize_t recvSize = recv(sd, buffer, bufferSize - 1, 0);
    if (-1 == recvSize) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        buffer[recvSize] = 0;
        if (recvSize > 0) {
            LogMessage(env, obj, "Received %d bytes: %s", recvSize, buffer);
        } else {
            LogMessage(env, obj, "Client disconnected.");
        }
    }
    return recvSize;
}

// receive datagram from udp socket
static ssize_t ReceiveDatagramFromSocket(
    JNIEnv* env,
    jobject obj,
    int sd,
    struct sockaddr_in* address,
    char* buffer,
    size_t bufferSize)
{
    socklen_t addressLength = sizeof(struct sockaddr_in);

    // Receive datagram from socket
    LogMessage(env, obj, "Receiving datagram from the socket...");
    ssize_t recvSize = recvfrom(sd, buffer, bufferSize, 0,
                                (struct sockaddr*) address,
                                &addressLength);

    // If receive is failed
    if (−1 == recvSize)
    {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    else
    {
        // Log address
        LogAddress(env, obj, "Received datagram from", address);

        // NULL terminate the buffer to make it a string
        buffer[recvSize] = NULL;

        // If data is received
        if (recvSize > 0)
        {
            LogMessage(env, obj, "Received %d bytes: %s",
                       recvSize, buffer);
        }
    }

    return recvSize;
}

// send data to socket
static ssize_t SendToSocket(JNIEnv* env, jobject obj, int sd, const char* buffer, size_t bufferSize) {
    LogMessage(env, obj, "Sending to the socket...");
    ssize_t sentSize = send(sd, buffer, bufferSize, 0);
    if (-1 == sentSize) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        if (sentSize > 0) {
            LogMessage(env, obj, "Sent %d bytes: %s", sentSize, buffer);
        } else {
            LogMessage(env, obj, "Client disconnected.");
        }
    }
    return sentSize;
}

// send datagram to socket
static ssize_t SendDatagramToSocket(
    JNIEnv* env,
    jobject obj,
    int sd,
    const struct sockaddr_in* address,
    const char* buffer,
    size_t bufferSize) {
    LogMessage(env, obj, "Sending datagram to ", address);
    ssize_t sentSize = sendto(sd, buffer, bufferSize, 0,
                              (const sockaddr*)address, sizeof(struct sockaddr_in));
    if (-1 == sentSize) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        if (sentSize > 0) {
            LogMessage(env, obj, "Sent %d bytes: %s", sentSize, buffer);
        }
    }
    return sentSize;
}

// connect to ip:port
static void ConnectToAddress(JNIEnv* env, jobject obj, int sd, const char* ip, unsigned short port) {
    LogMessage(env, obj, "Connecting to %s:%hu...", ip, port);
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;
    if (0 == inet_aton(ip, &(address.sin_addr))) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        address.sin_port = htons(port);
        if (-1 == connect(sd, (const sockaddr*)&address, sizeof(address))) {
            ThrowErrnoException(env, "java/io/IOException", errno);
        } else {
            LogMessage(env, obj, "Connected.");
        }
    }
}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeStartTcpServer
(JNIEnv *env, jobject obj, jint port) {
// Construct a new TCP socket.
    int serverSocket = NewTcpSocket(env, obj);
    if (NULL == env->ExceptionOccurred())
    {
        // Bind socket to a port number
        BindSocketToPort(env, obj, serverSocket, (unsigned short) port);
        if (NULL != env->ExceptionOccurred())
            goto exit;

        // If random port number is requested
        if (0 == port)
        {
            // Get the port number socket is currently binded
            GetSocketPort(env, obj, serverSocket);
            if (NULL != env->ExceptionOccurred())
                goto exit;
        }

        // Listen on socket with a backlog of 4 pending connections
        ListenOnSocket(env, obj, serverSocket, 4);
        if (NULL != env->ExceptionOccurred())
            goto exit;

        // Accept a client connection on socket
        int clientSocket = AcceptOnSocket(env, obj, serverSocket);
        if (NULL != env->ExceptionOccurred())
            goto exit;

        char buffer[MAX_BUFFER_SIZE];
        ssize_t recvSize;
        ssize_t sentSize;

        // Receive and send back the data
        while (1)
        {
            // Receive from the socket
            recvSize = ReceiveFromSocket(env, obj, clientSocket,
                                         buffer, MAX_BUFFER_SIZE);

            if ((0 == recvSize) || (NULL != env->ExceptionOccurred()))
                break;

            // Send to the socket
            char prefix[] = "Server: ";
            char toSend[MAX_BUFFER_SIZE + strlen(prefix)];
            strcpy(toSend, prefix);
            strcat(toSend, buffer);
            sentSize = SendToSocket(env, obj, clientSocket,
                                    toSend, (size_t) strlen(toSend));
            if ((0 == sentSize) || (NULL != env->ExceptionOccurred()))
                break;
        }

        // Close the client socket
        close(clientSocket);
    }

exit:
    if (serverSocket > 0)
    {
        close(serverSocket);
    }
}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeStartTcpClient
(JNIEnv *env, jobject obj, jstring ip, jint port, jstring message)
{
    // Construct a new TCP socket.
    int clientSocket = NewTcpSocket(env, obj);
    if (NULL == env->ExceptionOccurred())
    {
        // Get IP address as C string
        const char* ipAddress = env->GetStringUTFChars(ip, NULL);
        if (NULL == ipAddress)
            goto exit;

        // Connect to IP address and port
        ConnectToAddress(env, obj, clientSocket, ipAddress,
                         (unsigned short) port);

        // Release the IP address
        env->ReleaseStringUTFChars(ip, ipAddress);

        // If connection was successful
        if (NULL != env->ExceptionOccurred())
            goto exit;

        // Get message as C string
        const char* messageText = env->GetStringUTFChars(message, NULL);
        if (NULL == messageText)
            goto exit;

        // Get the message size
        jsize messageSize = env->GetStringUTFLength(message);

        // Send message to socket
        SendToSocket(env, obj, clientSocket, messageText, messageSize);

        // Release the message text
        env->ReleaseStringUTFChars(message, messageText);

        // If send was not successful
        if (NULL != env->ExceptionOccurred())
            goto exit;

        char buffer[MAX_BUFFER_SIZE];

        // Receive from the socket
        ReceiveFromSocket(env, obj, clientSocket, buffer, MAX_BUFFER_SIZE);
    }

exit:
    if (clientSocket > 0)
    {
        close(clientSocket);
    }
}
