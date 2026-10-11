#include "Texture.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace pc {
Texture Texture::load(std::span<const std::uint8_t> b, bool transparentZero) {
    if (b.size() < 0x70 || std::memcmp(b.data(), "TextureObject", 13) != 0)
        throw std::runtime_error("Invalid TextureObject header");
    auto u32 = [&](std::size_t o) { return std::uint32_t(b[o]) | (std::uint32_t(b[o+1]) << 8) | (std::uint32_t(b[o+2]) << 16) | (std::uint32_t(b[o+3]) << 24); };
    auto range = [&](std::size_t o, std::size_t n) {
        if (o > b.size() || n > b.size() - o) throw std::runtime_error("TextureObject data out of bounds");
    };
    const auto format = u32(0x20), sx = u32(0x24), sy = u32(0x28);
    if (sx > 7 || sy > 7 || u32(0x18) != 0 || (u32(0x14) & 1))
        throw std::runtime_error("Unsupported relocated or layered TextureObject");
    int bpp;
    switch (format) {
    case 1: case 4: case 6: bpp = 8; break;
    case 2: bpp = 2; break;
    case 3: bpp = 4; break;
    case 7: bpp = 16; break;
    default: throw std::runtime_error("Unsupported TextureObject pixel format");
    }
    const auto size = u32(0x30), offset = u32(0x34), paletteSize = u32(0x38), palette = u32(0x3c);
    range(offset, size); range(palette, paletteSize);
    Texture t; t.width = 8 << sx; t.height = 8 << sy;
    const auto pixelsAvailable = std::size_t(size) * 8 / bpp;
    if (!size || pixelsAvailable > std::size_t(t.width) * t.height || pixelsAvailable % t.width != 0)
        throw std::runtime_error("Invalid TextureObject pixel count");
    // Full-screen images contain 192 rows in a nominal 256-row GX texture.
    t.height = static_cast<int>(pixelsAvailable / t.width);
    return decodeGX(t.width, t.height, format, b.subspan(offset, size), b.subspan(palette, paletteSize), transparentZero);
}
Texture Texture::decodeGX(int width, int height, unsigned format, std::span<const std::uint8_t> bytes,
    std::span<const std::uint8_t> palette, bool transparentZero, std::span<const std::uint8_t> blockInfo) {
    if (width <= 0 || height <= 0 || width > 1024 || height > 1024) throw std::runtime_error("Invalid GX texture dimensions");
    int bpp;
    switch (format) {
    case 1: case 4: case 6: bpp = 8; break;
    case 2: case 5: bpp = 2; break;
    case 3: bpp = 4; break;
    case 7: bpp = 16; break;
    default: throw std::runtime_error("Unsupported GX texture format");
    }
    Texture t; t.width = width; t.height = height;
    const auto pixelsAvailable = std::size_t(width) * height;
    if (bytes.size() < (pixelsAvailable * bpp + 7) / 8) throw std::runtime_error("Truncated GX texture pixels");
    t.pixels.resize(pixelsAvailable);
    auto rgb = [](unsigned c, unsigned a) {
        auto expand = [](unsigned v) { return (v << 3) | (v >> 2); };
        return (a << 24) | (expand(c & 31) << 16) | (expand((c >> 5) & 31) << 8) | expand((c >> 10) & 31);
    };
    if(format==5) {
        if((width%4)||(height%4)||blockInfo.size()<pixelsAvailable/8)throw std::runtime_error("Truncated GX 4x4 block information");
        auto color=[&](unsigned offset) {
            if(offset+2>palette.size())throw std::runtime_error("GX 4x4 palette out of bounds");
            return unsigned(palette[offset])|(unsigned(palette[offset+1])<<8);
        };
        auto blend=[](unsigned a,unsigned b,unsigned aw,unsigned bw,unsigned shift) {
            unsigned result=0;
            for(unsigned c=0;c<3;++c)result|=(( ((a>>(c*5))&31)*aw+((b>>(c*5))&31)*bw)>>shift)<<(c*5);
            return result;
        };
        for(unsigned by=0;by<unsigned(height)/4;++by)for(unsigned bx=0;bx<unsigned(width)/4;++bx) {
            const unsigned block=by*(width/4)+bx;
            const unsigned info=blockInfo[block*2]|(unsigned(blockInfo[block*2+1])<<8),offset=(info&16383)*4,mode=info>>14;
            const unsigned c0=color(offset),c1=color(offset+2);
            unsigned colors[4]={rgb(c0,255),rgb(c1,255),0,0};
            colors[2]=rgb(mode==0||mode==2 ? color(offset+4) : blend(c0,c1,mode==1?1:5,mode==1?1:3,mode==1?1:3),255);
            if(mode==2)colors[3]=rgb(color(offset+6),255);
            if(mode==3)colors[3]=rgb(blend(c0,c1,3,5,3),255);
            for(unsigned y=0;y<4;++y)for(unsigned x=0;x<4;++x)
                t.pixels[std::size_t(by*4+y)*width+bx*4+x]=colors[(bytes[block*4+y]>>(x*2))&3];
        }
        return t;
    }
    for (std::size_t i = 0; i < pixelsAvailable; ++i) {
        unsigned index = 0, alpha = 255;
        if (format == 7) {
            const unsigned c = bytes[i * 2] | (unsigned(bytes[i * 2 + 1]) << 8);
            t.pixels[i] = rgb(c, c & 0x8000 ? 255 : 0); continue;
        }
        const unsigned value = bytes[i * bpp / 8];
        if (format == 1) { index = value & 31; alpha = ((value >> 5) * 255 + 3) / 7; }
        else if (format == 6) { index = value & 7; alpha = ((value >> 3) * 255 + 15) / 31; }
        else { index = (value >> (i * bpp % 8)) & ((1u << bpp) - 1); if (!index && transparentZero) alpha = 0; }
        if (index * 2 + 2 > palette.size()) throw std::runtime_error("GX palette index out of bounds");
        const unsigned c = palette[index * 2] | (unsigned(palette[index * 2 + 1]) << 8);
        t.pixels[i] = rgb(c, alpha);
    }
    return t;
}
void Texture::draw(std::vector<std::uint32_t>& target, int tw, int th, int x, int y) const {
    if (tw < 0 || th < 0 || target.size() != std::size_t(tw) * th) throw std::runtime_error("Invalid texture destination");
    for (int j = 0; j < height; ++j) for (int i = 0; i < width; ++i) {
        if (x+i < 0 || x+i >= tw || y+j < 0 || y+j >= th) continue;
        const auto source = pixels[std::size_t(j) * width + i], a = source >> 24;
        auto& dest = target[std::size_t(y+j) * tw + x+i];
        if (a == 255) dest = source;
        else if (a) {
            unsigned result = 0xff000000;
            for (int shift : {0, 8, 16}) result |= ((((source >> shift) & 255) * a + ((dest >> shift) & 255) * (255-a) + 127) / 255) << shift;
            dest = result;
        }
    }
}
}
