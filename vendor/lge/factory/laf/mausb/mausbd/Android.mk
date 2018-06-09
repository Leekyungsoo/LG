LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CFLAGS := -std=c99
LOCAL_SRC_FILES := \
    mausbd.c \
    mausb_network.c \
    mausb_common.c \
    names.c \
    mausb_host_driver.c
LOCAL_MODULE := mausbd
include $(BUILD_EXECUTABLE)
