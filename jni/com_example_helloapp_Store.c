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

JNIEXPORT jobject JNICALL Java_com_example_helloapp_Store_getColor(JNIEnv *env,
		jobject obj, jstring pkey) {
	StoreEntry *entry = findEntry(env, &gStore, pkey);
	if (isEntryValid(env, entry, StoreType_Color)) {
		return entry->mValue.mColor;
	} else {
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_com_example_helloapp_Store_setColor
(JNIEnv *env, jobject obj, jstring pkey, jobject pvalue) {
	jobject jColor=(*env)->NewGlobalRef(env,pvalue);
	if(jColor==NULL) {
		return;
	}
	StoreEntry *entry=allocateEntry(env, &gStore, pkey);
	if(entry!=NULL) {
		entry->mType=StoreType_Color;
		entry->mValue.mColor=jColor;
	} else {
		(*env)->DeleteGlobalRef(env,jColor);
	}
}

JNIEXPORT jintArray JNICALL Java_com_example_helloapp_Store_getIntegerArray(
		JNIEnv *env, jobject obj, jstring pkey) {
	StoreEntry *entry = findEntry(env, &gStore, pkey);
	if (isEntryValid(env, entry, StoreType_IntegerArray)) {
		jintArray intArray = (*env)->NewIntArray(env, entry->mLength);
		if (intArray == NULL) {
			return NULL;
		}
		(*env)->SetIntArrayRegion(env, intArray, 0, entry->mLength,
				entry->mValue.mIntegerArray);
		return intArray;
	} else {
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_com_example_helloapp_Store_setIntegerArray
(JNIEnv *env, jobject obj, jstring pkey, jintArray pvalue) {
	jsize length=(*env)->GetArrayLength(env,pvalue);
	int32_t *array=(int32_t*)malloc(sizeof(int32_t)*length);
	(*env)->GetIntArrayRegion(env,pvalue,0,length,array);
	if((*env)->ExceptionCheck(env)) {
		free(array);
		return;
	}
	StoreEntry *entry=allocateEntry(env,&gStore,pkey);
	if(entry!=NULL) {
		entry->mType=StoreType_IntegerArray;
		entry->mLength=length;
		entry->mValue.mIntegerArray=array;
	} else {
		free(array);
		return;
	}
}

JNIEXPORT jobjectArray JNICALL Java_com_example_helloapp_Store_getColorArray(
		JNIEnv *env, jobject obj, jstring pkey) {
	StoreEntry *entry = findEntry(env, &gStore, pkey);
	if (isEntryValid(env, entry, StoreType_ColorArray)) {
		jclass colorClass = (*env)->FindClass(env,
				"com/example/helloapp/Color");
		if (colorClass == NULL) {
			return NULL;
		}
		jobjectArray array = (*env)->NewObjectArray(env, entry->mLength,
				colorClass, NULL);
		(*env)->DeleteLocalRef(env, colorClass);
		if (array == NULL) {
			return NULL;
		}
		int32_t i;
		for (i = 0; i < entry->mLength; ++i) {
			(*env)->SetObjectArrayElement(env, array, i,
					entry->mValue.mColorArray[i]);
			if ((*env)->ExceptionCheck(env)) {
				return NULL;
			}
		}

		return array;
	} else {
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_com_example_helloapp_Store_setColorArray
(JNIEnv *env, jobject obj, jstring pkey, jobjectArray pvalue) {
	jsize length=(*env)->GetArrayLength(env,pvalue);
	jobject *array=(jobject*)malloc(sizeof(jobject)*length);
	int32_t i,j;
	for(i=0;i<length;++i) {
		jobject tmpColor=(*env)->GetObjectArrayElement(env,pvalue,i);
		if(tmpColor==NULL) {
			for (j = 0; j < i; ++j) {
				(*env)->DeleteGlobalRef(env,array[j]);
			}
			free(array);
			return;
		}
		array[i]=(*env)->NewGlobalRef(env,tmpColor);
		if(array[i]==NULL) {
			for (j = 0; j < i; ++j) {
				(*env)->DeleteGlobalRef(env,array[j]);
			}
			free(array);
			return;
		}
	}
	StoreEntry *entry=allocateEntry(env, &gStore, pkey);
	if(entry!=NULL) {
		entry->mType=StoreType_ColorArray;
		entry->mLength=length;
		entry->mValue.mColorArray=array;
	} else {
		for (j = 0; j < i; ++j) {
			(*env)->DeleteGlobalRef(env,array[j]);
		}
		free(array);
		return;
	}
}

