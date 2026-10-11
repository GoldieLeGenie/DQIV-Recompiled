#pragma once
namespace pc {
// Submit a model's original SBC and display lists to the active geometry context.
// The caller supplies the camera/global state and owns the model resource.
void drawNitroModel(void* model,void* jointAnimation=nullptr,int frame=0);
void resetNitroScene();
}
