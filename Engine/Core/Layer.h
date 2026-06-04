// Engine/Core/Layer.h
#pragma once

class Layer
{
public:
    void OnAttach();
    void OnDetach();
    void OnUpdate(float deltaTime);
    void OnRender();
    bool OnEvent(); // need event
};