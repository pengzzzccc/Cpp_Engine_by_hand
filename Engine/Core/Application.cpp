#include "Application.h"


void Application::Run()
{
}

bool Application::Init()
{
    Log::Init();

    Time_E t;
    double curr = t.DeltaTime();


    Log::Info("Time: %f" ,curr);

    return true;
}

void Application::Shutdown()
{
}

void Application::RequestClose()
{
}

bool Application::IsRunning()
{
    return true;
}

void Application::PushLayer(Layer *)
{
}

void Application::PopLayer(Layer *)
{
}
