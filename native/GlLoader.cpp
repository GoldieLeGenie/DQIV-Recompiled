#include "GlLoader.hpp"
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>

namespace pc::gl {
#define DQIV_GL11_DEFINE(name,symbol) decltype(&::symbol) name=nullptr;
#define DQIV_GL_DEFINE(name,type,symbol) type name=nullptr;
DQIV_GL11_FUNCTIONS(DQIV_GL11_DEFINE)
DQIV_GL_FUNCTIONS(DQIV_GL_DEFINE)
#undef DQIV_GL11_DEFINE
#undef DQIV_GL_DEFINE
void load() {
    auto resolve=[](const char* symbol) {
        auto* function=SDL_GL_GetProcAddress(symbol);
        if(!function)throw std::runtime_error(std::string("OpenGL function unavailable: ")+symbol);
        return function;
    };
#define DQIV_GL11_LOAD(name,symbol) name=reinterpret_cast<decltype(name)>(resolve(#symbol));
#define DQIV_GL_LOAD(name,type,symbol) name=reinterpret_cast<type>(resolve(#symbol));
    DQIV_GL11_FUNCTIONS(DQIV_GL11_LOAD)
    DQIV_GL_FUNCTIONS(DQIV_GL_LOAD)
#undef DQIV_GL11_LOAD
#undef DQIV_GL_LOAD
}
}
