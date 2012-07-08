LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := store

LOCAL_SRC_FILES := com_example_helloapp_Store.c store.c store_watcher.c

#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := hello

LOCAL_SRC_FILES := hello.c

#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
