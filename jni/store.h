#ifndef _STORE_H_
#define _STORE_H_

#include <stdio.h>
#include <stdint.h>
#include "jni.h"

#define STORE_MAX_CAPACITY 16
#define TRUE 1
#define FALSE 0

typedef enum {
	StoreType_Integer, StoreType_String, StoreType_Color,
	StoreType_IntegerArray, StoreType_ColorArray
} StoreType;

typedef union {

	int32_t mInteger;
	char *mString;
	jobject mColor;
	int32_t *mIntegerArray;
	jobject *mColorArray;
} StoreValue;

typedef struct {
	char *mKey;
	StoreType mType;
	StoreValue mValue;
	int32_t mLength;
} StoreEntry;

typedef struct {
	StoreEntry mEntries[STORE_MAX_CAPACITY];
	int32_t mLength;
} Store;

int32_t isEntryValid(JNIEnv *env, StoreEntry *entry, StoreType type);
StoreEntry *allocateEntry(JNIEnv *env, Store *store, jstring key);
StoreEntry *findEntry(JNIEnv *env, Store *store, jstring key);
void releaseEntryValue(JNIEnv *env, StoreEntry *entry);

void throwInvalidTypeException(JNIEnv* env);
void throwNotExistingKeyException(JNIEnv* env);
void throwStoreFullException(JNIEnv* env);

#endif
