#include "GxTextures.hpp"
#include "NitroVideo.hpp"
#include "Profile.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace pc {
namespace {
std::uint64_t hashBytes(std::uint64_t hash,std::span<const std::uint8_t> bytes) {
    // FNV-1a over 8-byte words, then the tail.
    std::size_t i=0;
    for(;i+8<=bytes.size();i+=8) {
        std::uint64_t word;std::memcpy(&word,bytes.data()+i,8);
        hash=(hash^word)*1099511628211ull;
    }
    for(;i<bytes.size();++i)hash=(hash^bytes[i])*1099511628211ull;
    return hash;
}
}
GxTextures::GxTextures():textureBytes_(512*1024),paletteBytes_(96*1024) {}
std::span<const std::uint8_t> GxTextures::bytes(bool palette) const {return palette?paletteBytes_:textureBytes_;}
const Texture& GxTextures::texture(std::uint32_t parameters,std::uint32_t palette) {
    auto& video=activeVideo();
    if(video.textureRevision()!=videoRevision_ || video.textureBanks()!=bankSelection_) {
        video.readTexture(false,0,textureBytes_);video.readTexture(true,0,paletteBytes_);
        videoRevision_=video.textureRevision();bankSelection_=video.textureBanks();++memory_;
    }
    // Repetition, reflection and coordinate generation do not affect decoding.
    parameters&=0x3ff0ffffu;
    const std::uint64_t key=(std::uint64_t(palette)<<32)|parameters;
    auto found=cache_.find(key);
    if(found!=cache_.end()&&found->second.checked==memory_)return found->second.texture;
    const unsigned format=(parameters>>26)&7;
    if(!format)throw std::runtime_error("No texture specified");
    if(palette>paletteBytes_.size()/(format==2?8:16))throw std::runtime_error("GX palette address out of bounds");
    const unsigned offset=(parameters&65535)*8;
    const std::size_t paletteOffset=std::size_t(palette)*(format==2?8:16);
    auto data=bytes(false),pal=bytes(true);
    if(offset>data.size() || paletteOffset>pal.size())throw std::runtime_error("GX texture address out of bounds");
    const unsigned width=8<<((parameters>>20)&7),height=8<<((parameters>>23)&7);
    std::span<const std::uint8_t> info;
    if(format==5) {
        if(offset&0x20000)throw std::runtime_error("Compressed texture outside slot 0 or 2");
        const unsigned indexOffset=0x20000+((offset&0x1ffff)>>1)+((offset&0x40000)>>2);
        info=data.subspan(indexOffset);
    }
    // Texture memory changes often (animated characters): a texture is decoded
    // again only when the bytes it reads changed.
    static constexpr unsigned bits[8]={0,8,2,4,8,2,8,16};
    const std::size_t pixels=std::size_t(width)*height;
    std::size_t paletteBytes=0;
    switch(format) {
    case 1:paletteBytes=64;break;
    case 2:paletteBytes=8;break;
    case 3:paletteBytes=32;break;
    case 4:paletteBytes=512;break;
    case 6:paletteBytes=16;break;
    case 5: {
        // Block palettes: up to the last 4-colour set any block refers to.
        for(std::size_t block=0;block<pixels/16&&block*2+1<info.size();++block)
            paletteBytes=std::max<std::size_t>(paletteBytes,(((info[block*2]|(info[block*2+1]<<8))&16383)+2)*4);
        break;
    }
    }
    auto clip=[](std::span<const std::uint8_t> span,std::size_t size) {return span.first(std::min(size,span.size()));};
    std::uint64_t hash=hashBytes(1469598103934665603ull,clip(data.subspan(offset),pixels*bits[format]/8));
    hash=hashBytes(hash,clip(pal.subspan(paletteOffset),paletteBytes));
    if(format==5)hash=hashBytes(hash,clip(info,pixels/8));
    if(found!=cache_.end()&&found->second.hash==hash) {found->second.checked=memory_;return found->second.texture;}
    ProfileScope timing("texture-decode");
    auto& entry=cache_[key];
    entry.texture=Texture::decodeGX(width,height,format,data.subspan(offset),pal.subspan(paletteOffset),(parameters&(1u<<29))!=0,info);
    entry.texture.version=++versions_;entry.hash=hash;entry.checked=memory_;
    return entry.texture;
}
}
