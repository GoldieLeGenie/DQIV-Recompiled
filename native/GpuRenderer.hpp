#pragma once
#include "GxRasterizer.hpp"
#include "Texture.hpp"
#include <array>
#include <cstdint>
#include <span>
#include <unordered_map>
#include <vector>

namespace pc {
using GpuMatrix=std::array<float,16>; // row-major, applied to column vectors
GpuMatrix gpuIdentity();
GpuMatrix gpuMultiply(const GpuMatrix&,const GpuMatrix&);
// Maps the DS screen's normalized device square to a rectangle of the target,
// both given in the target's normalized device coordinates.
GpuMatrix gpuPlacement(float left,float top,float right,float bottom);

// How the frame's clip coordinates reach the high-resolution 3D target.
// Perspective triangles use `world`, the others (2D overlays drawn with an
// orthographic projection) use `screen`. Viewports are applied beforehand.
// Untextured orthographic triangles spanning the whole screen width (fades,
// flashes) use `screenWide`, so they can fill a canvas wider than the DS screen.
struct GpuPlacement {
    GpuMatrix world=gpuIdentity(),screen=gpuIdentity(),screenWide=gpuIdentity();
};
// Triangles drawn together into one 3D frame.
struct GpuBatch {
    std::span<const GxTriangle> triangles;
    GpuPlacement placement;
    bool perspective=true,orthographic=true; // which kinds of triangle to include
    int layer=-1; // only triangles of this GxTriangle::layer, when not negative
    // When not negative: the DS screen (0 top, 1 bottom) whose captured picture
    // the direct-colour 256 x 256 textures hold (screen effects).
    // They sample the kept world picture (keepWorld) at that screen's place
    // instead, so the effect covers the whole canvas.
    int worldScreen=-1;
};
// Fog of `upper` (when set) above `split` (fraction of the target height from
// the bottom), the frame's own fog below, blended over +-`band`.
struct GpuFogSplit {
    const GxRasterState* upper=nullptr;
    float split=0.5f,band=0;
};
// Where the 3D targets and the two DS screens sit on the presented canvas.
struct GpuLayout {
    using Rect=std::array<float,4>; // x, y, width, height in DS pixels; empty when unused
    // Recipe rows: both screens (384), then the inset screen's (192).
    static constexpr unsigned recipeRows=704;
    Rect inset{0,0,0,0}; // the top screen shown small, with 3D target 1
    float insetAlpha=1;
    float canvasWidth=256,canvasHeight=384;
    std::array<Rect,2> threeRects{Rect{0,0,256,192},Rect{0,0,0,0}};
    bool sides=false; // canvas columns beside the screens show 3D target 0
    bool backdrop3D=false; // pixels showing only a screen's backdrop show 3D target 0
    bool threeScreenBottom=true; // the screen whose engine draws the 3D (for its fades)
    // When `placed`: where each DS screen (0 top, 1 bottom) is drawn, scaled,
    // instead of stacked in the canvas centre; a zero width hides it.
    bool placed=false;
    std::array<Rect,2> screens{Rect{0,0,0,0},Rect{0,0,0,0}};
    // Large battles: the top screen drawn at this scale at the canvas top, the
    // 3D around it. Wide battles: recipe rows 384.. hold the band around the
    // bottom screen (its plates above and beside it).
    float topScale=1;
    bool band=false;
};
struct GpuSettings {
    // Highest supersampling factor; lowered automatically while the GPU cannot
    // keep 60 frames per second (autoQuality).
    int supersample=4;
    bool autoQuality=true;
    bool smoothTextures=true;
    // Pixel-art upscaling of textures before filtering: 1, 2 or 4 (Scale2x
    // passes). 1 keeps the DS pixels exactly (textures not smoothed).
    int textureScale=4;
    // The time a frame may take on the GPU (1 / frame rate), for autoQuality.
    double frameMilliseconds=1000.0/60;
};

// OpenGL 3.3 renderer for the DS 3D engine output and the final composition.
// Requires a current context with entry points loaded (gl::load).
class GpuRenderer {
public:
    GpuRenderer();
    ~GpuRenderer();
    GpuRenderer(const GpuRenderer&)=delete;
    GpuRenderer& operator=(const GpuRenderer&)=delete;
    GpuSettings settings;
    // Set while drawing an interpolated frame: its textures are this frame's
    // (no new uploads or upscaling then).
    bool betweenFrame=false;
    // The composition recipes changed since the last present: they are uploaded
    // again (frames between two game frames reuse them).
    bool recipesChanged=true;
    // Renders into 3D target `slot` (0 or 1), kept until it is rendered again.
    void render3D(std::span<const GpuBatch>,const GxRasterState&,GxTextures&,const GxRenderOptions&,int width,int height,
        const GpuFogSplit& fog={},unsigned slot=0);
    // The last rendered 3D target reduced to 256 x 192, straight ARGB as GxRasterizer
    // outputs. Unless `immediate`, the result is the previous call's frame: the
    // read completes in the background instead of stalling the GPU.
    // `source`: the part of the target to read (x, y, width, height as fractions, y from the top).
    // `key` separates alternating screens, each getting its own previous frame.
    void readNative(std::vector<std::uint32_t>& pixels,bool immediate=false,const std::array<float,4>& source={0,0,1,1},unsigned key=0);
    // Recipes: Display::recipes() layout (3 planes of 256 x 384).
    void present(const GpuLayout&,std::span<const std::uint32_t> recipes,int windowWidth,int windowHeight,bool stretch);
    // Keeps a copy of 3D target 0 (a world picture over the whole canvas) for
    // GpuBatch::worldScreen.
    void keepWorld();
    // Last presented frame, top-down ARGB (for screenshots).
    std::vector<std::uint32_t> readPresented(int& width,int& height);
    int supersample() const; // the factor in use
    // Draws an interface picture (premultiplied ARGB, top-down) at window pixel
    // (x, y) over the presented frame; `changed` uploads it again.
    void drawOverlay(unsigned id,std::span<const std::uint32_t> pixels,int width,int height,bool changed,
        int x,int y,int windowWidth,int windowHeight);
private:
    bool uploadedSides_=false; // the side columns uploaded with the recipes
    struct Target {unsigned framebuffer=0,color=0,attributes=0,depth=0;int width=0,height=0;};
    struct GpuTexture {unsigned name=0;std::uint64_t hash=0,generation=~0ull,lastUse=0;int width=0,height=0;};
    void resize(Target&,int width,int height,bool attributes,bool depth);
    void release(Target&);
    // GPU time of whole frames (first render to presentation), for autoQuality.
    void beginFrameTiming();
    void endFrameTiming();
    unsigned threeColor(unsigned slot) const; // a 3D target's final colour texture
    unsigned upload(const struct Texture&,std::uint64_t key);
    void finishUpload(GpuTexture&,const std::vector<std::uint32_t>& premultiplied,int width,int height,std::uint64_t hash,
        const struct Texture&,bool mipmaps);
    bool anisotropyChecked_=false,anisotropy_=false;
    void prepareTextures(std::span<const GpuBatch>,GxTextures&,const GxRenderOptions&);
    // Uploaded textures are dropped when the upscaling changes (settings).
    int uploadedScale_=0;
    void dropTextures();
    void upgradeTextures();
    // Textures uploaded without upscaling, waiting to be upscaled.
    struct Upgrade {std::uint64_t key;Texture texture;std::uint64_t hash;};
    std::vector<Upgrade> upgrades_;
    unsigned program_=0,fogProgram_=0,composeProgram_=0,reduceProgram_=0,overlayProgram_=0;
    struct OverlayPicture {unsigned name=0;int width=0,height=0;};
    std::unordered_map<unsigned,OverlayPicture> overlays_;
    unsigned vertexArray_=0,vertexBuffer_=0,emptyArray_=0;
    unsigned recipeTextures_[4]{},sideColumns_=0;
    unsigned hdAtlas_=0; // HdPictures cells, upscaled and premultiplied
    Target scene_[2],native_,output_,fogged_[2],world_;
    unsigned lastSlot_=0,readBuffers_[2]{};
    bool readPending_[2]{},fogApplied_[2]{};
    unsigned frameQueries_[4]{},queryHead_=0,queriesPending_=0;
    bool queryActive_=false;
    double gpuTime_=0; // nanoseconds summed over gpuSamples_ frames
    unsigned gpuSamples_=0,quietWindows_=0,busyWindows_=0;
    int autoSupersample_=2;
    std::uint64_t renders_=0;
    std::unordered_map<std::uint64_t,GpuTexture> textures_;
    std::unordered_map<unsigned,unsigned> samplingState_; // texture name -> its sampling parameters + 1
};
}
