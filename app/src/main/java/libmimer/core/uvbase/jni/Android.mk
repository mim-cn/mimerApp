#获取当前目录
LOCAL_PATH := $(call my-dir)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := ellog
#链接的第三方库
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../ellog/libs/$(TARGET_ARCH_ABI)/libellog.so
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
LOCAL_MODULE    := libuvbase
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
LOCAL_C_INCLUDES := $(TOP_INCLUDE)/../ \
	$(TOP_INCLUDE)/../../libuv/include/  \
	$(TOP_INCLUDE)/../../ellog
$(warning "include"  $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/../
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/biter.cpp             \
	$(TOP_SRC)/mmloop.cpp            \
	$(TOP_SRC)/mmloop_files.cpp      \
	$(TOP_SRC)/mmloop_lock.cpp       \
	$(TOP_SRC)/mmloop_MT.cpp         \
	$(TOP_SRC)/mmloop_net.cpp        \
	$(TOP_SRC)/mmloop_utils.cpp      \
$(warning "source"  $(LOCAL_SRC_FILES))	
#链接的第三方库
LOCAL_SHARED_LIBRARIES  := ellog
LOCAL_SHARED_LIBRARIES  += uv
#生成共享库
include $(BUILD_SHARED_LIBRARY)
