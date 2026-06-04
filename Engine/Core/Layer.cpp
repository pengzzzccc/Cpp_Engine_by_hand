#include "Layer.h"
#include <iostream>

using namespace std;

void Layer::OnAttach() {}
void Layer::OnDetach() {}
void Layer::OnUpdate(float deltaTime) {}
void Layer::OnRender() {}
bool Layer::OnEvent() { return true } // need event
