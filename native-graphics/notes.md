In C++ code, JNIEnv is actually a C++ class instance. JNI functions are exposed as member
functions. Since JNI methods have access to the current JNI environment, the JNI method calls do
not require the JNIEnv instance as an argument. In C++, the same code looks like
     return env->NewStringUTF("Hello from JNI !");

Primitive types: boolean, byte, char, short, int, long, float, and double
Reference types: String, arrays, and other classes


Java Type JNI Type C/C++ Type Size
Boolean     Jboolean        unsigned char       Unsigned 8 bits
Byte        Jbyte           char                Signed 8 bits
Char        Jchar           unsigned short      Unsigned 16 bits
Short       Jshort          short               Signed 16 bits
Int         Jint            Int                 Signed 32 bits
Long        Jlong           long                long Signed 64 bits
Float       Jfloat          float               32 bits
Double      Jdouble         double              64 bits


Java Type               Native Type
java.lang.Class         jclass
java.lang.Throwable     jthrowable
java.lang.String        jstring
Other objects           jobject
java.lang.Object[]      jobjectArray
boolean[]               jbooleanArray
byte[]                  jbyteArray
char[]                  jcharArray
short[]                 jshortArray
int[]                   jintArray
long[]                  jlongArray
float[]                 jfloatArray
double[]                jdoubleArray
Other arrays            Jarray


Operations on Reference Types
 Strings
 Arrays
 NIO Buffers
 Fields
 Methods


New String

using the functions NewString for Unicode strings and NewStringUTF for UTF-8 strings
jstring javaString;
javaString = (*env)->NewStringUTF(env, "Hello World!");

Converting a Java String to C String

const jbyte* str;
jboolean isCopy;
str = (*env)->GetStringUTFChars(env, javaString, &isCopy);
if (0 != str) {
printf("Java string: %s", str);
if (JNI_TRUE == isCopy) {
printf("C string is a copy of the Java string.");
} else {
printf("C string points to actual string.");
}

(*env)->ReleaseStringUTFChars(env, javaString, str);

Array Operations

New Array

jintArray javaArray;
javaArray = (*env)->NewIntArray(env, 10);
if (0 != javaArray) {
/* You can now use the array. */
}

Operating on a Copy

jint nativeArray[10];
(*env)->GetIntArrayRegion(env, javaArray, 0, 10, nativeArray);
(*env)->SetIntArrayRegion(env, javaArray, 0, 10, nativeArray);

Operating on Direct Pointer

jint* nativeDirectArray;
jboolean isCopy;
nativeDirectArray = (*env)->GetIntArrayElements(env, javaArray, &isCopy);
(*env)->ReleaseIntArrayElements(env, javaArray, nativeDirectArray, 0);

Direct Byte Buffer

unsigned char* buffer = (unsigned char*) malloc(1024);
// ...
jobject directBuffer;
directBuffer = (*env)->NewDirectByteBuffer(env, buffer, 1024);
unsigned char* buffer;
buffer = (unsigned char*) (*env)->GetDirectBufferAddress(env,
directBuffer);

Accessing Fields
// filed id can cached

jclass clazz;
clazz = (*env)->GetObjectClass(env, instance);

jfieldID instanceFieldId;
instanceFieldId = (*env)->GetFieldID(env, clazz, "instanceField", "Ljava/lang/String;");

jfieldID staticFieldId;
staticFieldId = (*env)->GetStaticFieldID(env, clazz, "staticField", "Ljava/lang/String;");

jstring staticField;
staticField = (*env)->GetStaticObjectField(env, clazz, staticFieldId);

Getting the value of a single field takes two or three JNI function calls. Native code reaching back
to Java to obtain values of each individual field adds extra overhead to the application and leads to
poorer performance. It is strongly recommended to pass all needed parameters to native method calls
instead of having the native code reach back to Java.

Calling Methods
// method id can be cached

jmethodID instanceMethodId;
instanceMethodId = (*env)->GetMethodID(env, clazz, "instanceMethod", "()Ljava/lang/String;");

jmethodID staticMethodId;
staticMethodId = (*env)->GetStaticMethodID(env, clazz,"staticMethod", "()Ljava/lang/String;");

jstring instanceMethodResult;
instanceMethodResult = (*env)->CallStringMethod(env, instance, instanceMethodId);

// Minimizing these transitions can benefit the application performance greatly.

Field and Method Descriptors

Table 3-4. Java Type Signature Mapping
Java Type           Signature
Boolean             Z
Byte                B
Char                C
Short               S
Int                 I
Long                J
Float               F
Double              D
fullClassName       LfullClassName;
type[]              [type
method type         (arg-type)ret-type

Java Class File Disassembler: javap
javap –classpath bin/classes –p –s com.example.hellojni.HelloJni


Catching Exceptions

jthrowable ex;
//...
(*env)->CallVoidMethod(env, instance, throwingMethodId);
ex = (*env)->ExceptionOccurred(env);
if (0 != ex) {
(*env)->ExceptionClear(env);
/* Exception handler. */
}

Throwing Exceptions

jclass clazz;
//...
clazz = (*env)->FindClass(env, "java/lang/NullPointerException");
if (0 ! = clazz) {
(*env)->ThrowNew(env, clazz, "Exception message.");
}

Local and Global References

The JNI supports three type kinds of references: local references, global references, and weak global references

Local references are freed once the native function returns.

New Global Reference

jclass localClazz;
jclass globalClazz;
//...
localClazz = (*env)->FindClass(env, "java/lang/String");
globalClazz = (*env)->NewGlobalRef(env, localClazz);
//...
(*env)->DeleteLocalRef(env, localClazz);
(*env)->DeleteGlobalRef(env, globalClazz);

jclass weakGlobalClazz;
weakGlobalClazz = (*env)->NewWeakGlobalRef(env, localClazz);

// Validating a Weak Global Reference
if (JNI_FALSE == (*env)->IsSameObject(env, weakGlobalClazz, NULL)) {
/* Object is still live and can be used. */
} else {
/* Object is garbage collected and cannot be used. */
}
(*env)->DeleteWeakGlobalRef(env, weakGlobalClazz);

Threading

Local references are valid only during the execution of the native method and in
the thread context that is executing the native method. Local references cannot
be shared among multiple threads. Only global references can be shared by
multiple threads.

The JNIEnv interface pointer that is passed into each native method call is also
valid in the thread associated with the method call. It cannot be cached and
used by other threads.

// Java
synchronized(obj) {
/* Synchronized thread-safe code block. */
}

// Cpp
if (JNI_OK == (*env)->MonitorEnter(env, obj)) {
/* Error handling. */
}
/* Synchronized thread-safe code block. */
if (JNI_OK == (*env)->MonitorExit(env, obj)) {
/* Error handling. */
}

Native Threads

Native threads should be attached to the virtual machine first in order to interact
with the remaining portion of the application.

JavaVM* cachedJvm;
...
JNIEnv* env;
...
/* Attach the current thread to virtual machine. */
(*cachedJvm)->AttachCurrentThread(cachedJvm, &env, NULL);
/* Thread can communicate with the Java application
using the JNIEnv interface. */
/* Detach the current thread from virtual machine. */
(*cachedJvm)->DetachCurrentThread(cachedJvm);





















