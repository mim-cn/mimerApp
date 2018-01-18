#获取当前目录
LOCAL_PATH := $(call my-dir)
#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := ellog
#链接的第三方库
ifdef ($(LIBROOT))
LOCAL_SRC_FILES := $(LIBROOT)/$(TARGET_ARCH_ABI)/libellog.so
else
LOCAL_SRC_FILES := $(LOCAL_PATH)/../lib/android/$(TARGET_ARCH_ABI)/libellog.so
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..
include $(PREBUILT_SHARED_LIBRARY)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := transmitter
#链接的第三方库
ifdef ($(LIBROOT))
LOCAL_SRC_FILES := $(LIBROOT)/$(TARGET_ARCH_ABI)/libtransmitter.so
else
LOCAL_SRC_FILES := $(LOCAL_PATH)/../lib/android/$(TARGET_ARCH_ABI)/libtransmitter.so
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include
include $(PREBUILT_SHARED_LIBRARY)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := uv
#链接的第三方库
ifdef ($(LIBROOT))
LOCAL_SRC_FILES := $(LIBROOT)/$(TARGET_ARCH_ABI)/libuv.so
else
LOCAL_SRC_FILES := $(LOCAL_PATH)/../lib/android/$(TARGET_ARCH_ABI)/libuv.so
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include
include $(PREBUILT_SHARED_LIBRARY)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := uvbase
#链接的第三方库
ifdef ($(LIBROOT))
LOCAL_SRC_FILES := $(LIBROOT)/$(TARGET_ARCH_ABI)/libuvbase.so
else
LOCAL_SRC_FILES := $(LOCAL_PATH)/../lib/android/$(TARGET_ARCH_ABI)/libuvbase.so
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..
include $(PREBUILT_SHARED_LIBRARY)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := mimp
#链接的第三方库
ifdef ($(LIBROOT))
LOCAL_SRC_FILES := $(LIBROOT)/$(TARGET_ARCH_ABI)/libmimp.so
else
LOCAL_SRC_FILES := $(LOCAL_PATH)/../lib/android/$(TARGET_ARCH_ABI)/libmimp.so
endif
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..
include $(PREBUILT_SHARED_LIBRARY)


#清除一些变量
include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := libmimer
#指定平台
#LOCAL_ARM_MODE := arm
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared -ldl
LOCAL_LDLIBS += -latomic
LOCAL_CPPFLAGS += -fexceptions -frtti
#-L$(SYSROOT)/usr/lib -lrt -luuid -pthread
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)
LOCAL_C_INCLUDES := $(TOP_INCLUDE)../ \
	$(TOP_INCLUDE)/../transmitter       \
	$(TOP_INCLUDE)/../uvbase            \
	$(TOP_INCLUDE)/../libuv/include \
	$(TOP_INCLUDE)/../ellog      \
	$(TOP_INCLUDE)/../MIMProtocol
$(warning "include"  $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/../
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/client.cpp \
	$(TOP_SRC)/core/transmitter/transmitter.cpp
#链接的第三方库
LOCAL_SHARED_LIBRARIES  := ellog
LOCAL_SHARED_LIBRARIES  += uvbase
LOCAL_SHARED_LIBRARIES  += mimp
LOCAL_SHARED_LIBRARIES  += uv
LOCAL_SHARED_LIBRARIES  += transmitter

#生成共享库
include $(BUILD_SHARED_LIBRARY)
