// Engine/Core/Application.h
#pragma once

#include "Assert.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Log.h"
#include "Time_E.h"

class Application
{
public:
    void Run();
    bool Init();
    void Shutdown();

    void RequestClose();
    bool IsRunning();
    void PushLayer(Layer *);
    void PopLayer(Layer *);
};