#pragma once
#include "Display.hpp"
#include "GameWindow.hpp"
#include "GxGeometry.hpp"
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pc {
// Turns each DS frame into the presented picture: the 3D engine output at the
// window's resolution, finished with the display recipes. In the two-screen
// 3D mode (DualScreen.hpp) both screens are joined into one picture: the 3D
// scene comes from the current frame, and each screen keeps its latest 2D
// layers and orthographic overlays, which the game refreshes on alternate frames.
class FramePresenter {
public:
    FramePresenter(GameWindow&,Display&,GxTextures&);
    // After the game's buffer swap. `native3d` asks for the 3D frame at the
    // hardware size (display capture, diagnostics).
    void frame(NitroVideo&,const GxGeometry&,const GxRasterState&,const GxRenderOptions&,bool native3d);
    void present();
    // Draws the 3D scene `alpha` (0..1) of the way from the previous frame to
    // this one, then presents it (frame rates above 60). False when the two
    // frames cannot be interpolated.
    bool presentBetween(float alpha);
    const std::vector<std::uint32_t>& native3D() const {return native3d_;}
    const std::vector<std::uint32_t>& recipes() const {return recipes_;}
private:
    GameWindow& window_;
    Display& display_;
    GxTextures& textures_;
    GpuLayout layout_;
    std::vector<std::uint32_t> native3d_,recipes_;
    // Per screen (0 top, 1 bottom) in the joined mode.
    std::array<std::vector<GxTriangle>,2> overlays_;
    std::array<std::vector<std::uint32_t>,2> screenRecipes_;
    std::array<GxRasterState,2> fog_;
    std::array<bool,2> seen_{};
    // menu: a menu over the world, which stays as it was last drawn (3D target 0).
    // effect: a screen effect over the kept world picture, across the canvas.
    enum class Mode {single,split,joined,battle,field,menu,effect} mode_=Mode::single;
    bool effectWorld_=false; // the world picture was kept for screen effects
    unsigned held_=0; // frames the last picture was held (leaving a menu)
    unsigned insetFrames_=0; // field frames since the inset map was first drawn
    bool presentable_=false,seen3d_=false;
    Mode worldMode_=Mode::single; // the last mode other than menu
    bool worldKept_=false; // 3D target 0 holds the world over the whole canvas (joined, field)
    // Panel layout (DualScreen panelLayout): current rectangles, moving towards
    // their target, and how many updates each screen has stayed empty.
    std::array<GpuLayout::Rect,2> panels_{};
    std::array<unsigned,2> empty_{};
    bool panelsShown_=false;
    void placePanels(float width);
    // The last two frames' 3D batches, for presentBetween.
    struct Interpolation {
        bool valid=false;
        int mode=-1;
        std::vector<std::vector<GxTriangle>> previous,current,work;
        std::vector<int> pairs; // per current triangle: its previous one, or -1
        std::vector<char> used;
        std::vector<float> moves;
        std::vector<int> left,right; // scene triangles of the previous and current frame
        std::vector<std::array<int,3>> aligned;
        std::vector<char> movedTriangles;
        std::unordered_map<std::uint64_t,std::array<float,4>> corners; // current corner -> drawn place
        std::unordered_multimap<std::uint64_t,int> still; // unchanged previous triangles
        std::unordered_set<std::uint64_t> unsteady; // battle sprite sheets changing picture
        std::vector<std::vector<int>> grid;
        std::vector<GpuBatch> batches;
        GxRasterState state;
        GxRenderOptions options;
        int width=0,height=0;
        GpuFogSplit fog;
        unsigned slot=0;
    } interpolation_;
    void keepForInterpolation(int mode,const std::vector<GpuBatch>& batches,const GxRasterState& state,
        const GxRenderOptions& options,int width,int height,const GpuFogSplit& fog,unsigned slot);
    static bool emptyScreen(std::span<const std::uint32_t> screenRecipes);
    // Copies one screen's recipes (3 planes of 256 x 192) to `row` of the presented ones.
    void place(std::span<const std::uint32_t> screenRecipes,unsigned row);
};
}
