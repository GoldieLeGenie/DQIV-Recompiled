#include "NitroGeometry.hpp"
#include <cstring>
#include <stdexcept>
#include <nitro/native_g3.h>
namespace pc {
namespace { thread_local GxGeometry* current=nullptr; }
NitroGeometry::NitroGeometry(GxGeometry& g):previous_(current) {current=&g;}
NitroGeometry::~NitroGeometry() {current=previous_;}
GxGeometry& activeGeometry() {
    if(!current) throw std::runtime_error("No native geometry context is active");
    return *current;
}
}
extern "C" void pcG3Write(unsigned int op,u32 word) {
    if(op==0)pc::activeGeometry().fifo(word);
    else pc::activeGeometry().direct(op,word);
}
extern "C" void pcG3Send(const void* src,u32 size) {
    if(size%4)throw std::runtime_error("Unaligned geometry transfer size");
    const auto* bytes=static_cast<const unsigned char*>(src);
    for(u32 i=0;i<size;i+=4) {u32 word;std::memcpy(&word,bytes+i,4);pc::activeGeometry().fifo(word);}
}
extern "C" int func_020653a8(void* dest) {
    const auto m=pc::activeGeometry().clipMatrix();std::memcpy(dest,m.data(),64);return 0;
}
extern "C" int func_020653d8(void* dest) {
    const auto& m=pc::activeGeometry().vectorMatrix();
    auto* bytes=static_cast<unsigned char*>(dest);
    for(unsigned i=0;i<3;++i)std::memcpy(bytes+12*i,m.data()+4*i,12);
    return 0;
}
extern "C" int func_020654e4(int* level) {*level=pc::activeGeometry().positionStackLevel();return 0;}
extern "C" int func_02065514(int* level) {*level=pc::activeGeometry().projectionStackLevel();return 0;}
extern "C" int func_02065544(int* result) {*result=pc::activeGeometry().boxTestResult()?2:0;return 0;}
extern "C" void func_02065228() {
    auto& g=pc::activeGeometry();
    pcG3Write(0x10,3);pcG3Write(0x15,0);
    pcG3Write(0x10,0);pcG3Write(0x12,g.projectionStackLevel());pcG3Write(0x15,0);
    pcG3Write(0x10,2);pcG3Write(0x12,g.positionStackLevel());pcG3Write(0x15,0);
}
extern "C" void func_020652c0() {
    auto& g=pc::activeGeometry();
    pcG3Write(0x10,3);pcG3Write(0x15,0);
    pcG3Write(0x10,0);if(g.projectionStackLevel())pcG3Write(0x12,g.projectionStackLevel());
    pcG3Write(0x10,2);pcG3Write(0x12,g.positionStackLevel());pcG3Write(0x15,0);
}
extern "C" void func_0206541c(u32 rgb,u32 alpha,u32 depth,u32 polygon,int fog) {
    auto& state=pc::activeGeometry().raster;
    state.clearColor=rgb|(alpha<<16)|(polygon<<24)|(fog?0x8000:0);state.clearDepth=static_cast<std::uint16_t>(depth);
}
extern "C" void func_02065350(int enabled,int alphaOnly,int slope,int offset) {
    auto& state=pc::activeGeometry().raster;
    state.fogEnabled=enabled!=0;
    if(enabled) {state.fogAlphaOnly=alphaOnly!=0;state.fogSlope=slope;state.fogOffset=static_cast<std::uint16_t>(offset);}
}
// The ROM copies 32 bytes to 0x04000360: this entry point loads the fog table.
extern "C" void func_02065408(const void* table) {std::memcpy(pc::activeGeometry().raster.fogTable.data(),table,32);}
extern "C" void func_02065444() {
    auto& g=pc::activeGeometry();g.raster.fogTable.fill(0);g.raster.edgeTable.fill(0);g.raster.toonTable.fill(0);
    const std::array<u32,32> table{};g.command(0x34,table);
}
extern "C" void func_02065088() {
    auto& g=pc::activeGeometry();g=pc::GxGeometry{};
    func_02065228();func_02065444();pcG3Write(0x29,0x1f0080);
}
extern "C" void func_02065194() {
    func_020652c0();pcG3Write(0x29,0x1f0080);pcG3Write(0x2a,0);pcG3Write(0x2b,0);
}
namespace {
void matrix(unsigned op,const void* data,unsigned words) {
    const auto* bytes=static_cast<const unsigned char*>(data);
    for(unsigned i=0;i<words;++i) {u32 word;std::memcpy(&word,bytes+4*i,4);pcG3Write(op,word);}
}
}
extern "C" void func_02065034(const void* m) {matrix(0x17,m,12);}
extern "C" void func_02065050(const void* m) {matrix(0x19,m,12);}
extern "C" void func_0206506c(const void* m) {matrix(0x1a,m,9);}
extern "C" void func_02066dcc(const void* m,volatile void* dest) {
    const auto address=reinterpret_cast<std::uintptr_t>(dest);
    if(address==0x04000400)pcG3Send(m,48);
    else if(address>=0x04000440 && address<=0x040005cc && !(address&3))matrix(unsigned((address-0x04000400)/4),m,12);
    else throw std::runtime_error("Unsupported geometry FIFO destination");
}
extern "C" void func_02067540(u32,const void* dl,u32 size,void (*done)(void*),void* arg) {
    pcG3Send(dl,size);if(done)done(arg);
}
extern "C" void func_02067744(u32 channel,const void* dl,u32 size,void (*done)(void*),void* arg) {
    func_02067540(channel,dl,size,done,arg);
}
