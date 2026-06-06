#include "Time_E.h"
#include <chrono>

double Time_E::Now()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

void Time_E::UpdateFrameTime()
{
    double now = Now();
    deltaTime = now - lateUpdateTime;
    lateUpdateTime = now;
}

double Time_E::DeltaTime()
{
    return deltaTime;
}