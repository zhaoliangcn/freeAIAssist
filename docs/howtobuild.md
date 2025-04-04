# 如何构建 chat-ai-ui 项目

## 环境要求

- **CMake**: 3.14 或更高版本
- **Qt**: 5.14.1 (MSVC 2017 64-bit)

## Windows 构建步骤

1. 安装 Visual Studio 2022 (Community 或更高版本)
2. 安装 Qt 5.14.1 (MSVC 2017 64-bit)
3. 设置环境变量：
   - 运行 `vcvars64.bat` 以配置 Visual Studio 环境
   - 确保 `qmake.exe` 在系统 PATH 中
4. 执行构建脚本：
   ```bash
   build-chat-ai-ui.bat
   ```

## Linux 构建步骤

1. 安装依赖：
   ```bash
   sudo apt-get install build-essential cmake qt5-default
   ```
2. 配置并构建项目：
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

# How to Build chat-ai-ui Project

## Environment Requirements

- **CMake**: 3.14 or higher
- **Qt**: 5.14.1 (MSVC 2017 64-bit)

## Windows Build Steps

1. Install Visual Studio 2022 (Community or higher)
2. Install Qt 5.14.1 (MSVC 2017 64-bit)
3. Set up environment variables:
   - Run `vcvars64.bat` to configure Visual Studio environment
   - Ensure `qmake.exe` is in system PATH
4. Execute build script:
   ```bash
   build-chat-ai-ui.bat
   ```

## Linux Build Steps

1. Install dependencies:
   ```bash
   sudo apt-get install build-essential cmake qt5-default
   ```
2. Configure and build project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```