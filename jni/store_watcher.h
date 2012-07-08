#ifndef _STOREWATCHER_H_
#define _STOREWATCHER_H_

#include "store.h"
#include <jni.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define SLEEP_DURATION 5
#define STATE_OK 0
#define STATE_KO 1

typedef struct {
	Store *mStore;
	JavaVM *mJavaVM;
	jobject mStoreFront;
	jobject mColor;
	// classes
	jclass ClassStore;
	jclass ClassColor;
	//methods
	jmethodID MethodOnAlertInt;
	jmethodID MethodOnAlertString;
	jmethodID MethodOnAlertColor;
	jmethodID MethodColorEquals;
	// threads variables
	pthread_t mThread;
	int32_t mState;
} StoreWatcher;

void startWatcher(JNIEnv *env, StoreWatcher *watcher, Store *store,
		jobject storeFront);
void stopWatcher(JNIEnv *env, StoreWatcher *watcher);

void makeGlobalRef(JNIEnv *env, jobject *obj);
void deleteGlobalRef(JNIEnv *env, jobject *obj);
JNIEnv *getJNIEnv(JavaVM *vm);

void *runWatcher(void* args);

void processEntry(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry);
void processEntryInt(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry);
void processEntryString(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry);
void processEntryColor(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry);

#endif
