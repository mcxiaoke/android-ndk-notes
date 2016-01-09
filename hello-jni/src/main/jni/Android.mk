# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := hello-jni.c

#LOCAL_MODULE_FILENAME = jni-hello
#LOCAL_CPP_EXTENSION := .cpp .cxx .cc
#LOCAL_CPP_FEATURES := rtti
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
#LOCAL_CFLAGS := -DPORT=9999 -DNDEBUG
#LOCAL_CPPFLAGS := -DMAX=100
#LOCAL_LDFLAGS := -llog
#LOCAL_ARM_NENO := true
#LOCAL_DISABLE_NO_EXECUTE := true
#LOCAL_EXPORT_CFLAGS := − DENABLE_AUDIO
#LOCAL_EXPORT_CPPFLAGS
#LOCAL_EXPORT_LDFLAGS
#LOCAL_EXPORT_C_INCLUDES
include $(BUILD_SHARED_LIBRARY)

# include $(CLEAR_VARS)

# LOCAL_MODULE    := hello-jni-static
# LOCAL_SRC_FILES := hello-jni.c

# include $(BUILD_STATIC_LIBRARY)

#include $(CLEAR_VARS)

#LOCAL_MODULE    := hello
#LOCAL_SRC_FILES := hello.c
#LOCAL_STATIC_LIBS := hello-jni

#include $(BUILD_EXECUTABLE)

# build system defined
#TARGET_ARCH: Name of the target CPU architecture, such as arm.
#TARGET_PLATFORM: Name of the target Android platform, such as android-3
#TARGET_ARCH_ABI: Name of the target CPU architecture and the ABI, such as
#armeabi-v7a.
#TARGET_ABI: Concatenation of target platform and ABI, such as android-3-
#armeabi-v7a

#build system marcros
#include $(call all-subdir-makefiles)
#all-subdir-makefiles - makefiles in sub-directories
#this-makefile - current makefile
#parent-makefile - path of parent makefile

# custom variables
#MY_SRC_FILES := avilib.c platform_posix.c
#LOCAL_SRC_FILES := $(addprefix avilib/, $(MY_SRC_FILES))

# conditional operations
# ifeq ($(TARGET_ARCH),arm)
# LOCAL_SRC_FILES + = armonly.c
# else
# LOCAL_SRC_FILES + = generic.c
# endif
