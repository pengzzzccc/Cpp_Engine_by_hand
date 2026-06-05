// Engine/Core/Time.h
#pragma once

#include <time.h>

class Time
{
public:
    double Now();
    void UpdateFrameTime();
    double DeltaTime();
    // test
};