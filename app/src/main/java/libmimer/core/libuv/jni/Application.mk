#APP_STL := stlport_static
APP_CPPFLAGS += -std=c++11 # APP_CPPFLAGS += -std=gnu++11
APP_STL := gnustl_static
#APP_STL := stlport_static
STLPORT_FORCE_REBUILD := true
#APP_ABI := all
APP_PLATFORM := android-19
APP_ABI := armeabi armeabi-v7a
