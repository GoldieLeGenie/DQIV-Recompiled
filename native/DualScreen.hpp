#pragma once
#include <array>
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
// Wide battles in the large layout (BattleLayout): the scene zoomed so that
// the tallest monster fills the room between the party's windows and the
// commands (setBattleMonsterExtent); panorama follows the battle zoom setting.
bool largeBattleFrame();
void setLargeBattle(bool value);
// Large battles draw the top screen (the party's windows) smaller, and the
// bottom screen's name plates may stand outside that screen: above it, in the
// room this frees (down to battlePlateTop()), and beside it (panorama
// battles too: their monsters reach beyond the DS screen). The band: the
// bottom screen's coordinates -battleBandSide..255+battleBandSide across,
// -battleBandRows..191 down.
inline constexpr float largeBattleTopScale=0.82f;
inline constexpr int battleBandRows=64,battleBandSide=128;
inline constexpr int battleBandWidth=256+2*battleBandSide,battleBandHeight=battleBandRows+192;
// The highest a name plate may stand on the bottom screen (0 on the DS), and
// how far beside it (0 on the DS).
int battlePlateTop();
int battlePlateSide();
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
// A point of the 16:9 battle scene (normalized device coordinates) moved to
// the DS screen pixel it falls on, as the DS rasterizer snaps its vertices:
// the parts of a monster then meet without gaps, even zoomed.
void snapBattlePoint(float& x,float& y);
// How much larger battle monsters are drawn (the battle zoom of 16:9
// battles; 1 otherwise): their sizes on screen grow by this.
double battleSpriteScale();
// The battle zoom of the frame whose monster screen positions the game reads
// now (they are projected a frame earlier).
double battlePositionZoom();
// Its shift (y' = zoom y + shift, DS bottom screen pixels).
double battlePositionShift();
// The largest battle zoom at which a monster of this size stays clear of the
// windows, standing where battle monsters stand: its top height * zoom +
// offset above its feet. Only the monsters' sizes count, not where the
// moving battle camera shows them (that would make the zoom pump). 1 at least.
double battleZoomToFit(double height,double offset);
// The largest zoom every monster of the battle fits at (the battle zoom
// setting stays the upper bound); 0 lets the setting apply alone.
void setBattleZoomFit(double zoom);
// The largest zoom keeping the monsters and their plates within the band
// around the DS bottom screen (large battles); 0: not known this frame (the
// last stays).
void setBattleZoomAcross(double zoom);
// The height (DS pixels) of the battle's tallest monster above its feet; 0
// keeps the last one (once the monsters are gone).
void setBattleMonsterExtent(double extent);
// Large battles: where the monsters stand on the DS bottom screen, unzoomed:
// the highest of their tops and the lowest of their feet (only ever grows
// during a battle).
void setBattleMonsterSpan(double top,double bottom);
// Converts a position in the unified picture (x in canvas pixels, y in 0..383)
// to coordinates on one DS screen (0 top, 1 bottom).
void canvasToScreen(float x,float y,unsigned screen,int& sx,int& sy);
}
