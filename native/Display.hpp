#pragma once
#include "NitroVideo.hpp"
namespace pc {
// Original display-engine composition and frame capture. Output is top then
// bottom with no physical console border or gap; presentation can scale it.
// Each frame also yields a composition recipe per pixel (recipes()), so a
// renderer with its own high-resolution 3D layer can finish the blend.
class Display {
public:
    // Recipe planes, 256 x 384 each (top screen first). ARGB words:
    // plane 0: rgb = 2D colour above the 3D layer, or the final colour; a = kind
    //          (0 final, 1 3D over plane 1, 2 colour blended over 3D with eva/evb).
    // plane 1: rgb = colour below the 3D layer; a = flags (1 3D alpha blends
    //          with it, 2 BLDY applies to the 3D, 8 only the backdrop shows).
    // plane 2: r = eva*8, g = evb*8, b = BLDY mode<<5|evy, a = master mode<<5|amount.
    // plane 3: a high-resolution picture drawn over the pixel (HdPictures tag
    //          with the pixel's place in it), or 0. Planes 0-2 then leave out
    //          the layer showing that picture.
    static constexpr unsigned recipePlanes=4;
    Display();
    // `subEngine` false skips the sub engine (its screen is not presented).
    void frame(NitroVideo&,std::span<const std::uint32_t> graphics3d,bool subEngine=true);
    std::span<const std::uint32_t> pixels() const {return screens_;}
    std::span<const std::uint32_t> mainGraphics() const {return graphics_;}
    std::span<const std::uint32_t> recipes() const {return recipes_;}
    // Main engine graphics recipes (3 planes of 256 x 192), before the display
    // mode selects VRAM or main memory output: what the capture unit sees.
    std::span<const std::uint32_t> mainGraphicsRecipes() const {return graphicsRecipes_;}
    bool mainOnTop() const {return mainTop_;}
    // The presenter keeps a picture of the world (3D target 0, the whole
    // canvas): recipes then show it where menus show their captured copy of
    // it (an untransformed direct-colour BG2 bitmap on the main engine) and
    // through the sub engine's bare backdrop.
    void setWorldBackground(bool on) {world_=on;}
    // The main engine shows such a copy: BG2 as a direct-colour bitmap, enabled
    // and untransformed, and BG0 not showing 3D.
    static bool showsBitmapBackdrop(NitroVideo&);
    // The engine's direct-colour bitmap background (2 or 3), or -1.
    static int directBitmapLayer(NitroVideo&,bool sub);
private:
    struct Pixel {std::uint32_t color=0;unsigned char priority=4,alpha=0;bool semi=false,three=false;};
    std::vector<std::uint8_t> bg_,obj_,bgPalette_,objPalette_;
    std::array<std::vector<Pixel>,5> layers_;
    std::vector<unsigned char> objWindow_;
    // High-resolution pictures of this engine's frame: tags per 64-byte VRAM
    // block, and per BG layer pixel.
    bool hd_=false;
    std::vector<std::uint32_t> hdBlocks_;
    std::array<std::vector<std::uint32_t>,4> hdTags_;
    std::vector<std::uint32_t> screens_,graphics_,main_,sub_,recipes_,mainRecipes_,subRecipes_,graphicsRecipes_;
    bool mainTop_=true,world_=false;
    void engine(NitroVideo&,bool,std::span<const std::uint32_t>,std::vector<std::uint32_t>&,std::vector<std::uint32_t>&);
    // Rows y0..y1 only, so bands of rows can be built in parallel.
    void background(NitroVideo&,bool,unsigned,std::uint32_t,unsigned y0,unsigned y1);
    void objects(NitroVideo&,bool,std::uint32_t,unsigned y0,unsigned y1);
    void capture(NitroVideo&,std::span<const std::uint32_t>);
};
}
