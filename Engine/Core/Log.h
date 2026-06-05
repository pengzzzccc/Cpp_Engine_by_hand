#pragma once
#include <cstdio>
#include <cstdarg>

namespace Log
{
    void Init();
    void Shutdown();

    void Info(const char *fmt, ...);
    void Warn(const char *fmt, ...);
    void Error(const char *fmt, ...);
    void Debug(const char *fmt, ...);
};