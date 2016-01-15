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

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))
LOCAL_MODULE    := native
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_LDLIBS := -landroid -llog
LOCAL_STATIC_LIBRARIES := android_native_app_glue

### LOG START
MY_LOG_TAG := \"NativeApp\"
# ndk-build NDK_DEBUG=1
ifeq ($(APP_OPTIM),release)
  MY_LOG_LEVEL := NLOG_LEVEL_ERROR
else
  MY_LOG_LEVEL := NLOG_LEVEL_VERBOSE
endif
LOCAL_CFLAGS += -DNLOG_LEVEL=$(MY_LOG_LEVEL)
LOCAL_CFLAGS += -DNLOG_TAG=$(MY_LOG_TAG)
### LOG END

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
