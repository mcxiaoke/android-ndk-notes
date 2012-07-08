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
	pthread_t mThread;
	int32_t mState;
} StoreWatcher;

void startWatcher(JNIEnv *env, StoreWatcher *watcher, Store *store,
		jobject storeFront);
void stopWatcher(JNIEnv *env, StoreWatcher *watcher);

void deleteGlobalRef(JNIEnv *env, jobject *ref);

JNIEnv *getJNIEnv(JavaVM *vm);

void *runWatcher(void* args);

void processEntry(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry);

#endif
