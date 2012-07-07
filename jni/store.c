#include "store.h"
#include <string.h>

int32_t isEntryValid(JNIEnv *env, StoreEntry *entry, StoreType type) {
	if (entry == NULL) {
		throwNotExistingKeyException(env);
	} else if (entry->mType != type) {
		throwInvalidTypeException(env);
	} else {
		return TRUE;
	}
	return FALSE;
}

StoreEntry *allocateEntry(JNIEnv *env, Store *store, jstring key) {
	//int32_t error = 0;
	StoreEntry *entry = findEntry(env, store, key);
	if (entry != NULL) {
		releaseEntryValue(env, entry);
	} else {
		if (store->mLength >= STORE_MAX_CAPACITY) {
			throwStoreFullException(env);
			return NULL;
		}
		entry = store->mEntries + store->mLength;
		const char *tmpKey = (*env)->GetStringUTFChars(env, key, NULL);
		if (tmpKey == NULL) {
			return NULL;
		}
		entry->mKey = (char*) malloc(strlen(tmpKey));
		strcpy(entry->mKey, tmpKey);
		(*env)->ReleaseStringUTFChars(env, key, tmpKey);
		++store->mLength;
	}
	return entry;
}

StoreEntry *findEntry(JNIEnv *env, Store *store, jstring key) {
	StoreEntry *start = store->mEntries;
	StoreEntry *end = start + store->mLength;

	const char *tmpKey = (*env)->GetStringUTFChars(env, key, NULL);
	if (tmpKey == NULL) {
//		if (error != NULL) {
//			*error = 1;
//		}
		return NULL;
	}
	while ((start < end) && (strcmp(start->mKey, tmpKey) != 0)) {
		++start;
	}
	(*env)->ReleaseStringUTFChars(env, key, tmpKey);
	return (start == end) ? NULL : start;
}

void releaseEntryValue(JNIEnv *env, StoreEntry *entry) {
	int32_t i;
	switch (entry->mType) {
	case StoreType_String:
		free(entry->mValue.mString);
		break;
	case StoreType_Color:
		(*env)->DeleteGlobalRef(env, entry->mValue.mColor);
		break;
	case StoreType_IntegerArray:
		free(entry->mValue.mIntegerArray);
		break;
	case StoreType_ColorArray:
		for (i = 0; i < entry->mLength; ++i) {
			(*env)->DeleteGlobalRef(env, entry->mValue.mColorArray[i]);
		}
		free(entry->mValue.mColorArray);
		break;
	}
}

void throwInvalidTypeException(JNIEnv* env) {
	jclass cls = (*env)->FindClass(env,
			"com/example/helloapp/InvalidTypeException");
	if (cls != NULL) {
		(*env)->ThrowNew(env, cls, "invalid type.");
	}
	(*env)->DeleteLocalRef(env, cls);
}

void throwNotExistingKeyException(JNIEnv* env) {
	jclass cls = (*env)->FindClass(env,
			"com/example/helloapp/NotExistingKeyException");
	if (cls != NULL) {
		(*env)->ThrowNew(env, cls, "Key does not exists.");
	}
	(*env)->DeleteLocalRef(env, cls);
}

void throwStoreFullException(JNIEnv* env) {
	jclass cls = (*env)->FindClass(env,
			"com/example/helloapp/StoreFullException");
	if (cls != NULL) {
		(*env)->ThrowNew(env, cls, "storage is full.");
	}
	(*env)->DeleteLocalRef(env, cls);
}
