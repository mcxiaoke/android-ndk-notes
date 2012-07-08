#include "store_watcher.h"

void startWatcher(JNIEnv *env, StoreWatcher *watcher, Store *store,
		jobject storeFront) {
	DLOG("start watcher.");
	memset(watcher, 0, sizeof(StoreWatcher));
	watcher->mState = STATE_OK;
	watcher->mStore = store;
	if ((*env)->GetJavaVM(env, &watcher->mJavaVM) != JNI_OK) {
		goto ERROR;
		//stopWatcher(env,watcher);
		//return;
	}

	//cache classes
	watcher->ClassStore = (*env)->FindClass(env, "com/example/helloapp/Store");
	makeGlobalRef(env, &watcher->ClassStore);
	if (watcher->ClassStore == NULL) {
		goto ERROR;
	}
	watcher->ClassColor = (*env)->FindClass(env, "com/example/helloapp/Color");
	makeGlobalRef(env, &watcher->ClassColor);
	if (watcher->ClassColor == NULL) {
		goto ERROR;
	}

	//cache java methods
	watcher->MethodOnAlertInt = (*env)->GetMethodID(env, watcher->ClassStore,
			"onAlert", "(I)V");
	if (watcher->MethodOnAlertInt == NULL) {
		goto ERROR;
	}
	watcher->MethodOnAlertString = (*env)->GetMethodID(env, watcher->ClassStore,
			"onAlert", "(Ljava/lang/String;)V");
	if (watcher->MethodOnAlertString == NULL) {
		goto ERROR;
	}
	watcher->MethodOnAlertColor = (*env)->GetMethodID(env, watcher->ClassStore,
			"onAlert", "(Lcom/example/helloapp/Color;)V");
	if (watcher->MethodOnAlertColor == NULL) {
		goto ERROR;
	}
	watcher->MethodColorEquals = (*env)->GetMethodID(env, watcher->ClassColor,
			"equals", "(Ljava/lang/Object;)Z");
	if (watcher->MethodColorEquals == NULL) {
		goto ERROR;
	}
	jmethodID colorConstructor = (*env)->GetMethodID(env, watcher->ClassColor,
			"<init>", "(Ljava/lang/String;)V");
	if (colorConstructor == NULL) {
		goto ERROR;
	}
	jstring color = (*env)->NewStringUTF(env, "white");
	if (color == NULL) {
		goto ERROR;
	}
	watcher->mColor = (*env)->NewObject(env, watcher->ClassColor,
			colorConstructor, color);
	makeGlobalRef(env, &watcher->mColor);
	if (watcher->mColor == NULL) {
		goto ERROR;
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
	ERROR:
		DLOG("start watcher error.");
		stopWatcher(env, watcher);
		return;

}

void stopWatcher(JNIEnv *env, StoreWatcher *watcher) {
	DLOG("stop watcher.");
	if (watcher->mState == STATE_OK) {
		DLOG("stop watcher. state is ok.");
		(*env)->MonitorEnter(env, watcher->mStoreFront);
		watcher->mState = STATE_KO;
		(*env)->MonitorExit(env, watcher->mStoreFront);
		pthread_join(watcher->mThread, NULL);
		deleteGlobalRef(env, &watcher->mStoreFront);
		deleteGlobalRef(env, &watcher->mColor);
		deleteGlobalRef(env, &watcher->ClassStore);
		deleteGlobalRef(env, &watcher->ClassColor);
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

void makeGlobalRef(JNIEnv *env, jobject *obj) {
	if (*obj != NULL) {
		jobject globalRef = (*env)->NewGlobalRef(env, *obj);
		(*env)->DeleteLocalRef(env, *obj);
		*obj = globalRef;
	}

}

void deleteGlobalRef(JNIEnv *env, jobject *obj) {
	if (*obj != NULL) {
		(*env)->DeleteGlobalRef(env, obj);
		*obj = NULL;
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
	int32_t count=0;
	while (isRunning) {
		sleep(SLEEP_DURATION);
		StoreEntry *entry = store->mEntries;
		int32_t isScanning = 1;
		while (isScanning) {
			count++;
			(*env)->MonitorEnter(env, watcher->mStoreFront);
			isRunning = (watcher->mState == STATE_OK);
			StoreEntry *entryEnd = store->mEntries + store->mLength;
			isScanning = (entry < entryEnd);
			DLOG("run watcher is scanning store length:%d",store->mLength);
			DLOG("run watcher is scanning start:%p,end:%p",entry,entryEnd);
			DLOG("run watcher is scanning running=%d,scanning=%d",isRunning,isScanning);
			if (isRunning && isScanning) {
				DLOG("run watcher process entry.");
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
	switch (entry->mType) {
	case StoreType_Integer:
		processEntryInt(env, watcher, entry);
		break;
	case StoreType_String:
		processEntryString(env, watcher, entry);
		break;
	case StoreType_Color:
		processEntryColor(env, watcher, entry);
		break;
	default:
		break;
	}
}

void processEntryInt(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry) {
	DLOG("processEntryInt.");
	if (strcmp(entry->mKey, "watcherCount") == 0) {
		++entry->mValue.mInteger;
	} else if (entry->mValue.mInteger > 1000
			|| entry->mValue.mInteger < -1000) {
		DLOG("processEntryInt. value need process.");
		(*env)->CallVoidMethod(env, watcher->mStoreFront,
				watcher->MethodOnAlertInt, (jint) entry->mValue.mInteger);
	}
}
void processEntryString(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry) {
	DLOG("processEntryString.");
	if (strcmp(entry->mValue.mString, "apple")) {
		jstring str = (*env)->NewStringUTF(env, entry->mValue.mString);
		(*env)->CallVoidMethod(env, watcher->mStoreFront,
				watcher->MethodOnAlertString, str);
		(*env)->DeleteLocalRef(env, str);
	}
}
void processEntryColor(JNIEnv *env, StoreWatcher *watcher, StoreEntry *entry) {
	DLOG("processEntryColor.");
	jboolean result = (*env)->CallBooleanMethod(env, watcher->mColor,
			watcher->MethodColorEquals, entry->mValue.mColor);
	if (result) {
		(*env)->CallVoidMethod(env, watcher->mStoreFront,
				watcher->MethodOnAlertColor, entry->mValue.mColor);
	}
}

