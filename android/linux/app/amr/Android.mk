LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= \
		frameworks/av/include/media

LOCAL_SRC_FILES:= \
    recorder.cpp \
	player.cpp

LOCAL_SHARED_LIBRARIES:= \
    libcutils \
    libutils \
	libmedia \
	libbinder

LOCAL_CFLAGS += -Wno-multichar
LOCAL_MODULE_TAGS:= optional
LOCAL_MODULE:= recorder

include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)
#include $(call all-makefiles-under,$(LOCAL_PATH))
