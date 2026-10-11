#include "DualScreen.hpp"
#include "Settings.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>

namespace pc {
namespace {bool unified=false,wideBattle=false,wideField=false,bothScreens=false;unsigned layer=0;float stretch=1;bool panels=false,start=false,decal=false;int focus=-1;}
bool spriteDecal() {return decal;}
void setSpriteDecal(bool value) {decal=value;}
bool startScreen() {return start;}
void setStartScreen(bool value) {start=value;}
bool panelLayout() {return panels;}
void setPanelLayout(bool value) {panels=value;}
int panelFocus() {return focus;}
void setPanelFocus(int value) {focus=value;}
DualScreenSettings& dualScreen() {static DualScreenSettings settings;return settings;}
bool unifiedFrame() {return unified;}
void setUnifiedFrame(bool value) {unified=value;}
bool wideFieldFrame() {return wideField;}
void setWideFieldFrame(bool value) {wideField=value;}
int fieldExtendX() {return static_cast<int>(std::ceil((dualScreen().canvasWidth-256)/2));}
unsigned drawLayer() {return layer;}
void setDrawLayer(unsigned value) {layer=value;}
bool spritesOnBothScreens() {return bothScreens;}
void setSpritesOnBothScreens(bool value) {bothScreens=value;}
float stretchX() {return stretch;}
void setStretchX(float value) {stretch=value;}
// Three quarters of the stretch to the canvas width: wide enough for the
// opening's camera pans, still close to the original proportions of clouds.
float backdropStretch() {return static_cast<float>(std::max(1.0,0.75*dualScreen().canvasWidth/256.0));}
bool wideBattleFrame() {return wideBattle;}
void setWideBattleFrame(bool value) {wideBattle=value;}
namespace {std::int32_t originalProjection[4][4],extendedProjection[4][4];bool battleProjection=false;}
void extendProjection(std::int32_t (&m)[4][4]) {
    std::memcpy(originalProjection,m,sizeof originalProjection);
    // x' = x * 256 / width; y' = (y - w) / 2: the bottom camera's view lands in
    // the lower half and its continuation fills the upper half.
    const double xScale=256.0/dualScreen().canvasWidth;
    for(auto& row:m) {
        row[0]=static_cast<std::int32_t>(std::lround(row[0]*xScale));
        row[1]=static_cast<std::int32_t>(std::lround((double(row[1])-row[3])/2));
    }
    std::memcpy(extendedProjection,m,sizeof extendedProjection);
    battleProjection=wideBattleFrame();
}
}
// Screen positions of battle actors (names, cursors, target marks) are 2D
// placed on the DS screen: they come from the projection as the DS has it.
extern "C" const std::int32_t* pcScreenProjection(const std::int32_t* current) {
    if(pc::battleProjection&&pc::wideBattleFrame()&&std::memcmp(current,pc::extendedProjection,sizeof pc::extendedProjection)==0)
        return &pc::originalProjection[0][0];
    return current;
}
namespace pc {
void canvasToScreen(float x,float y,unsigned screen,int& sx,int& sy) {
    sx=static_cast<int>(std::floor(x-(dualScreen().canvasWidth-256)/2));
    sy=static_cast<int>(std::floor(screen?y-192:y));
}
namespace {
// The world map draws a tile per 16 pixels: farther out than 70 % it costs
// too much to keep 60 frames per second.
constexpr int fieldZoomMin=70;
float fieldZoom() {return wideFieldFrame()?std::clamp(gameSettings().cameraZoom,fieldZoomMin,cameraZoomMax)/100.f:1.f;}
}
void zoomFieldRect(int& left,int& top,int& right,int& bottom) {
    const float zoom=fieldZoom();
    if(zoom==1.f)return;
    const float cx=(left+right)*0.5f,cy=(top+bottom)*0.5f,hw=(right-left)*0.5f/zoom,hh=(bottom-top)*0.5f/zoom;
    left=int(std::lround(cx-hw));right=int(std::lround(cx+hw));top=int(std::lround(cy-hh));bottom=int(std::lround(cy+hh));
}
int fieldZoomExtraX() {
    const float zoom=fieldZoom(),half=128.f+float(fieldExtendX());
    return zoom<1.f?int(std::ceil(half/zoom-half)):0;
}
int fieldZoomExtraY() {
    const float zoom=fieldZoom(),half=96.f+float(fieldExtendY);
    return zoom<1.f?int(std::ceil(half/zoom-half)):0;
}
}
