#include "NitroOverlays.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace pc {
namespace { thread_local NitroOverlays* current = nullptr; }
struct NitroOverlays::Impl {
    enum class State { absent, loading, active, unloading };
    struct Module {
        OverlayImage image;
        std::vector<unsigned char> data, bss;
        std::vector<OverlayFunction> destructors;
        State state = State::absent;
    };
    std::vector<Module> modules;
    std::vector<unsigned> loadOrder;
    Module* find(unsigned id) {
        for(auto& module:modules) if(module.image.id==id) return &module;
        return nullptr;
    }
    static std::vector<unsigned char> snapshot(OverlayRange range) {
        const auto begin=reinterpret_cast<std::size_t>(range.begin);
        const auto end=reinterpret_cast<std::size_t>(range.end);
        if(end<begin) throw std::runtime_error("Invalid native overlay section bounds");
        std::vector<unsigned char> bytes(end-begin);
        if(!bytes.empty()) std::memcpy(bytes.data(),range.begin,bytes.size());
        return bytes;
    }
    static void restore(OverlayRange range,const std::vector<unsigned char>& bytes) {
        if(!bytes.empty()) std::memcpy(range.begin,bytes.data(),bytes.size());
    }
    static void destroy(Module& module) {
        // Destruction may register another object, as on the original SDK.
        while(!module.destructors.empty()) {
            const auto function=module.destructors.back();module.destructors.pop_back();function();
        }
    }
};
NitroOverlays::NitroOverlays(std::span<const OverlayImage> images):impl_(std::make_unique<Impl>()) {
    if(current) throw std::runtime_error("A native overlay context is already active");
    impl_->modules.reserve(images.size());
    for(const auto& image:images) {
        if(impl_->find(image.id)) throw std::runtime_error("Duplicate native overlay ID");
        impl_->modules.push_back({image,Impl::snapshot(image.data),Impl::snapshot(image.bss),{}});
    }
    current=this;
}
NitroOverlays::~NitroOverlays() {
    while(!impl_->loadOrder.empty()) unload(impl_->loadOrder.back());
    current=nullptr;
}
bool NitroOverlays::load(unsigned id) {
    auto* module=impl_->find(id);
    if(!module || module->state!=Impl::State::absent) return false;
    Impl::restore(module->image.data,module->data);
    Impl::restore(module->image.bss,module->bss);
    module->state=Impl::State::loading;
    try {
        for(auto p=module->image.constructorsBegin;p!=module->image.constructorsEnd;++p)
            if(*p) (*p)();
        impl_->loadOrder.push_back(id);
        module->state=Impl::State::active;
    } catch(...) {
        module->state=Impl::State::unloading;
        Impl::destroy(*module);module->state=Impl::State::absent;
        throw;
    }
    return true;
}
bool NitroOverlays::unload(unsigned id) {
    auto* module=impl_->find(id);
    if(!module || module->state!=Impl::State::active) return false;
    module->state=Impl::State::unloading;
    Impl::destroy(*module);module->state=Impl::State::absent;
    std::erase(impl_->loadOrder,id);
    return true;
}
bool NitroOverlays::loaded(unsigned id) const {
    auto* module=impl_->find(id);return module && module->state==Impl::State::active;
}
void NitroOverlays::registerDestructor(unsigned id,OverlayFunction function) {
    auto* module=impl_->find(id);
    if(!module || module->state==Impl::State::absent || !function)
        throw std::runtime_error("An overlay object was constructed outside its module lifetime");
    module->destructors.push_back(function);
}
}
extern "C" int __cdecl pcRegisterOverlayDestructor(unsigned id,pc::OverlayFunction function) {
    if(!pc::current) throw std::runtime_error("Native overlay constructor ran before the overlay context");
    pc::current->registerDestructor(id,function);return 0;
}
extern "C" int FS_LoadOverlay(void* target,unsigned id) {
    if(target) return 0; // This executable contains ARM9 game modules only.
    if(!pc::current) throw std::runtime_error("FS_LoadOverlay without a native overlay context");
    return pc::current->load(id)?1:0;
}
extern "C" int FS_UnloadOverlay(void* target,unsigned id) {
    if(target) return 0;
    if(!pc::current) throw std::runtime_error("FS_UnloadOverlay without a native overlay context");
    return pc::current->unload(id)?1:0;
}
