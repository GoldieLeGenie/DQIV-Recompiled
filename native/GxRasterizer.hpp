#pragma once
#include "GxGeometry.hpp"
#include "GxTextures.hpp"
namespace pc {
struct GxRenderOptions {
    int width=256,height=192;
    unsigned displayControl=9,alphaTest=0;
    bool wBuffer=false,manualSort=false;
};
// Uses the game's homogeneous clip coordinates and viewport. No preview camera.
class GxRasterizer {
public:
    void draw(std::span<const GxTriangle>,const GxRasterState&,GxTextures&,GxRenderOptions={});
    const std::vector<std::uint32_t>& pixels() const {return pixels_;}
    const std::vector<std::uint32_t>& depths() const {return depth_;}
    std::size_t drawnPixels() const {return drawn_;}
private:
    std::vector<std::uint32_t> pixels_,depth_;
    std::vector<unsigned char> polygon_,translucent_,fog_,shadow_;
    std::size_t drawn_=0;
};
}
