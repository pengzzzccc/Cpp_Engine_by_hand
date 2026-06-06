// Engine/Core/Time.h
#pragma once

class Time_E
{
public:
    double Now();
    void UpdateFrameTime();
    double DeltaTime();

private:
    double currentTime = 0.0;
    double deltaTime = 0.0;
    double lateUpdateTime = 0.0;
};