@echo off
REM 设置工作目录为当前目录
cd /d "%~dp0"

REM 检查是否存在 release 文件夹
if not exist "release" (
    echo release 文件夹不存在！
    pause
    exit /b 1
)

REM 创建临时文件夹
if not exist "temp" mkdir "temp"

REM 使用 robocopy 复制 .exe 文件及其依赖文件到临时文件夹
robocopy "release" "temp" *.exe /s /njh /njs /ndl /nc /ns /np
robocopy "release" "temp" *.dll /s /njh /njs /ndl /nc /ns /np
robocopy "release" "temp" *.json /s /njh /njs /ndl /nc /ns /np
robocopy "release\doctotext" "temp\doctotext" * /s /njh /njs /ndl /nc /ns /np
robocopy "release\plugins" "temp\plugins" * /s /njh /njs /ndl /nc /ns /np
robocopy "release\pdf" "temp\plugins" * /s /njh /njs /ndl /nc /ns /np
REM 打包为 release.zip
powershell -Command "Compress-Archive -Path 'temp\*' -DestinationPath 'release.zip' -Force"

REM 删除临时文件夹
rd /s /q "temp"

echo 打包完成！release.zip 已生成。
pause
