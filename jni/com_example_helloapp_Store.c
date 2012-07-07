#include "com_example_helloapp_Store.h"
#include "store.h"
#include <stdint.h>
#include <string.h>

static Store gStore = { { }, 0 };

JNIEXPORT jint JNICALL Java_com_example_helloapp_Store_getInteger(JNIEnv *env,
		jobject obj, jstring key) {
	StoreEntry *entry = findEntry(env, &gStore, key);
	if (isEntryValid(env, entry, StoreType_Integer)) {
		return entry->mValue.mInteger;
	} else {
		return 0.0f;
	}
}

JNIEXPORT void JNICALL Java_com_example_helloapp_Store_setInteger
(JNIEnv *env, jobject obj, jstring key, jint value) {
	StoreEntry *entry=allocateEntry(env,&gStore, key);
	if(entry!=NULL) {
		entry->mType=StoreType_Integer;
		entry->mValue.mInteger=value;
	}
}

JNIEXPORT jstring JNICALL Java_com_example_helloapp_Store_getString(JNIEnv *env,
		jobject obj, jstring key) {
	StoreEntry *entry = findEntry(env, &gStore, key);
	if (isEntryValid(env, entry, StoreType_String)) {
		return (*env)->NewStringUTF(env, entry->mValue.mString);
	} else {
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_com_example_helloapp_Store_setString
(JNIEnv *env, jobject obj, jstring pkey, jstring pvalue) {
	const char *tmpValue=(*env)->GetStringUTFChars(env,pvalue,NULL);
	if(tmpValue==NULL) {
		return;
	}
	StoreEntry *entry=allocateEntry(env,&gStore,pkey);
	if(entry!=NULL) {
		entry->mType=StoreType_String;
		jsize valueLength=(*env)->GetStringUTFLength(env,pvalue);
		entry->mValue.mString=(char*)malloc(sizeof(char)*(valueLength+1));
		strcpy(entry->mValue.mString, tmpValue);
	}
}

JNIEXPORT jobject JNICALL Java_com_example_helloapp_Store_getColor
  (JNIEnv *env, jobject obj, jstring key){
	StoreEntry *entry=findEntry(env,&gStore,key);
	if(isEntryValid(env,entry,StoreType_Color)){
		return entry->mValue.mColor;
	}else{
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_com_example_helloapp_Store_setColor
  (JNIEnv *env, jobject obj, jstring pkey, jobject pvalue){
	jobject jColor=(*env)->NewGlobalRef(env,pvalue);
	if(jColor==NULL){
		return;
	}
	StoreEntry *entry=allocateEntry(env, &gStore, pkey);
	if(entry!=NULL){
		entry->mType=StoreType_Color;
		entry->mValue.mColor=jColor;
	}else{
		(*env)->DeleteGlobalRef(env,jColor);
	}
}
