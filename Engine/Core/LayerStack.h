// Engine/Core/LayerStack.h
#pragma once

class LayerStack
{
public:
    void PushLayer(Layer *);
    void PushOverlay(Layer *);
    void PopLayer(Layer *);
    void PopOverlay(Layer *);
    void Begin();
    void End();
};