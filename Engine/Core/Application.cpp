#include "Application.h"

void Application::Run()
{
}

bool Application::Init()
{
    Log::Init();

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
