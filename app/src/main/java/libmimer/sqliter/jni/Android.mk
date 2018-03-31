#获取当前目录
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := libsqlite
#指定平台
#LOCAL_ARM_MODE := arm
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared -ldl -llog
$(warning "LOCAL_LDFLAGS" $(LOCAL_LDFLAGS))
LOCAL_LDLIBS += -latomic
LOCAL_CPPFLAGS += -fexceptions -frtti
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)/..
LOCAL_C_INCLUDES := $(TOP_INCLUDE)/       \
    $(TOP_INCLUDE)/include                \
	$(TOP_INCLUDE)/../core                \
	$(TOP_INCLUDE)/sqlite3                \
	$(TOP_INCLUDE)/../utils

$(warning "include" $(TOP_INCLUDE) $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/..
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/sqliter.cpp \
    $(TOP_SRC)/sqlite3/sqlite3.c          \
	$(TOP_SRC)/src/Backup.cpp             \
	$(TOP_SRC)/src/Column.cpp             \
	$(TOP_SRC)/src/Database.cpp           \
	$(TOP_SRC)/src/Exception.cpp          \
	$(TOP_SRC)/src/Statement.cpp          \
	$(TOP_SRC)/src/Transaction.cpp        \
	$(TOP_SRC)/../utils/ndkUtils.cpp
	
$(warning "src" $(LOCAL_SRC_FILES))
#生成共享库
include $(BUILD_SHARED_LIBRARY)
