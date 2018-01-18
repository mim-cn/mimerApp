#获取当前目录
LOCAL_PATH := $(call my-dir)
#清除一些变量
include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := libellog
#指定平台
#LOCAL_ARM_MODE := arm
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared -ldl
LOCAL_CPPFLAGS += -fexceptions -frtti
LOCAL_LDLIBS += -latomic
#-L$(SYSROOT)/usr/lib -lrt -luuid -pthread
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)
LOCAL_C_INCLUDES := $(TOP_INCLUDE)/../ \
	$(TOP_INCLUDE)/../../
$(warning "include"  $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/..
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/ellog.cpp
LOCAL_SRC_FILES += $(TOP_SRC)/../Stream.cpp
$(warning "source" $(LOCAL_SRC_FILES))

#生成共享库
include $(BUILD_SHARED_LIBRARY)
