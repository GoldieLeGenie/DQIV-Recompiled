#include "NitroVideo.hpp"
#include "NitroGeometry.hpp"
#include "main/dss/DssCore.hpp"
#include <nitro/native_video.h>
#include <array>
#include <vector>
extern "C" {
void GX_Init();
void GX_DispOff();
void GX_SetBankForLcdc(int);
int GX_VBlankIntr(int);
}
int data_02116ce4=0;
// These hooks are empty in the original retail game as well.
void unkfunc_0207e7e0() {}
void unkfunc_0207e7e4() {}
void debugPrintfStub(const char*,...) {}
int getTick() {return static_cast<int>(func_02079bf0());}

void initSystem(int extendedArena) {
    // Host heaps, files, clock and graphics contexts are established by main.
    (void)pc::activeVideo();(void)pc::activeGeometry();
    data_02116ce4=extendedArena!=0;
    GX_Init();func_02063920(-1);GX_DispOff();
    *VIDEO_REG(u32,0x04001000)&=~0x10000u;
    GX_VBlankIntr(1);
    // Keep the original sound initialization; its native backend must exist
    // before this application's startup can be considered operational.
    func_0207315c();
}
void clearVram() {
    auto& video=pc::activeVideo();GX_SetBankForLcdc(0x1ff);
    std::vector<std::uint8_t> zero(0xa4000);video.write(0x06800000,zero);
    func_02064abc();
    std::array<std::uint8_t,0x800> oam{};
    for(std::size_t i=0;i<oam.size();i+=4)oam[i]=0xc0;
    video.write(0x07000000,oam);video.write(0x05000000,{zero.data(),0x800});
}
void unkfunc_0207ecf4() {
    func_02065088();func_02065228();func_0206dfa4();
    *VIDEO_REG(u32,0x04000000)&=~0x3f000000u;
}
void setMainBrightness(int brightness) {func_020638f8(VIDEO_REG(u16,0x0400006c),brightness);}
void setSubBrightness(int brightness) {func_020638f8(VIDEO_REG(u16,0x0400106c),brightness);}
