@echo off
if /i "%1"=="" goto :default
if /i %NDKROOT% == "" goto :ndk_error
if /i "%2" == "clean" goto :clean

:: 获取当期路径
set TOPPATH=%cd%
:: 获取依赖库根路径
set COREPATH=%TOPPATH%\\core
:: 获取ndk-build路径
set NDK_BUILD=%NDKROOT%\\ndk-build
:: 获取ndk-build编译参数
set REBUILD=%2

if /i "%1"=="0" goto :all
if /i "%1"=="1" goto :mimp
if /i "%1"=="2" goto :uv
if /i "%1"=="3" goto :uvbase
if /i "%1"=="4" goto :mimcore
if /i "%1"=="5" goto :sqliter

:: 编译mimp
:mimp
cd %COREPATH%\\MIMProtocol\\jni && %NDK_BUILD% && cd %TOPPATH%
echo "build mimp Success"
goto :eof

:: 编译uv
:uv
cd %COREPATH%\\libuv\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH%
echo "build uv Success"
goto :eof

:: 编译uvbase
:uvbase
cd %COREPATH%\\uvbase\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH%
echo "build uvbase Success"
goto :eof

:: 编译mimcore
:mimcore
cd %COREPATH%\\libuv\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% &&       ^
cd %COREPATH%\\MIMProtocol\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% && ^
cd %COREPATH%\\uvbase\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% &&      ^
cd %TOPPATH%\\mimcore\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% &&               ^
echo "build mimcore Success"
goto :eof

:: 编译sqliter
:sqliter
cd %TOPPATH%\\sqliter\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH%
echo "build sqliter Success"
goto :eof

:: 编译所有
:all
cd %COREPATH%\\libuv\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% &&       ^
cd %COREPATH%\\MIMProtocol\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% && ^
cd %COREPATH%\\uvbase\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% &&      ^
cd %TOPPATH%\\sqliter\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% &&      ^
cd %TOPPATH%\\mimcore\\jni && %NDK_BUILD% %REBUILD% && cd %TOPPATH% &&      ^
echo "build all Success"
goto :eof

:: 默认错误提示
:default
echo "%0 module(default:all) build-option(eg:-B V=1)"
echo "module: "
echo "0.  all"
echo "1.  mimp"
echo "2.  uv"
echo "3.  uvbase"
echo "4.  mimcore"
echo "5.  sqliter"
echo "NOTE: id build failed, Please Goto path %TOPPATH%"
goto :eof

:: 清空所有编译
:clean
set /p r="Are You sure clean all ??? (Y/N)"
if  %r% == N  goto :eof
if  %r% == n  goto :eof
rd /s /Q %COREPATH%\\libuv\\libs       %COREPATH%\\libuv\\obj
rd /s /Q %COREPATH%\\MIMProtocol\\libs %COREPATH%\\MIMProtocol\\obj
rd /s /Q %COREPATH%\\uvbase\\libs      %COREPATH%\\uvbase\\obj
rd /s /Q %TOPPATH%\\sqliter\\libs      %TOPPATH%\\obj
rd /s /Q %TOPPATH%\\libs               %TOPPATH%\\obj
echo "clean all..."
goto :eof

:: 检测NDK是否安装配置
:ndk_error
echo "Warning: %NDKROOT% is EMPTY!!! %YOU ARE NOT SET NDK-ROOT PATH"
goto :eof
