LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CFLAGS := -Os -Wall
LOCAL_SRC_FILES := mausb.c utils.c mausb_bind.c mausb_unbind.c mausb_common.c
LOCAL_MODULE := mausb
include $(BUILD_EXECUTABLE)
