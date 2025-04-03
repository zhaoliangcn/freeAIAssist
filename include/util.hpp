#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif
#include <string>

std::string GetCurrentModulePath() {
    char currentPath[4096];  // 用于存储路径的缓冲区
#ifdef _WIN32
    // 在Windows系统下获取当前可执行文件的完整路径
    GetModuleFileNameA(NULL, currentPath, 4096);
    // 查找路径中最后一个反斜杠
    char* p = strrchr(currentPath, '\\');
    // 如果找到反斜杠，将其替换为字符串结束符，以获取目录路径
    if (p) *p = 0;
#else
    // 在Linux/Unix系统下通过读取符号链接获取可执行文件路径
    ssize_t count = readlink("/proc/self/exe", currentPath, sizeof(currentPath) - 1);
    if (count != -1) {
        // 添加字符串结束符
        currentPath[count] = '\0';
        // 查找路径中最后一个正斜杠
        char* p = strrchr(currentPath, '/');
        // 如果找到正斜杠，将其替换为字符串结束符，以获取目录路径
        if (p) *p = 0;
    }
#endif
    // 将字符数组转换为std::string并返回
    return std::string(currentPath);
}