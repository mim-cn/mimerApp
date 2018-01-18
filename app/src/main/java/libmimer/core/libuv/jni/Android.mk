#获取当前目录
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := uv
#指定平台
#LOCAL_ARM_MODE := arm
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared  -Wl -ldl
#LOCAL_LDFLAGS  += -L ../../lib/ellog/$(TARGET_ARCH_ABI)/libellog.so
$(warning "LOCAL_LDFLAGS" $(LOCAL_LDFLAGS))
LOCAL_LDLIBS += -latomic
LOCAL_CPPFLAGS += -fexceptions -frtti
#-L$(SYSROOT)/usr/lib -lrt -luuid -pthread
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)/..
LOCAL_C_INCLUDES := $(TOP_INCLUDE)/include \
	$(TOP_INCLUDE)/src 

$(warning "include" $(TOP_INCLUDE) $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/..
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/src/fs-poll.c \
	$(TOP_SRC)/src/heap-inl.h              \
	$(TOP_SRC)/src/inet.c                  \
	$(TOP_SRC)/src/queue.h                 \
	$(TOP_SRC)/src/threadpool.c            \
	$(TOP_SRC)/src/uv-common.c             \
	$(TOP_SRC)/src/uv-common.h             \
	$(TOP_SRC)/src/version.c               \
	$(TOP_SRC)/src/unix/loop.c        \
	$(TOP_SRC)/src/unix/pipe.c        \
	$(TOP_SRC)/src/unix/poll.c        \
	$(TOP_SRC)/src/unix/timer.c        \
	$(TOP_SRC)/src/unix/fs.c        \
	$(TOP_SRC)/src/unix/thread.c        \
	$(TOP_SRC)/src/unix/async.c        \
	$(TOP_SRC)/src/unix/tcp.c        \
	$(TOP_SRC)/src/unix/udp.c        \
	$(TOP_SRC)/src/unix/core.c        \
	$(TOP_SRC)/src/unix/stream.c        \
	$(TOP_SRC)/src/unix/signal.c        \
	$(TOP_SRC)/src/unix/process.c        \
	$(TOP_SRC)/src/unix/internal.h        \
	$(TOP_SRC)/src/unix/loop-watcher.c       \
	$(TOP_SRC)/src/unix/getaddrinfo.c       \
	$(TOP_SRC)/src/unix/proctitle.c        \
	$(TOP_SRC)/src/unix/linux-core.c       \
	$(TOP_SRC)/src/unix/linux-inotify.c    \
	$(TOP_SRC)/src/unix/linux-syscalls.c   \
	$(TOP_SRC)/src/unix/linux-syscalls.h   \
	$(TOP_SRC)/src/unix/pthread-fixes.c    \
	$(TOP_SRC)/src/unix/android-ifaddrs.c  \

$(warning "src" $(LOCAL_SRC_FILES))
#生成共享库
include $(BUILD_SHARED_LIBRARY)

