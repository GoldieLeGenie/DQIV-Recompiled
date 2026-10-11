#pragma once
#include <cstddef>
#include <memory>
#include <span>

namespace pc {
using OverlayFunction = void (__cdecl*)();
struct OverlayRange { unsigned char* begin; unsigned char* end; };
struct OverlayImage {
    unsigned id;
    OverlayRange data, bss;
    const OverlayFunction* constructorsBegin;
    const OverlayFunction* constructorsEnd;
};

// Code and relocated pointers stay resident. Each load restores the module's
// original writable image, then constructs its objects; unload destroys them.
class NitroOverlays {
public:
    explicit NitroOverlays(std::span<const OverlayImage> images);
    ~NitroOverlays();
    NitroOverlays(const NitroOverlays&) = delete;
    NitroOverlays& operator=(const NitroOverlays&) = delete;
    bool load(unsigned id);
    bool unload(unsigned id);
    bool loaded(unsigned id) const;
    void registerDestructor(unsigned id, OverlayFunction function);
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
extern "C" int __cdecl pcRegisterOverlayDestructor(unsigned id, pc::OverlayFunction function);
