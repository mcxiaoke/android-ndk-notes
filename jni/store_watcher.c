#include "store_watcher.h"

void startWatcher(JNIEnv *env, StoreWatcher *watcher, Store *store,
		jobject storeFront) {
	memset(watcher, 0, sizeof(StoreWatcher));
	watcher->mState = STATE_OK;
	watcher->mStore = store;
	if ((*env)->GetJavaVM(env, &watcher->mJavaVM) != JNI_OK) {
		goto ERROR;
		//stopWatcher(env,watcher);
		//return;
	}
	watcher->mStoreFront = (*env)->NewGlobalRef(env, storeFront);
	if (watcher->mStoreFront == NULL) {
		goto ERROR;
	}
	pthread_attr_t attrs;
	int error = pthread_attr_init(&attrs);
	if (error) {
		goto ERROR;
	}
	error = pthread_create(&watcher->mThread, &attrs, runWatcher, watcher);
	if (error) {
		goto ERROR;
	}
	return;
	ERROR: stopWatcher(env, watcher);
	return;

}

void stopWatcher(JNIEnv *env, StoreWatcher *watcher) {
	if (watcher->mState == STATE_OK) {
		(*env)->MonitorEnter(env, watcher->mStoreFront);
		watcher->mState = STATE_KO;
		(*env)->MonitorExit(env, watcher->mStoreFront);
		pthread_join(watcher->mThread, NULL);
		deleteGlobalRef(env, &watcher->mStoreFront);
	}
}

JNIEnv *getJNIEnv(JavaVM *vm) {
	JavaVMAttachArgs vmArgs;
	vmArgs.version = JNI_VERSION_1_6;
	vmArgs.name = "NativeThread";
	vmArgs.group = NULL;

	JNIEnv *env;
	if ((*vm)->AttachCurrentThread(vm, &env, &vmArgs) != JNI_OK) {
		env = NULL;
	}
	return env;
}

void deleteGlobalRef(JNIEnv *env, jobject *ref) {
	if (*ref != NULL) {
		(*env)->DeleteGlobalRef(env, ref);
		*ref = NULL;
	}
}

void *runWatcher(void* args) {
	StoreWatcher *watcher = (StoreWatcher*) args;
	Store *store = watcher->mStore;
	JavaVM *vm = watcher->mJavaVM;
	JNIEnv *env = getJNIEnv(vm);
	if (env == NULL) {
		goto ERROR;
	}
	int32_t isRunning = 1;
	while (isRunning) {
		sleep(SLEEP_DURATION);
		StoreEntry *entry = watcher->mStore->mEntries;
		int32_t isScanning = 1;
		while (isScanning) {
			(*env)->MonitorEnter(env, watcher->mStoreFront);
			isRunning = (watcher->mState == STATE_OK);
			StoreEntry *entryEnd = watcher->mStore->mEntries
					+ watcher->mStore->mLength;
			isScanning = (entry < entryEnd);
			if (isRunning && isScanning) {
				processEntry(env, watcher, entry);
			}
			(*env)->MonitorExit(env, watcher->mStoreFront);
			++entry;
		}
	}
	ERROR: (*vm)->DetachCurrentThread(vm);
	pthread_exit(NULL);
}

void processEntry(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry) {
	if (entry->mType == StoreType_Integer
			&& strcmp(entry->mKey, "watcherCount") == 0) {
		++entry->mValue.mInteger;
	}
}

