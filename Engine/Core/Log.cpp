#include "Log.h"
#include <cstdio>
#include <cstdarg>

void Log::Init()
{
    printf("[Log] System initialized\n");
}

void Log::Shutdown()
{
    printf("[Log] System Shutdown");
}

static void PrintLevel(const char *level, const char *fmt, va_list args)
{
    printf("[%s] ", level);
    vprintf(fmt, args);
    printf("\n");
}

void Log::Info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    PrintLevel("Info", fmt, args);
    va_end(args);
}
void Log::Warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    PrintLevel("Warn", fmt, args);
    va_end(args);
}
void Log::Error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    PrintLevel("Error", fmt, args);
    va_end(args);
}
void Log::Debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    PrintLevel("Debug", fmt, args);
    va_end(args);
}