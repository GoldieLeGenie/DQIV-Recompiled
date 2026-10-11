#include "NitroVideo.hpp"
#include <algorithm>
#include <atomic>
#include <cstring>
#include <stdexcept>
#include <string>
#include <nitro/native_video.h>
namespace pc {
namespace {thread_local NitroVideo* current=nullptr;std::atomic<std::uint64_t> nextRevision{0};}
NitroVideo::NitroVideo():vram_(0xa4000),previous_(current),revision_(++nextRevision) {current=this;}
NitroVideo::~NitroVideo() {current=previous_;}
NitroVideo& activeVideo() {if(!current)throw std::runtime_error("No native video context is active");return *current;}
void* NitroVideo::reg(std::uint32_t address,std::size_t width) {
    const auto offset=address-0x04000000u;
    if(!width || offset>=registers_.size() || width>registers_.size()-offset || (offset%width))
        throw std::runtime_error("Invalid native video register address");
    const bool supported=offset<=0x70 || (offset>=0x1000&&offset<=0x1070) ||
        (offset>=0x240&&offset<=0x249) || offset==0x304 || offset==0x320 ||
        (offset>=0x330&&offset<0x3c0) || (offset>=0x600&&offset<0x6a4);
    if(!supported)throw std::runtime_error("Unported register access: "+std::to_string(address));
    return registers_.data()+offset;
}
std::vector<NitroVideo::Mapping> NitroVideo::mappings(std::uint32_t address) const {
    constexpr unsigned offsets[9]={0,0x20000,0x40000,0x60000,0x80000,0x90000,0x94000,0x98000,0xa0000};
    constexpr unsigned sizes[9]={0x20000,0x20000,0x20000,0x20000,0x10000,0x4000,0x4000,0x8000,0x4000};
    std::vector<Mapping> result;
    for(unsigned bank=0;bank<9;++bank) {
        const unsigned control=registers_[0x240+bank+(bank>=7?1:0)];
        if(!(control&0x80))continue;
        const auto mode=control&7,slot=(control>>3)&3;
        if(mode==0 && address>=0x06800000+offsets[bank] && address<0x06800000+offsets[bank]+sizes[bank]) {
            const auto offset=address-0x06800000;
            result.push_back({offset,offsets[bank]+sizes[bank]-offset});continue;
        }
        std::uint32_t base=0;
        if(bank<4) {
            if(mode==1)base=0x06000000+slot*0x20000;
            if(bank<2 && mode==2)base=0x06400000+(slot&1)*0x20000;
            if(bank==2 && mode==4)base=0x06200000;
            if(bank==3 && mode==4)base=0x06600000;
        } else if(bank<=6) {
            const unsigned offset=bank==4?0:(slot&1)*0x4000+(slot&2)*0x8000;
            if(mode==1)base=0x06000000+offset;
            if(mode==2)base=0x06400000+offset;
        } else if(bank==7 && mode==1)base=0x06200000;
        else if(bank==8) {
            if(mode==1)base=0x06208000;
            if(mode==2)base=0x06600000;
        }
        if(base && address>=base && address-base<sizes[bank])result.push_back({offsets[bank]+address-base,sizes[bank]-(address-base)});
    }
    return result;
}
void NitroVideo::write(std::uint32_t address,std::span<const std::uint8_t> source) {
    if(source.empty())return;
    revision_=++nextRevision;
    auto simple=[&](auto& target,unsigned base) {
        const auto offset=address-base;
        if(offset>target.size() || source.size()>target.size()-offset)throw std::runtime_error("Video transfer exceeds memory region");
        std::copy(source.begin(),source.end(),target.begin()+offset);
    };
    if(address>=0x05000000 && address<0x05000800) {simple(palette_,0x05000000);return;}
    if(address>=0x07000000 && address<0x07000800) {simple(oam_,0x07000000);return;}
    std::size_t done=0;
    while(done<source.size()) {
        auto maps=mappings(address+static_cast<unsigned>(done));
        if(maps.empty())throw std::runtime_error("Write to unmapped video memory");
        auto size=source.size()-done;for(const auto& m:maps)size=std::min(size,m.size);
        for(const auto& m:maps) {
            std::copy_n(source.data()+done,size,vram_.data()+m.offset);
            // Banks A-D hold texture images and E-G texture palettes in mode 3;
            // they are usually filled while mapped to the LCDC, then remapped.
            constexpr unsigned starts[7]={0,0x20000,0x40000,0x60000,0x80000,0x90000,0x94000},ends[7]={0x20000,0x40000,0x60000,0x80000,0x90000,0x94000,0x98000};
            for(unsigned bank=0;bank<7;++bank) {
                const unsigned mode=registers_[0x240+bank]&0x87;
                if(m.offset<ends[bank]&&m.offset+size>starts[bank]&&(mode==0x83||mode==0x80))textureRevision_=revision_;
            }
        }
        done+=size;
    }
}
std::uint64_t NitroVideo::textureBanks() const {
    std::uint64_t result=0;
    for(unsigned i=0;i<7;++i)result|=std::uint64_t(registers_[0x240+i])<<(i*8);
    return result;
}
void NitroVideo::readTexture(bool palette,std::size_t offset,std::span<std::uint8_t> dest) const {
    const std::size_t capacity=palette?0x18000:0x80000;
    if(offset>capacity || dest.size()>capacity-offset)throw std::runtime_error("Texture memory read outside logical range");
    std::fill(dest.begin(),dest.end(),std::uint8_t{0});
    constexpr unsigned physical[7]={0,0x20000,0x40000,0x60000,0x80000,0x90000,0x94000};
    for(unsigned bank=palette?4:0;bank<(palette?7u:4u);++bank) {
        const unsigned control=registers_[0x240+bank];if((control&0x87)!=0x83)continue;
        const unsigned slot=(control>>3)&3;
        const std::size_t base=palette ? (bank==4?0:(slot&1)*0x4000+(slot&2)*0x8000) : slot*0x20000;
        const std::size_t size=bank<4?0x20000:bank==4?0x10000:0x4000;
        const auto begin=std::max(offset,base),end=std::min(offset+dest.size(),base+size);
        for(auto i=begin;i<end;++i)dest[i-offset]|=vram_[physical[bank]+i-base];
    }
}
void NitroVideo::read(std::uint32_t address,std::span<std::uint8_t> dest) const {
    if(dest.empty())return;
    auto simple=[&](const auto& source,unsigned base) {
        const auto offset=address-base;
        if(offset>source.size() || dest.size()>source.size()-offset)throw std::runtime_error("Video read exceeds memory region");
        std::copy_n(source.data()+offset,dest.size(),dest.data());
    };
    if(address>=0x05000000 && address<0x05000800) {simple(palette_,0x05000000);return;}
    if(address>=0x07000000 && address<0x07000800) {simple(oam_,0x07000000);return;}
    std::size_t done=0;
    while(done<dest.size()) {
        auto maps=mappings(address+static_cast<unsigned>(done));
        if(maps.empty())throw std::runtime_error("Read from unmapped video memory");
        auto size=dest.size()-done;for(const auto& m:maps)size=std::min(size,m.size);
        std::fill_n(dest.data()+done,size,std::uint8_t{0});
        for(const auto& m:maps)for(std::size_t i=0;i<size;++i)dest[done+i]|=vram_[m.offset+i];
        done+=size;
    }
}
void NitroVideo::readDisplay(std::uint32_t address,std::span<std::uint8_t> dest) const {
    for(std::size_t done=0;done<dest.size();) {
        const auto addr=address+static_cast<unsigned>(done);
        const auto size=std::min(dest.size()-done,std::size_t(0x4000-(addr&0x3fff)));
        const auto maps=mappings(addr);
        auto* out=dest.data()+done;
        if(maps.empty())std::fill_n(out,size,std::uint8_t{0});
        else {
            // One bank is the usual case; overlapping banks are ORed together.
            const auto first=std::min(size,maps[0].size);
            std::memcpy(out,vram_.data()+maps[0].offset,first);
            std::fill(out+first,out+size,std::uint8_t{0});
            for(std::size_t m=1;m<maps.size();++m)
                for(std::size_t i=0;i<std::min(size,maps[m].size);++i)out[i]|=vram_[maps[m].offset+i];
        }
        done+=size;
    }
}
void NitroVideo::readExtendedPalette(bool sub,bool obj,std::span<std::uint8_t> dest) const {
    if(dest.size()>(obj?0x2000u:0x8000u))throw std::runtime_error("Extended palette read exceeds size");
    std::fill(dest.begin(),dest.end(),std::uint8_t{0});
    auto bank=[&](unsigned reg,unsigned mode,unsigned physical,unsigned base,unsigned size) {
        if((registers_[reg]&0x87u)!=(0x80|mode))return;
        for(unsigned i=base;i<std::min(dest.size(),std::size_t(base+size));++i)dest[i]|=vram_[physical+i-base];
    };
    if(sub) {if(obj)bank(0x249,3,0xa0000,0,0x2000);else bank(0x248,2,0x98000,0,0x8000);}
    else if(obj) {bank(0x245,5,0x90000,0,0x2000);bank(0x246,5,0x94000,0,0x2000);}
    else {
        bank(0x244,4,0x80000,0,0x8000);
        bank(0x245,4,0x90000,(registers_[0x245]&8)?0x4000:0,0x4000);
        bank(0x246,4,0x94000,(registers_[0x246]&8)?0x4000:0,0x4000);
    }
}
bool NitroVideo::lcdcBank(unsigned bank) const {return bank<4 && (registers_[0x240+bank]&0x87)==0x80;}
void* NitroVideo::lcdcSource(std::uint32_t address,std::size_t size) {
    const auto offset=address-0x06800000u;
    if(offset>=vram_.size() || size>vram_.size()-offset)
        throw std::runtime_error("LCDC source outside video memory");
    for(std::size_t done=0;done<size;) {
        const auto maps=mappings(address+static_cast<unsigned>(done));
        if(maps.size()!=1 || maps[0].offset!=offset+done)
            throw std::runtime_error("LCDC source bank is not CPU accessible");
        done+=std::min(size-done,maps[0].size);
    }
    return vram_.data()+offset;
}
void NitroVideo::capture(unsigned bank,unsigned offset,std::span<const std::uint16_t> pixels) {
    if(!lcdcBank(bank))return;
    for(std::size_t i=0;i<pixels.size();++i) {
        const auto p=bank*0x20000+((offset+i*2)&0x1ffff);
        vram_[p]=static_cast<std::uint8_t>(pixels[i]);vram_[p+1]=static_cast<std::uint8_t>(pixels[i]>>8);
    }
    revision_=++nextRevision;
}
}
extern "C" void* pcVideoRegister(u32 address,u32 width) {return pc::activeVideo().reg(address,width);}
extern "C" void* pcVideoLcdcSource(u32 address,u32 size) {return pc::activeVideo().lcdcSource(address,size);}
extern "C" void pcVideoFill(u32 destination,u32 value,u32 size) {
    const std::size_t words=(static_cast<std::size_t>(size)+3)/4;
    const std::vector<u32> values(words,value);
    pc::activeVideo().write(destination,{reinterpret_cast<const std::uint8_t*>(values.data()),words*4});
}
extern "C" void pcVideoCopy(u32 destination,const void* source,u32 size) {
    pc::activeVideo().write(destination,{static_cast<const std::uint8_t*>(source),size});
}
