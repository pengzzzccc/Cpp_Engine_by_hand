#include "Core/Application.h"

int main()
{
    Application app;
    if (app.Init())
    {
        app.Run();
        app.Shutdown();
    }
    return 0;
}