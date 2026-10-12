#include "FramePresenter.hpp"
#include "DualScreen.hpp"
#include "NitroVideo.hpp"
#include "Profile.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace pc {
namespace {
constexpr unsigned screenPixels=256*192,planePixels=256*GpuLayout::recipeRows;
void copyFog(GxRasterState& target,const GxRasterState& source) {
    target.fogColor=source.fogColor;target.fogOffset=source.fogOffset;target.fogEnabled=source.fogEnabled;
    target.fogAlphaOnly=source.fogAlphaOnly;target.fogSlope=source.fogSlope;target.fogTable=source.fogTable;
}
}
FramePresenter::FramePresenter(GameWindow& window,Display& display,GxTextures& textures):
    window_(window),display_(display),textures_(textures),recipes_(planePixels*Display::recipePlanes) {
    window_.setInterpolator([this](float alpha){return presentBetween(alpha);});
}
void FramePresenter::place(std::span<const std::uint32_t> screenRecipes,unsigned row) {
    for(unsigned plane=0;plane<Display::recipePlanes;++plane)
        std::copy_n(screenRecipes.data()+plane*screenPixels,screenPixels,recipes_.data()+plane*planePixels+row*256);
}
bool FramePresenter::emptyScreen(std::span<const std::uint32_t> screenRecipes) {
    // Plane 0 of one screen: nothing but black final colours.
    for(unsigned p=0;p<screenPixels;++p)if(screenRecipes[p]!=0)return false;
    return true;
}
void FramePresenter::placePanels(float width) {
    // Side by side, as large as the canvas allows; or one screen alone, twice
    // the DS size, when the other stays empty (or the scene asks for it).
    constexpr unsigned settle=30;
    int alone=panelFocus();
    if(alone<0&&empty_[0]>=settle&&empty_[1]<settle)alone=1;
    if(alone<0&&empty_[1]>=settle&&empty_[0]<settle)alone=0;
    std::array<GpuLayout::Rect,2> target;
    if(alone>=0) {
        const float scale=std::min(width/256,2.f),w=256*scale,h=192*scale;
        target[alone]={(width-w)/2,(384-h)/2,w,h};
        target[1-alone]={width/2,192,0,0};
    } else {
        const float scale=std::min(width/512,2.f),w=256*scale,h=192*scale,x=(width-2*w)/2,y=(384-h)/2;
        target[0]={x,y,w,h};target[1]={x+w,y,w,h};
    }
    // Changes glide over a quarter of a second.
    for(unsigned s=0;s<2;++s)for(unsigned k=0;k<4;++k) {
        auto& value=panels_[s][k];
        value=panelsShown_?value+(target[s][k]-value)*0.2f:target[s][k];
        if(std::abs(target[s][k]-value)<0.05f)value=target[s][k];
    }
    panelsShown_=true;
}
void FramePresenter::frame(NitroVideo& video,const GxGeometry& geometry,const GxRasterState& state,
    const GxRenderOptions& options,bool native3d) {
    const bool mainTop=(*static_cast<unsigned short*>(video.reg(0x04000304,2))&0x8000)!=0;
    const unsigned control=*static_cast<unsigned*>(video.reg(0x04000000,4));
    const bool capturing=(*static_cast<unsigned*>(video.reg(0x04000064,4))&0x80000000u)!=0;
    // Joined: one scene through both screens (DualScreen.hpp). Field: the world
    // map over the whole canvas, the top screen's map as an inset. Battle: the
    // battle scene continued over the top screen. Split: the DS two-screen 3D
    // mode, where the main engine shows this frame's capture and the other
    // screen keeps the previous one; each screen keeps its own 3D.
    // Menu: right after the world filled the canvas, the main engine shows a
    // captured copy of it instead of 3D (Display::showsBitmapBackdrop).
    const bool menu=worldKept_&&((control>>16)&3)==1&&!capturing&&Display::showsBitmapBackdrop(video);
    // The game freezes the picture by capturing only the previous capture
    // (EVA 0), e.g. while it loads a battle into the texture memory the scene
    // still uses: the kept wide picture stays too.
    const unsigned captureControl=*static_cast<unsigned*>(video.reg(0x04000064,4));
    const bool frozen=capturing&&((captureControl>>29)&3)>=2&&(captureControl&31)==0&&((captureControl>>8)&31)!=0;
    // A menu is recognized before the part's own mode, which can last a frame
    // longer than the scene it shows.
    Mode mode=menu?Mode::menu:unifiedFrame()?Mode::joined:wideFieldFrame()?Mode::field:wideBattleFrame()?Mode::battle:
        ((control>>16)&3)==2&&capturing?Mode::split:Mode::single;
    // Screen effects texture each screen with its captured picture: right
    // after the world filled the canvas, they use that picture instead.
    if(mode==Mode::split||mode==Mode::single) {
        const auto& all=geometry.triangles();
        const bool captured=std::any_of(all.begin(),all.end(),[](const GxTriangle& t) {
            const auto p=t.material.texture;return ((p>>26)&7)==7&&((p>>20)&7)==5&&((p>>23)&7)==5;
        });
        if(worldKept_&&captured&&!effectWorld_) {window_.gpu().keepWorld();effectWorld_=true;}
        if(effectWorld_&&captured)mode=Mode::effect;
    } else effectWorld_=false;
    // Leaving a menu, the game shows its capture in the DS layout for a frame
    // before the world comes back: the last picture stays meanwhile.
    if(mode_==Mode::menu&&(mode==Mode::split||mode==Mode::single)&&worldKept_&&held_<4) {
        ++held_;
        display_.setWorldBackground(false);
        display_.frame(video,{},true);
        return;
    }
    held_=0;
    // Back from a menu to the same world, each screen's last overlays and
    // layers (backgrounds drawn on alternate frames) are still valid.
    const bool resumed=mode_==Mode::menu&&mode==worldMode_;
    if(mode!=mode_&&mode!=Mode::menu&&!resumed)seen_={};
    if(mode!=Mode::menu)worldMode_=mode;
    mode_=mode;
    const float width=dualScreen().canvasWidth,left=(width-256)/2;
    const unsigned screen=mainTop?0:1,other=1-screen;
    std::vector<GpuBatch> batches;
    GpuFogSplit fogSplit;
    GxRasterState frameState=state;
    unsigned slot=0;
    layout_=GpuLayout{};layout_.canvasWidth=width;
    float canvasWidth=0,canvasHeight=0;window_.canvasPixels(layout_,canvasWidth,canvasHeight);
    auto pixels=[&](const GpuLayout::Rect& rect,int& w,int& h) {
        w=std::max(1,int(std::lround(canvasWidth*rect[2]/layout_.canvasWidth)));
        h=std::max(1,int(std::lround(canvasHeight*rect[3]/layout_.canvasHeight)));
    };
    if(mode==Mode::joined) {
        layout_.threeRects={GpuLayout::Rect{0,0,width,384},GpuLayout::Rect{0,0,0,0}};layout_.sides=true;
        auto& overlay=overlays_[screen];overlay.clear();
        for(const auto& triangle:geometry.triangles())if(!triangle.perspective)overlay.push_back(triangle);
        fog_[screen]=state;
        const float l=(width-256)/width-1,r=(width+256)/width-1;
        auto placement=[&](unsigned s) {
            GpuPlacement p;
            p.screen=s?gpuPlacement(l,0,r,-1):gpuPlacement(l,1,r,0);
            p.screenWide=s?gpuPlacement(-1,0,1,-1):gpuPlacement(-1,1,1,0);
            return p;
        };
        batches.push_back({geometry.triangles(),placement(screen),true,true});
        if(seen_[other])batches.push_back({overlays_[other],placement(other),false,true});
        // Each screen keeps its own fog, blended across the join.
        copyFog(frameState,fog_[seen_[1]||screen==1?1:screen]);
        fogSplit={&fog_[seen_[0]||screen==0?0:screen],0.5f,12.f/384};
    } else if(mode==Mode::field) {
        // The game's field camera already covers the canvas (layer 0); the top
        // screen's map (layer 1) is kept for the inset from its own frames.
        layout_.threeRects={GpuLayout::Rect{0,0,width,384},GpuLayout::Rect{0,0,0,0}};layout_.sides=true;
        const float insetWidth=256*0.6f,insetHeight=192*0.6f;
        layout_.inset={width-insetWidth-6,6,insetWidth,insetHeight};
        const auto& triangles=geometry.triangles();
        if(!frozen&&std::any_of(triangles.begin(),triangles.end(),[](const GxTriangle& t){return t.layer!=0;})) {
            int w=0,h=0;pixels(layout_.inset,w,h);
            const GpuBatch insetBatch{triangles,{},true,true,1};
            ProfileScope timing("render3D-inset");
            window_.gpu().render3D({&insetBatch,1},state,textures_,options,w,h,{},1);
            seen3d_=true;
        }
        if(!seen3d_)layout_.inset={0,0,0,0};
        // Arriving on the field, the map takes a moment to be ready (blank,
        // then still loading its colours): it fades in afterwards.
        if(seen3d_)++insetFrames_;
        layout_.insetAlpha=std::clamp((float(insetFrames_)-30.f)/20.f,0.f,1.f);
        batches.push_back({triangles,{},true,true,0});
    } else if(mode==Mode::battle) {
        if(largeBattleFrame())layout_.topScale=largeBattleTopScale;
        layout_.band=wideBattleFrame();
        // The camera already covers the canvas; overlays stay on the main screen.
        layout_.threeRects={GpuLayout::Rect{0,0,width,384},GpuLayout::Rect{0,0,0,0}};layout_.sides=true;layout_.backdrop3D=true;
        layout_.threeScreenBottom=!mainTop;
        const float l=(width-256)/width-1,r=(width+256)/width-1;
        GpuPlacement p;
        p.screen=mainTop?gpuPlacement(l,1,r,0):gpuPlacement(l,0,r,-1);
        // Full-screen quads (fades, flashes) cover the whole scene.
        p.screenWide=gpuPlacement(-1,1,1,-1);
        batches.push_back({geometry.triangles(),p,true,true});
    } else if(mode==Mode::effect) {
        // Each screen's half of the canvas, rendered across its whole width.
        slot=screen;
        layout_.threeRects={GpuLayout::Rect{0,0,0,0},GpuLayout::Rect{0,0,0,0}};
        layout_.threeRects[slot]={0,screen*192.f,width,192};
        if(seen_[other])layout_.threeRects[other]={0,other*192.f,width,192};
        layout_.sides=true;
        GpuBatch batch{geometry.triangles(),{},true,true};batch.worldScreen=int(screen);
        batches.push_back(batch);
    } else if(mode==Mode::menu) {
        // The kept world covers the canvas; the menus' 2D layers lie over it.
        layout_.threeRects={GpuLayout::Rect{0,0,width,384},GpuLayout::Rect{0,0,0,0}};layout_.sides=true;
    } else {
        // The 3D target covers the main engine's screen.
        slot=mode==Mode::split?screen:0;
        const bool panels=panelLayout()&&width>=320;
        if(panels)placePanels(width);else panelsShown_=false;
        auto rect=[&](unsigned s){return panels?panels_[s]:GpuLayout::Rect{left,s*192.f,256,192};};
        layout_.threeRects[0]=layout_.threeRects[1]=GpuLayout::Rect{0,0,0,0};
        layout_.threeRects[slot]=rect(screen);
        if(mode==Mode::split&&seen_[other])layout_.threeRects[other]=rect(other);
        if(panels) {
            layout_.placed=true;layout_.screens=panels_;
        }
        batches.push_back({geometry.triangles(),{},true,true});
    }
    if(mode!=Mode::field&&mode!=Mode::menu) {seen3d_=false;insetFrames_=0;}
    // A hidden screen (panel layouts) still renders at a usable size.
    const auto renderRect=layout_.threeRects[slot][2]>=1?layout_.threeRects[slot]:GpuLayout::Rect{0,0,256,192};
    int targetWidth=0,targetHeight=0;pixels(renderRect,targetWidth,targetHeight);
    if(mode!=Mode::menu&&!(frozen&&worldKept_&&(mode==Mode::joined||mode==Mode::field))) {
        ProfileScope timing("render3D");
        window_.gpu().render3D(batches,frameState,textures_,options,targetWidth,targetHeight,fogSplit,slot);
        worldKept_=mode==Mode::joined||mode==Mode::field;
        // The scenes whose camera covers the canvas move smoothly between frames.
        if(mode==Mode::joined||mode==Mode::field||mode==Mode::battle)
            keepForInterpolation(int(mode),batches,frameState,options,targetWidth,targetHeight,fogSplit,slot);
        else interpolation_.valid=false,interpolation_.mode=-1;
    } else interpolation_.valid=false,interpolation_.mode=-1;
    ProfileScope captureTiming("capture+display");
    // The capture unit stores the frame for the game's later use (menu
    // backgrounds, effects): it needs this screen's part of the 3D target.
    std::array<float,4> source{0,0,1,1};
    const float column=(width-256)/2/width,span=256/width;
    if(mode==Mode::joined)source={column,screen*0.5f,span,0.5f};
    else if(mode==Mode::battle)source={column,mainTop?0.f:0.5f,span,0.5f};
    else if(mode==Mode::field)source={column,fieldExtendY/384.f,span,0.5f};
    else if(mode==Mode::effect)source={column,0,span,1};
    const bool fieldInset=mode==Mode::field&&screen==0; // the map screen is drawn in the inset
    if(native3d)window_.gpu().readNative(native3d_,true,source);
    else if(capturing&&!fieldInset)window_.gpu().readNative(native3d_,false,source,screen);
    else native3d_.clear();
    display_.setWorldBackground(mode==Mode::menu);
    display_.setBand(mode==Mode::battle&&wideBattleFrame());
    display_.frame(video,native3d_,mode==Mode::single||mode==Mode::battle||mode==Mode::menu);
    if(mode==Mode::joined||mode==Mode::split||mode==Mode::field||mode==Mode::effect) {
        // The main engine's graphics, before its display mode selects the capture.
        const auto main=display_.mainGraphicsRecipes();
        screenRecipes_[screen].assign(main.begin(),main.end());seen_[screen]=true;
        empty_[screen]=emptyScreen(main)?empty_[screen]+1:0;
        const auto& top=screenRecipes_[seen_[0]?0:screen];
        const auto& bottom=screenRecipes_[seen_[1]?1:screen];
        if(mode==Mode::field) {
            // Above the bottom screen only the field shows, with its fades.
            place(bottom,192);place(top,384);
            for(unsigned y=0;y<192;++y)for(unsigned x=0;x<256;++x) {
                recipes_[y*256+x]=1u<<24;recipes_[planePixels+y*256+x]=0;
                recipes_[2*planePixels+y*256+x]=bottom[2*screenPixels+x]&0xff000000u;recipes_[3*planePixels+y*256+x]=0;
            }
        } else {
            place(top,0);place(bottom,192);
            if(mode==Mode::joined) {
                // The halves come from consecutive frames: during fades their master
                // brightness differs by a step, which would show the join. Both take
                // this frame's.
                const std::uint32_t code=main[2*screenPixels]&0xff000000u;
                auto* plane=recipes_.data()+2*planePixels+other*192*256;
                for(unsigned p=0;p<screenPixels;++p)plane[p]=(plane[p]&0x00ffffffu)|code;
            }
        }
    } else {
        const auto all=display_.recipes();
        for(unsigned plane=0;plane<Display::recipePlanes;++plane)
            std::copy_n(all.data()+plane*screenPixels*2,screenPixels*2,recipes_.data()+plane*planePixels);
        for(unsigned s=0;s<2;++s)empty_[s]=emptyScreen(all.subspan(s*screenPixels,screenPixels))?empty_[s]+1:0;
        if(layout_.band) {
            // The plates above the bottom screen: their colour, or the scene
            // (the bare backdrop), with the bottom screen's master brightness.
            const auto band=display_.band();
            const std::uint32_t master=all[2*screenPixels*2+192*256]&0xff000000u;
            // Above the screen: rows 384..447; beside it: rows 448..703, the
            // left side in columns 0..127, the right one in 128..255.
            const auto put=[&](unsigned at,std::uint32_t colour) {
                recipes_[at]=colour>>24?colour&0x00ffffffu:0;
                recipes_[planePixels+at]=colour>>24?0:8u<<24;
                recipes_[2*planePixels+at]=master;recipes_[3*planePixels+at]=0;
            };
            if(band.size()==std::size_t(battleBandWidth)*battleBandHeight) {
                for(int y=0;y<battleBandHeight;++y) {
                    const auto row=band.subspan(std::size_t(y)*battleBandWidth,battleBandWidth);
                    if(y<battleBandRows)
                        for(int x=0;x<256;++x)put((384+y)*256+x,row[battleBandSide+x]);
                    for(int x=0;x<battleBandSide;++x) {
                        put((448+y)*256+x,row[x]);
                        put((448+y)*256+battleBandSide+x,row[battleBandSide+256+x]);
                    }
                }
            }
        }
    }
    presentable_=true;
}
void FramePresenter::present() {
    ProfileScope timing("present");
    // Interpolated frames follow until the next one (GameWindow::pace).
    if(interpolation_.valid&&window_.interpolating())return;
    if(presentable_)window_.present(layout_,recipes_);
}
void FramePresenter::keepForInterpolation(int mode,const std::vector<GpuBatch>& batches,const GxRasterState& state,
    const GxRenderOptions& options,int width,int height,const GpuFogSplit& fog,unsigned slot) {
    auto& kept=interpolation_;
    const bool same=kept.mode==mode&&kept.width==width&&kept.height==height&&kept.slot==slot&&kept.current.size()==batches.size();
    std::swap(kept.previous,kept.current);
    kept.current.resize(batches.size());
    for(std::size_t b=0;b<batches.size();++b)kept.current[b].assign(batches[b].triangles.begin(),batches[b].triangles.end());
    kept.valid=same&&window_.interpolating();
    kept.mode=mode;kept.batches=batches;kept.state=state;kept.options=options;
    kept.width=width;kept.height=height;kept.fog=fog;kept.slot=slot;
}
bool FramePresenter::presentBetween(float alpha) {
    auto& kept=interpolation_;
    if(!kept.valid||!presentable_)return false;
    kept.work.resize(kept.current.size());
    for(std::size_t b=0;b<kept.current.size();++b) {
        const auto& from=kept.previous[b];const auto& to=kept.current[b];auto& out=kept.work[b];
        out=to;
        // The scene's triangles (perspective) come mostly in the same order from
        // frame to frame (an object may appear or vanish): each one is paired with
        // the next one of the previous frame with the same material, a few places
        // ahead at most. One that changed material (an animated or turning
        // sprite) or moved in the order (sorted sprites) is paired with the
        // nearest unpaired one of the previous frame. The 2D overlays stay.
        const auto same=[](const GxTriangle& x,const GxTriangle& y) {
            return x.material.texture==y.material.texture&&x.material.palette==y.material.palette&&
                x.material.polygon==y.material.polygon&&x.layer==y.layer;
        };
        // Screen position, steadied for corners close to or behind the eye
        // (their projection swings wildly for a small move): depths under a fifth
        // of the scene's typical one count as that.
        float depth=0;
        {
            auto& depths=kept.moves;depths.clear();
            for(const auto& t:to)if(t.perspective)depths.push_back(std::abs(t.vertices[0].clip[3]));
            if(!depths.empty()) {
                std::nth_element(depths.begin(),depths.begin()+std::ptrdiff_t(depths.size()/2),depths.end());
                depth=depths[depths.size()/2];
            }
        }
        const float nearDepth=std::max(0.5f,depth*0.2f);
        const auto ndc=[nearDepth](const GxTriangle& t,unsigned v,unsigned axis) {
            const auto& c=t.vertices[v].clip;return c[axis]/std::max(std::abs(c[3]),nearDepth);
        };
        // Distance between two triangles' corners on screen (infinite when behind the eye).
        const auto apart=[&](const GxTriangle& x,const GxTriangle& y) {
            float sum=0;
            for(unsigned v=0;v<3;++v)sum+=std::abs(ndc(x,v,0)-ndc(y,v,0))+std::abs(ndc(x,v,1)-ndc(y,v,1));
            return sum;
        };
        auto& pairs=kept.pairs;pairs.assign(out.size(),-1);
        auto& used=kept.used;used.assign(from.size(),0);
        // Triangles crossing the near plane (a corner at or behind the eye) have
        // no meaningful place on screen: they rely on the alignment alone.
        const auto crossing=[](const GxTriangle& t) {
            for(unsigned v=0;v<3;++v)if(t.vertices[v].clip[3]<=0)return true;
            return false;
        };
        // The scene triangles of both frames, aligned like a diff by their
        // picture: runs that match advance together; a triangle added or
        // removed is skipped (the nearest resynchronisation); one whose picture
        // changed (a sprite's next frame) stays paired in its run.
        // Triangles that didn't change at all are paired first: the game may
        // send a still mesh in another order, and same-picture neighbours
        // paired by order would be drawn half-way between each other.
        const auto whole=[](const GxTriangle& t) {
            std::uint64_t h=std::uint64_t(t.material.texture)*0x9e3779b97f4a7c15ull^t.material.palette^(std::uint64_t(t.material.polygon)<<32)^t.layer;
            for(const auto& v:t.vertices) {
                for(const float f:v.clip)h=(h^std::bit_cast<std::uint32_t>(f))*1099511628211ull;
                h=(h^std::bit_cast<std::uint32_t>(v.u))*1099511628211ull;h=(h^std::bit_cast<std::uint32_t>(v.v))*1099511628211ull;
            }
            return h;
        };
        auto& still=kept.still;still.clear();
        for(std::size_t i=0;i<from.size();++i)if(from[i].perspective)still.emplace(whole(from[i]),int(i));
        for(std::size_t j=0;j<out.size();++j) {
            if(!out[j].perspective)continue;
            const auto found=still.find(whole(out[j]));
            if(found==still.end())continue;
            pairs[j]=found->second;used[std::size_t(found->second)]=1;still.erase(found);
        }
        auto& left=kept.left;auto& right=kept.right;left.clear();right.clear();
        for(std::size_t i=0;i<from.size();++i)if(from[i].perspective&&!used[i])left.push_back(int(i));
        for(std::size_t j=0;j<out.size();++j)if(out[j].perspective&&pairs[j]<0)right.push_back(int(j));
        const auto key=[&](const GxTriangle& t) {
            return std::uint64_t(t.material.texture)^(std::uint64_t(t.material.palette)<<32)^(std::uint64_t(t.material.polygon)*0x9e3779b97f4a7c15ull)^t.layer;
        };
        const auto run=[&](std::size_t i,std::size_t j) {
            // Three triangles alike from (i, j) on.
            for(std::size_t k=0;k<3;++k) {
                if(i+k>=left.size()||j+k>=right.size())return k>0;
                if(key(from[std::size_t(left[i+k])])!=key(out[std::size_t(right[j+k])]))return false;
            }
            return true;
        };
        auto& aligned=kept.aligned;aligned.clear(); // (previous, current, same picture)
        {
            std::size_t i=0,j=0;
            while(i<left.size()&&j<right.size()) {
                const auto& f=from[std::size_t(left[i])];const auto& t=out[std::size_t(right[j])];
                if(key(f)==key(t)) {aligned.push_back({left[i],right[j],1});++i;++j;continue;}
                std::size_t skip=0;bool inFrom=false;
                for(std::size_t d=1;d<=48&&!skip;++d) {
                    if(run(i+d,j)) {skip=d;inFrom=true;}
                    else if(run(i,j+d)) {skip=d;inFrom=false;}
                }
                if(skip) {if(inFrom)i+=skip;else j+=skip;continue;}
                aligned.push_back({left[i],right[j],0});++i;++j;
            }
        }
        // How far the scene moved: the median distance of the aligned pairs,
        // the triangles that didn't change counting as still. Without them, a
        // still scene where identical sprites lost one (a monster of a group
        // defeated or blinking) would measure only its order-shifted pairs (each
        // matched with its neighbour) and accept them.
        auto& moves=kept.moves;moves.clear();
        for(std::size_t j=0;j<out.size();++j)if(pairs[j]>=0&&!crossing(out[j]))moves.push_back(0);
        for(const auto& a:aligned)
            if(a[2]&&!crossing(from[std::size_t(a[0])])&&!crossing(out[std::size_t(a[1])]))
                moves.push_back(apart(from[std::size_t(a[0])],out[std::size_t(a[1])]));
        float moved=0;
        if(!moves.empty()) {
            std::nth_element(moves.begin(),moves.begin()+std::ptrdiff_t(moves.size()/2),moves.end());
            moved=moves[moves.size()/2];
        }
        // Aligned pairs are kept when they moved like the scene (sprites sharing
        // a picture that changed places in the order are not).
        const float limit=std::max(0.12f,moved*3+0.05f);
        for(const auto& a:aligned) {
            const auto& f=from[std::size_t(a[0])];const auto& t=out[std::size_t(a[1])];
            const bool behind=crossing(f)||crossing(t);
            if(behind?a[2]==0:apart(f,t)>limit)continue;
            pairs[std::size_t(a[1])]=a[0];used[std::size_t(a[0])]=1;
        }
        // The others: through a grid of the unpaired previous triangles' centres.
        constexpr int cells=24;constexpr float span=4.f;
        auto cellOf=[&](const GxTriangle& t,int& cx,int& cy) {
            const float x=(ndc(t,0,0)+ndc(t,1,0)+ndc(t,2,0))/3,y=(ndc(t,0,1)+ndc(t,1,1)+ndc(t,2,1))/3;
            cx=std::clamp(int((x+span/2)/span*cells),0,cells-1);cy=std::clamp(int((y+span/2)/span*cells),0,cells-1);
        };
        auto& grid=kept.grid;grid.assign(cells*cells,{});
        for(std::size_t i=0;i<from.size();++i)if(from[i].perspective&&!used[i]&&!crossing(from[i])) {
            int cx=0,cy=0;cellOf(from[i],cx,cy);grid[std::size_t(cy*cells+cx)].push_back(int(i));
        }
        for(std::size_t j=0;j<out.size();++j) {
            if(!out[j].perspective||pairs[j]>=0||crossing(out[j]))continue;
            int cx=0,cy=0;cellOf(out[j],cx,cy);
            int best=-1;float bestApart=std::max(0.3f,limit);
            for(int y=std::max(0,cy-1);y<=std::min(cells-1,cy+1);++y)for(int x=std::max(0,cx-1);x<=std::min(cells-1,cx+1);++x)
                for(const int i:grid[std::size_t(y*cells+x)]) {
                    if(used[std::size_t(i)]||from[std::size_t(i)].layer!=out[j].layer)continue;
                    // The nearest, the same picture first.
                    const float d=apart(from[std::size_t(i)],out[j])+(same(from[std::size_t(i)],out[j])?0.f:0.1f);
                    if(d<bestApart) {bestApart=d;best=i;}
                }
            if(best>=0) {pairs[j]=best;used[std::size_t(best)]=1;}
        }
        // The same picture and the same part of it (sprites cut from one sheet
        // change parts as they animate).
        const auto picture=[&](const GxTriangle& previous,const GxTriangle& triangle) {
            bool sameUv=true;
            for(unsigned v=0;v<3;++v)sameUv&=previous.vertices[v].u==triangle.vertices[v].u&&previous.vertices[v].v==triangle.vertices[v].v;
            return same(previous,triangle)&&sameUv;
        };
        const auto sheet=[](const GxTriangle& t){return std::uint64_t(t.material.texture&0x3ff0ffffu)|(std::uint64_t(t.material.palette)<<32);};
        const bool battle=kept.mode==int(Mode::battle);
        auto& unsteady=kept.unsteady;unsteady.clear();
        if(battle)for(std::size_t j=0;j<out.size();++j)
            if(out[j].perspective&&(pairs[j]<0||!picture(from[std::size_t(pairs[j])],out[j])))unsteady.insert(sheet(out[j]));
        auto& movedTriangles=kept.movedTriangles;movedTriangles.assign(out.size(),0);
        for(std::size_t j=0;j<out.size();++j) {
            if(pairs[j]<0)continue;
            const auto& previous=from[std::size_t(pairs[j])];auto& triangle=out[j];
            // Unless it jumped (another object, a cut). Neighbours must all move
            // alike, or their shared edges would open.
            bool near=true;
            if(!crossing(previous)&&!crossing(triangle))for(unsigned v=0;v<3&&near;++v)
                near=std::abs(ndc(previous,v,0)-ndc(triangle,v,0))<0.25f+limit&&std::abs(ndc(previous,v,1)-ndc(triangle,v,1))<0.25f+limit;
            if(!near)continue;
            // The renderer drops triangles reaching past the far plane (as the
            // DS does): one dropped in either frame stays as it is, or a sliver
            // of distant ground would show between the two.
            bool past=false;
            for(unsigned v=0;v<3;++v)past|=triangle.vertices[v].clip[2]>triangle.vertices[v].clip[3];
            if(past&&!(triangle.material.polygon&(1u<<12)))continue; // GpuRenderer's own rule
            // Battle monsters are layered parts of one sheet: when any of them
            // changes picture, the whole sprite stays where it is now (moving
            // only some would put the body over the face).
            if(battle&&unsteady.count(sheet(triangle)))continue;
            movedTriangles[j]=1;
            if(picture(previous,triangle)) {
                for(unsigned v=0;v<3;++v)for(unsigned k=0;k<4;++k) {
                    auto& value=triangle.vertices[v].clip[k];
                    value=previous.vertices[v].clip[k]+(value-previous.vertices[v].clip[k])*alpha;
                }
            } else {
                // Another picture (a sprite's next frame): its own shape, only
                // moved, by its first corner's way (shared by a quad's two triangles).
                std::array<float,4> back;
                for(unsigned k=0;k<4;++k)back[k]=(triangle.vertices[0].clip[k]-previous.vertices[0].clip[k])*(1-alpha);
                for(unsigned v=0;v<3;++v)for(unsigned k=0;k<4;++k)triangle.vertices[v].clip[k]-=back[k];
            }
        }
        // A corner shared with a moved triangle moves with it, so that meshes
        // stay closed (a triangle left in place would open its edges).
        auto& corners=kept.corners;corners.clear();
        const auto cornerKey=[](const std::array<float,4>& c) {
            std::uint64_t h=1469598103934665603ull;
            for(const float f:c)h=(h^std::bit_cast<std::uint32_t>(f))*1099511628211ull;
            return h;
        };
        for(std::size_t j=0;j<out.size();++j)if(movedTriangles[j])
            for(unsigned v=0;v<3;++v)corners.emplace(cornerKey(to[j].vertices[v].clip),out[j].vertices[v].clip);
        if(!corners.empty())for(std::size_t j=0;j<out.size();++j)if(out[j].perspective&&!movedTriangles[j])
            for(unsigned v=0;v<3;++v)
                if(const auto found=corners.find(cornerKey(to[j].vertices[v].clip));found!=corners.end())out[j].vertices[v].clip=found->second;
        kept.batches[b].triangles=out;
    }
    {
        ProfileScope timing("render3D-between");
        window_.gpu().betweenFrame=true;
        window_.gpu().render3D(kept.batches,kept.state,textures_,kept.options,kept.width,kept.height,kept.fog,kept.slot);
        window_.gpu().betweenFrame=false;
    }
    window_.present(layout_,recipes_);
    return true;
}
}
