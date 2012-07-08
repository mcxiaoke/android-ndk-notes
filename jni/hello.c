#include "hello.h"

JavaVM *mGlobalJavaVM;

jint JNI_OnLoad(JavaVM* vm, void *reserved) {
	mGlobalJavaVM = vm;
	JNIEnv *env;
	if ((*vm)->GetEnv(vm,(void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	return JNI_VERSION_1_4;
}

JNIEXPORT jstring JNICALL Java_com_example_helloapp_NativeUtils_getTitle
  (JNIEnv * env, jclass cls){

	return (*env)->NewStringUTF(env,"Native HelloApp");
}

JNIEXPORT jlong JNICALL Java_com_example_helloapp_NativeUtils_getTime
  (JNIEnv *env, jobject obj){
	return 1000;
}

int main(int argc , char *argv){
	printf("Hello, World!\n");
}
