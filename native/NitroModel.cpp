#include "NitroModel.hpp"
#include <vector>
#include <stdexcept>
extern "C" {
#include "../game/libs/NitroSystem/src/g3d/g3d_internal.h"
void func_0206a34c(NNSG3dRenderObj*,void*);
void func_0206adcc(void);
void func_0206a2bc(NNSG3dAnmObj*,void*,const NNSG3dResMdl*,void*);
void func_0206a4c0(NNSG3dRenderObj*,NNSG3dAnmObj*);
}
namespace pc {
void resetNitroScene() {func_0206ac78();func_0206adcc();}
void drawNitroModel(void* model,void* jointAnimation,int frame) {
    NNSG3dRenderObj object{};func_0206a34c(&object,model);
    std::vector<u32> storage;
    if(jointAnimation) {
        if(frame<0 || frame>0x7ffff)throw std::runtime_error("Invalid joint animation frame");
        const auto size=func_0206a26c(jointAnimation,object.resMdl);
        if(!size)throw std::runtime_error("Unsupported animation category");
        storage.resize((size+3)/4);
        auto* animation=reinterpret_cast<NNSG3dAnmObj*>(storage.data());
        func_0206a2bc(animation,jointAnimation,object.resMdl,nullptr);
        animation->frame=frame*4096;func_0206a4c0(&object,animation);
    }
    auto* previous=data_0210d18c;
    try {G3d_Render(&object);func_0206dcf0();}
    catch(...) {data_0210d18c=previous;throw;}
}
}
