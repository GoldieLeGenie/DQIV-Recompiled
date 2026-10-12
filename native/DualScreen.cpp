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
namespace {bool largeBattle=false;}
bool largeBattleFrame() {return wideBattle&&largeBattle;}
void setLargeBattle(bool value) {largeBattle=value;}
// Plates snap to rows 20 n + 8 (BattleMonsterNamePlate): the highest under
// the smaller top screen (its windows end at 190 * 0.82, 36 rows above).
int battlePlateTop() {return largeBattleFrame()?-32:0;}
int battlePlateSide() {return wideBattleFrame()?battleBandSide:0;}
void setWideBattleFrame(bool value) {wideBattle=value;}
namespace {std::int32_t originalProjection[4][4],extendedProjection[4][4];bool battleProjection=false;
// The wide battle projection from the DS one: x' = sx x, y' = sy y + oy.
double battleScaleX=1,battleScaleY=1,battleOffsetY=0,battleZoom=1,battleShift=0,previousBattleZoom=1,battleFit=0,battleExtent=0;
// A battle's first frames take its zoom at once (no zoom-in as it opens).
bool battleZoomSettled=false;
double battleAcross=0,previousBattleShift=0;
// The monsters' tops and feet, measured (large battles).
double battleSpanTop=0,battleSpanBottom=0;bool battleSpan=false;
// The anchor of the zoom (the monsters' feet) and how far zooming lifts the
// scene, in DS bottom screen pixels; the room the windows leave the monsters.
constexpr double feetAnchor=-0.25,zoomLift=0.3,anchorY=(1-feetAnchor)*96,liftY=zoomLift*96;
constexpr double topLimit=30;}
void extendProjection(std::int32_t (&m)[4][4]) {
    std::memcpy(originalProjection,m,sizeof originalProjection);
    // x' = x * 256 / width; y' = (y - w) / 2: the bottom camera's view lands in
    // the lower half and its continuation fills the upper half.
    const double xScale=256.0/dualScreen().canvasWidth;
    for(auto& row:m) {
        row[0]=static_cast<std::int32_t>(std::lround(row[0]*xScale));
        row[1]=static_cast<std::int32_t>(std::lround((double(row[1])-row[3])/2));
    }
    if(!wideBattleFrame()) {battleZoomSettled=false;battleFit=0;battleExtent=0;battleAcross=0;battleSpan=false;}
    battleProjection=wideBattleFrame();
    if(battleProjection) {
        // The scene zoomed by z and shifted down by t (DS bottom screen pixels):
        // y' = z y + t, x' = 128 + z (x - 128). The monsters' names and cursors
        // are placed through the original projection, changed the same way.
        double zoom=1,shift=0;
        if(largeBattleFrame()) {
            // Large: the tallest monster with its name fills the room between
            // the party's windows and the commands (up to twice its size), never
            // reaching either: the tallest bosses get a little smaller than on
            // the DS, where their legs go behind the message window.
            // The windows end at the smaller top screen's bottom (its windows'
            // last row, 190); the commands' portrait begins at 116.
            constexpr double windowsBottom=190.0*largeBattleTopScale-192+6,commandsTop=113,plateRoom=22,smallest=0.7,largest=2;
            // Where the monsters stand once measured (some stand lower, in
            // front); until then, the tallest one's height above the feet.
            const double bottom=battleSpan?battleSpanBottom:anchorY;
            const double plateTop=(battleSpan?battleSpanTop:anchorY-(battleExtent>0?battleExtent:60))-plateRoom;
            zoom=std::clamp((commandsTop-windowsBottom)/(bottom-plateTop),smallest,largest);
            if(battleAcross>0)zoom=std::max(std::min(1.0,zoom),std::min(zoom,battleAcross));
            // The feet stay where the DS has them when they can; clear of the
            // commands first.
            shift=std::min(std::max(anchorY*(1-zoom),windowsBottom-zoom*plateTop),commandsTop-zoom*bottom);
        } else {
            // Panorama: the battle zoom setting, around the monsters' feet, the
            // scene lifted as it grows (clear of the portrait beside the
            // commands); bosses get a smaller zoom (setBattleZoomFit).
            zoom=std::clamp(gameSettings().battleZoom,100,200)/100.0;
            if(battleFit>0)zoom=std::max(1.0,std::min(zoom,battleFit));
            if(battleAcross>0)zoom=std::max(1.0,std::min(zoom,battleAcross));
            shift=anchorY*(1-zoom)-(zoom-1)*liftY;
        }
        // Eased towards, taken at once as the battle opens.
        const bool known=largeBattleFrame()?battleExtent>0:battleFit>0;
        if(battleZoomSettled) {
            zoom=std::abs(zoom-battleZoom)<0.02?zoom:battleZoom+(zoom>battleZoom?0.02:-0.02);
            shift=std::abs(shift-battleShift)<1?shift:battleShift+(shift>battleShift?1:-1);
        }
        if(known)battleZoomSettled=true;
        previousBattleZoom=battleZoom;previousBattleShift=battleShift;battleZoom=zoom;battleShift=shift;
        // Original NDC: y' = z y + (1 - z - t/96); wide: y' = z y - t/192.
        battleScaleX=xScale*zoom;battleScaleY=zoom/2;battleOffsetY=-zoom/2-shift/192;
        for(unsigned r=0;r<4;++r) {
            m[r][0]=static_cast<std::int32_t>(std::lround(m[r][0]*zoom));
            m[r][1]=static_cast<std::int32_t>(std::lround(m[r][1]*zoom-shift/192*m[r][3]));
            originalProjection[r][0]=static_cast<std::int32_t>(std::lround(originalProjection[r][0]*zoom));
            originalProjection[r][1]=static_cast<std::int32_t>(std::lround(originalProjection[r][1]*zoom+(1-zoom-shift/96)*originalProjection[r][3]));
        }
    }
    std::memcpy(extendedProjection,m,sizeof extendedProjection);
}
double battleSpriteScale() {return battleProjection&&wideBattleFrame()?battleZoom:1.0;}
double battlePositionZoom() {return battleProjection&&wideBattleFrame()?previousBattleZoom:1.0;}
double battlePositionShift() {return battleProjection&&wideBattleFrame()?previousBattleShift:0.0;}
void setBattleZoomFit(double zoom) {battleFit=zoom;}
// Only ever lower during a battle: the measure follows the zoom by a frame,
// and a zoom both ways would make the scene pump.
void setBattleZoomAcross(double zoom) {if(zoom>0&&(battleAcross==0||zoom<battleAcross))battleAcross=zoom;}
void setBattleMonsterExtent(double extent) {battleExtent=extent;}
void setBattleMonsterSpan(double top,double bottom) {
    if(!battleSpan) {battleSpanTop=top;battleSpanBottom=bottom;battleSpan=true;}
    else {battleSpanTop=std::min(battleSpanTop,top);battleSpanBottom=std::max(battleSpanBottom,bottom);}
}
double battleZoomToFit(double height,double offset) {
    // The feet stay at the zoom's anchor; the scene rises by (z - 1) liftY.
    // top(z) = anchorY - (z - 1) liftY - z height - offset >= topLimit.
    const double top1=anchorY-height-offset;
    const double limit=std::min(topLimit,top1);
    const double fit=(anchorY+liftY-offset-limit)/(liftY+height);
    return std::max(1.0,fit);
}
void snapBattlePoint(float& x,float& y) {
    if(!battleProjection||!wideBattleFrame())return;
    // Back to the DS screen (256 x 192 pixels), to the pixel, then out again.
    const double dx=std::round((x/battleScaleX+1)*128),dy=std::round((1-(y-battleOffsetY)/battleScaleY)*96);
    x=static_cast<float>((dx/128-1)*battleScaleX);
    y=static_cast<float>((1-dy/96)*battleScaleY+battleOffsetY);
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
