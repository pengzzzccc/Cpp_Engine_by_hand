// Engine/Core/Assert.h
#pragma once
#include "Log.h"
#include <cstdlib>

#define ENGINE_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            Log::Error("ASSERT failed: %s [%s:%d]", msg, __FILE__, __LINE__); \
            std::abort(); \
        } \
    } while(0)
