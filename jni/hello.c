#include "hello.h"

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
