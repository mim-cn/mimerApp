#!/bin/bash

#检测NDKROOT
if [[ -z $NDKROOT ]]; then
	echo -e "\033[31mWarning: \$NDKROOT is EMPTY!!! YOU ARE NOT SET NDK-ROOT PATH\033[0m"
	exit
fi

#检测参数
if [[ $# < 1 ]]; then
	echo "$0 module(default:all) build-option(eg:-B V=1)"
	echo "module: "
	echo "1.  mimp"
	echo "2.  uv"
	echo "3.  uvbase"
	echo "5.  transmitter"
	echo "0.  all"
	exit
fi


TOPAPTH=`pwd`
ROOTPATH=$TOPAPTH/core
NDK_BUILD=$NDKROOT/ndk-build
REBUILD=$2
case $1 in
	1 | mimp)
		cd $ROOTPATH/MIMProtocol/jni && $NDK_BUILD $REBUILD
		echo "build mimp Success"
		;;
	2 | uv) 
		cd $ROOTPATH/libuv/jni && $NDK_BUILD $REBUILD
		echo "build uv Success"
		;;
	3 | uvbase)
		cd $ROOTPATH/uvbase/jni && $NDK_BUILD $REBUILD
		echo "build uvbase Success"
		;;
	4 | transmitter)
		cd $ROOTPATH/transmitter/jni && $NDK_BUILD $REBUILD
		echo "build transmitter Success"
		;;
	0 | all)
		cd $ROOTPATH/libuv/jni && $NDK_BUILD $REBUILD
		cd $ROOTPATH/MIMProtocol/jni && $NDK_BUILD $REBUILD
		cd $ROOTPATH/uvbase/jni && $NDK_BUILD $REBUILD
		cd $ROOTPATH/transmitter/jni && $NDK_BUILD $REBUILD
		echo "build all Success"
		;;
	*)
		echo "$0 module(default:all) build-option(eg:-B V=1)"
		echo "module: "
		echo "1.  mimp"
		echo "2.  uv"
		echo "3.  uvbase"
		echo "4.  transmitter"
		echo "0.  all"
		exit
		;;
esac
