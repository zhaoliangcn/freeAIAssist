@echo off
chcp 65001
cd /d "%~dp0"

rem 清理旧的UI头文件
if exist ui_*.h (
    del ui_*.h
    if %errorlevel% neq 0 (
        echo 清理ui_*.h文件失败，请检查文件权限
        pause
        exit /b 1
    )
)

rem Set Visual Studio 2022 x64 environment variables
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

rem Define the project file path
set PROJECT_FILE=.\chat-ai-ui.pro

rem Define the full path of qmake
set QMAKE_PATH=C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin\qmake.exe

rem Call qmake to generate the Makefile
%QMAKE_PATH% %PROJECT_FILE%

rem Check if qmake executed successfully
if %errorlevel% neq 0 (
    echo qmake execution failed. Please check the project file and Qt environment configuration.
    pause
    exit /b 1
)

rem Call nmake to build the project
nmake

rem Check if nmake executed successfully
if %errorlevel% neq 0 (
    echo nmake execution failed. Please check the code and build environment.
    pause
    exit /b 1
)

echo 项目构建成功！

rem 定义构建输出目录，这里假设是当前目录下的 release 或 debug 目录
set BUILD_OUTPUT_DIR=debug
if not exist %BUILD_OUTPUT_DIR% (
    mkdir %BUILD_OUTPUT_DIR%
)
if not exist %BUILD_OUTPUT_DIR% (
    set BUILD_OUTPUT_DIR=debug
if not exist %BUILD_OUTPUT_DIR% (
    mkdir %BUILD_OUTPUT_DIR%
)
)

set QT_DIR=C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\plugins
rem 定义 Qt 库目录
set QT_LIB_DIR=C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin

set QT_PLUGIN_DIR=C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\plugins

rem 拷贝 Qt 依赖库到构建输出目录
copy %QT_LIB_DIR%\Qt5Cored.dll %BUILD_OUTPUT_DIR%
copy %QT_LIB_DIR%\Qt5Guid.dll %BUILD_OUTPUT_DIR%
copy %QT_LIB_DIR%\Qt5Widgetsd.dll %BUILD_OUTPUT_DIR%
copy %QT_LIB_DIR%\Qt5Networkd.dll %BUILD_OUTPUT_DIR%
copy %QT_LIB_DIR%\Qt5TextToSpeechd.dll %BUILD_OUTPUT_DIR%
copy %QT_LIB_DIR%\Qt5Guid.dll %BUILD_OUTPUT_DIR%
copy %QT_LIB_DIR%\Qt5Widgetsd.dll %BUILD_OUTPUT_DIR%
copy %QT_LIB_DIR%\Qt5Networkd.dll %BUILD_OUTPUT_DIR%
rem 可以根据项目实际依赖添加更多的库

rem 拷贝 platforms 文件夹到构建输出目录
xcopy /E /I /Y "%QT_PLUGIN_DIR%\platforms" "%BUILD_OUTPUT_DIR%\plugins\platforms"
xcopy /E /I /Y "%QT_PLUGIN_DIR%\platformthemes" "%BUILD_OUTPUT_DIR%\plugins\platformthemes"
xcopy /E /I /Y "%QT_PLUGIN_DIR%\texttospeech" "%BUILD_OUTPUT_DIR%\plugins\texttospeech"

echo Qt 依赖库已拷贝到构建输出目录！
pause