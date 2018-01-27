#获取当前目录
LOCAL_PATH := $(call my-dir)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := uvbase
#链接的第三方库
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../uvbase/libs/$(TARGET_ARCH_ABI)/libuvbase.so
include $(PREBUILT_SHARED_LIBRARY)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := mimp
#链接的第三方库
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../MIMProtocol/libs/$(TARGET_ARCH_ABI)/libmimp.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..
include $(PREBUILT_SHARED_LIBRARY)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := uv
#链接的第三方库
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../libuv/libs/$(TARGET_ARCH_ABI)/libuv.so
include $(PREBUILT_SHARED_LIBRARY)

#清除一些变量
include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := libtransmitter
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared -ldl -llog
LOCAL_LDLIBS += -latomic
LOCAL_CPPFLAGS += -fexceptions -frtti
#-L$(SYSROOT)/usr/lib -lrt -luuid -pthread
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)/
LOCAL_C_INCLUDES := $(TOP_INCLUDE)../  \
	$(TOP_INCLUDE)../../               \
	$(TOP_INCLUDE)/../../uvbase        \
	$(TOP_INCLUDE)/../../libuv/include \
	$(TOP_INCLUDE)/../../MIMProtocol
$(warning "include"  $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/../
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/transmitter.cpp
LOCAL_SHARED_LIBRARIES  += uvbase
LOCAL_SHARED_LIBRARIES  += mimp
LOCAL_SHARED_LIBRARIES  += uv
#生成共享库
include $(BUILD_SHARED_LIBRARY)
