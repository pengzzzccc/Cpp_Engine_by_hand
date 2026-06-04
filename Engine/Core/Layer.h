// Engine/Core/Layer.h

class Layer
{
public:
    void OnAttach();
    void OnDetach();
    void OnUpdate(float deltaTime);
    void OnRender();
    bool OnEvent(const Event &);
};