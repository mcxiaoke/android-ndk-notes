LOCAL_PATH := $(call my-dir)

#
# Transcode AVILib
#

# Source files
MY_AVILIB_SRC_FILES := avilib.c platform_posix.c

# Include path to export
MY_AVILIB_C_INCLUDES := $(LOCAL_PATH)

#
# AVILib static
#
include $(CLEAR_VARS)

# Module name
LOCAL_MODULE := avilib_static

# Source files
LOCAL_SRC_FILES := $(MY_AVILIB_SRC_FILES)

# Include path to export
LOCAL_EXPORT_C_INCLUDES := $(MY_AVILIB_C_INCLUDES)

# Build a static library
include $(BUILD_STATIC_LIBRARY)

#
# AVILib shared
#
include $(CLEAR_VARS)

# Module name
LOCAL_MODULE := avilib_shared

# Source files
LOCAL_SRC_FILES := $(MY_AVILIB_SRC_FILES)

# Include path to export
LOCAL_EXPORT_C_INCLUDES := $(MY_AVILIB_C_INCLUDES)

# Build a shared library
include $(BUILD_SHARED_LIBRARY)

