#pragma once
// PC presentation of the DS two-screen 3D mode as one continuous picture.
// The game normally renders the bottom screen on odd frames and the top screen
// on even frames, each with its own camera, and shows the other screen from a
// capture. In unified mode every frame is rendered once through the bottom
// camera with its frustum continued over the top screen (and optionally widened),
// so both screens are drawn from the same moment at 60 frames per second.
#include <cstdint>

namespace pc {
struct DualScreenSettings {
    bool unified=true;
    // Canvas width in DS pixels for a 384-pixel-high picture (256 = DS width).
    float canvasWidth=256;
};
DualScreenSettings& dualScreen();
// True while the frame being built uses the unified projection.
bool unifiedFrame();
void setUnifiedFrame(bool);
// Battles: the single 3D screen (bottom) continued over the top screen and
// the canvas width; the top screen's backdrop shows the 3D scene.
bool wideBattleFrame();
void setWideBattleFrame(bool);
// World map: the field is drawn every frame over the whole canvas, centred on
// the bottom screen's view; the top screen's map becomes an inset.
bool wideFieldFrame();
void setWideFieldFrame(bool);
// Extra DS pixels the field view covers on each side and above/below the
// bottom screen, in wide field frames.
int fieldExtendX();
constexpr int fieldExtendY=96;
// World map zoom (camera settings) in wide field frames: the view rectangle
// (DS pixels) scaled about its centre, and the extra DS pixels it then shows
// beyond fieldExtendX / fieldExtendY on each side.
void zoomFieldRect(int& left,int& top,int& right,int& bottom);
int fieldZoomExtraX();
int fieldZoomExtraY();
// Scenes without a world (logos, casino, book, file menus) present their two
// screens side by side, or one of them alone when the other stays empty.
bool panelLayout();
void setPanelLayout(bool);
// The adventure log screen is showing (the settings are offered there).
bool startScreen();
void setStartScreen(bool);
// The screen such a scene shows alone (0 top, 1 bottom), or -1 to decide by content.
int panelFocus();
void setPanelFocus(int);
// Layer of the triangles being drawn: 0 scene, 1 the top screen's inset map.
unsigned drawLayer();
void setDrawLayer(unsigned);
// Sprites drawn while set ignore which screen the frame is for.
bool spritesOnBothScreens();
void setSpritesOnBothScreens(bool);
// 2D sprites drawn while set pass the depth test over equal depths (decal).
bool spriteDecal();
void setSpriteDecal(bool);
// Horizontal stretch of the perspective geometry drawn while set (1: none),
// so a backdrop made for the DS screen's width can cover the canvas.
float stretchX();
void setStretchX(float);
// The stretch that suits such backdrops on the current canvas.
float backdropStretch();
class StretchScope {
public:
    explicit StretchScope(float stretch):previous_(stretchX()) {setStretchX(stretch);}
    ~StretchScope() {setStretchX(previous_);}
    StretchScope(const StretchScope&)=delete;
    StretchScope& operator=(const StretchScope&)=delete;
private:
    float previous_;
};
// Continues a DS projection (row-vector MtxFx44, clip = v * m) built for the
// bottom screen over both screens and the canvas width.
void extendProjection(std::int32_t (&m)[4][4]);
// Converts a position in the unified picture (x in canvas pixels, y in 0..383)
// to coordinates on one DS screen (0 top, 1 bottom).
void canvasToScreen(float x,float y,unsigned screen,int& sx,int& sy);
}
