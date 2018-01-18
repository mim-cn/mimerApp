#获取当前目录
LOCAL_PATH := $(call my-dir)

#清除一些变量
include $(CLEAR_VARS)
LOCAL_MODULE := ellog
#链接的第三方库
#链接的第三方库
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../ellog/libs/$(TARGET_ARCH_ABI)/libellog.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := libmimp
#指定平台
#LOCAL_ARM_MODE := arm
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared -ldl
$(warning "LOCAL_LDFLAGS" $(LOCAL_LDFLAGS))
LOCAL_LDLIBS += -latomic
LOCAL_CPPFLAGS += -fexceptions -frtti
#-L$(SYSROOT)/usr/lib -lrt -luuid -pthread
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)/..
LOCAL_C_INCLUDES := $(TOP_INCLUDE) \
	$(TOP_INCLUDE)/../ellog          \
	$(TOP_INCLUDE)/../../

$(warning "include" $(TOP_INCLUDE) $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/..
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/MIMInt.cpp \
	$(TOP_SRC)/MIMPacket.cpp             \
	$(TOP_SRC)/MIMProtocol.cpp           \
	$(TOP_SRC)/memUtil.c                 \
	$(TOP_SRC)/timeUtil.c                \
	$(TOP_SRC)/../Stream.cpp

$(warning "src" $(LOCAL_SRC_FILES))
#链接的第三方库
LOCAL_SHARED_LIBRARIES  := ellog
#生成共享库
include $(BUILD_SHARED_LIBRARY)
