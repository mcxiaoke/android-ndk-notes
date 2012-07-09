LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := livecamera
LOCAL_SRC_FILES := com_demo_livecamera_CameraView.c
LOCAL_LDLIBS += -ljnigraphics -llog

include $(BUILD_SHARED_LIBRARY)
