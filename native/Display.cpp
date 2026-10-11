#include "Display.hpp"
#include "HdPictures.hpp"
#include "Profile.hpp"
#include <cstdio>
#include <cstdlib>
#include <execution>
#include <algorithm>
#include <stdexcept>
namespace pc {
namespace {
constexpr unsigned count=256*192;
unsigned word(std::span<const std::uint8_t> data,unsigned offset) {
    return offset+1<data.size()?data[offset]|(unsigned(data[offset+1])<<8):0;
}
unsigned r16(NitroVideo& v,unsigned address) {return *static_cast<std::uint16_t*>(v.reg(address,2));}
unsigned r32(NitroVideo& v,unsigned address) {return *static_cast<std::uint32_t*>(v.reg(address,4));}
std::uint32_t rgb(unsigned c) {
    auto expand=[](unsigned v){return (v<<3)|(v>>2);};
    return 0xff000000|expand(c&31)<<16|expand((c>>5)&31)<<8|expand((c>>10)&31);
}
unsigned color15(std::uint32_t c) {return ((c>>19)&31)|((c>>6)&0x3e0)|((c<<7)&0x7c00);}
std::uint32_t blend(std::uint32_t a,std::uint32_t b,unsigned eva,unsigned evb,unsigned shift=4) {
    unsigned result=0;
    // Preserve the six-bit 3D output until the final display conversion.
    for(unsigned s:{0,8,16}) {
        auto v=std::min(63u,((((a>>s)&255)>>2)*eva+(((b>>s)&255)>>2)*evb)>>shift);
        result|=((v<<2)|(v>>4))<<s;
    }
    return 0xff000000|result;
}
std::uint32_t brightness(std::uint32_t c,unsigned mode,unsigned amount) {
    if(mode!=1&&mode!=2)return c;
    return mode==1?blend(c,0xffffffff,16-amount,amount):blend(c,0xff000000,16-amount,amount);
}
bool range(unsigned value,unsigned endpoints) {
    const auto start=endpoints>>8,end=endpoints&255;
    return start<=end?value>=start&&value<end:value>=start||value<end;
}
int signed28(unsigned v) {return static_cast<int>(v<<4)>>4;}
}
Display::Display():bg_(0x80000),obj_(0x40000),bgPalette_(0x8000),objPalette_(0x2000),
    objWindow_(count),screens_(count*2),graphics_(count),main_(count),sub_(count),
    recipes_(count*2*recipePlanes),mainRecipes_(count*recipePlanes),subRecipes_(count*recipePlanes),graphicsRecipes_(count*recipePlanes) {
    for(auto& layer:layers_)layer.resize(count);
    hdBlocks_.resize(bg_.size()/64);
    for(auto& tags:hdTags_)tags.resize(count);
}
void Display::background(NitroVideo& video,bool sub,unsigned layer,unsigned control,unsigned y0,unsigned y1) {
    const unsigned base=0x04000000+(sub?0x1000:0),cnt=r16(video,base+8+layer*2),mode=control&7;
    // 0=text, 1=8-bit affine tilemap, 2=extended affine, 3=large bitmap.
    unsigned type=0;
    if(layer>=2) {
        if(mode==6) {if(layer!=2||sub)return;type=3;}
        else if(layer==3)type=mode>=3?2:mode?1:0;
        else type=mode==5?2:mode==2||mode==4?1:0;
    } else if(mode==6&&layer==1)return;
    const unsigned size=cnt>>14;
    const bool bitmap=type==3||(type==2&&(cnt&128)),direct=bitmap&&type!=3&&(cnt&4);
    int width=256,height=256;
    if(type==0) {width=(size&1)?512:256;height=(size&2)?512:256;}
    else if(type==3) {width=(size&1)?1024:512;height=(size&1)?512:1024;}
    else if(bitmap) {width=size>=2?512:size==1?256:128;height=size==3?512:size?256:128;}
    else width=height=128<<size;
    const unsigned map=type==3?0:bitmap?((cnt>>8)&31)*0x4000:((cnt>>8)&31)*0x800+(sub?0:((control>>27)&7)*0x10000);
    const unsigned chars=((cnt>>2)&15)*0x4000+(sub?0:((control>>24)&7)*0x10000);
    const unsigned affine=base+0x20+(layer==3?0x10:0);
    const int pa=type?static_cast<std::int16_t>(r16(video,affine)):0,pb=type?static_cast<std::int16_t>(r16(video,affine+2)):0;
    const int pc=type?static_cast<std::int16_t>(r16(video,affine+4)):0,pd=type?static_cast<std::int16_t>(r16(video,affine+6)):0;
    const int refx=type?signed28(r32(video,affine+8)):0,refy=type?signed28(r32(video,affine+12)):0;
    const unsigned scrollx=r16(video,base+0x10+layer*4)&511,scrolly=r16(video,base+0x12+layer*4)&511;
    const unsigned mosaic=r16(video,base+0x4c),mx=(mosaic&15)+1,my=((mosaic>>4)&15)+1;
    auto byte=[&](unsigned p){return bg_[p&(sub?0x1ffff:0x7ffff)];};
    auto half=[&](unsigned p){return unsigned(byte(p))|(unsigned(byte(p+1))<<8);};
    for(unsigned y=y0;y<y1;++y)for(unsigned x=0;x<256;++x) {
        const int dx=(cnt&64)?x-x%mx:x,dy=(cnt&64)?y-y%my:y;
        int sx=type?(refx+dx*pa+dy*pb)>>8:dx+scrollx,sy=type?(refy+dx*pc+dy*pd)>>8:dy+scrolly;
        if(type && !(cnt&0x2000) && (sx<0||sy<0||sx>=width||sy>=height))continue;
        sx&=width-1;sy&=height-1;
        unsigned color=0,index=0,entry=0;
        if(bitmap) {
            if(direct) {color=half(map+(sy*width+sx)*2);if(!(color&0x8000))continue;}
            else {index=byte(map+sy*width+sx);if(!index)continue;color=word(video.palette(),(sub?0x400:0)+index*2);}
        } else {
            if(type==0) {
                const unsigned block=(sx/256)+(sy/256)*(width/256);
                entry=half(map+block*0x800+((sy/8%32)*32+(sx/8%32))*2);
            } else if(type==1)entry=byte(map+(sy/8)*(width/8)+sx/8);
            else entry=half(map+((sy/8)*(width/8)+sx/8)*2);
            unsigned tx=sx&7,ty=sy&7;
            if(type!=1) {if(entry&0x400)tx=7-tx;if(entry&0x800)ty=7-ty;}
            const bool eight=type||(cnt&128);
            const auto pixel=ty*8+tx;
            if(hd_&&type!=1&&eight&&!(entry&0xc00)) {
                // A tile of a high-resolution picture, opaque or not.
                const auto tag=hdBlocks_[((chars+(entry&1023)*64)&(sub?0x1ffff:0x7ffff))/64];
                if(tag)hdTags_[layer][y*256+x]=tag+(tx<<6)+ty;
            }
            index=byte(chars+(entry&(type==1?255:1023))*(eight?64:32)+(eight?pixel:pixel/2));
            if(!eight)index=(index>>((pixel&1)*4))&15;
            if(!index)continue;
            if(eight && type!=1 && (control&(1u<<30))) {
                const unsigned slot=layer<2?layer+((cnt&0x2000)?2:0):layer;
                color=word(bgPalette_,slot*0x2000+(entry>>12)*512+index*2);
            } else color=word(video.palette(),(sub?0x400:0)+(index+(!eight?(entry>>12)*16:0))*2);
        }
        layers_[layer][y*256+x]={rgb(color),static_cast<unsigned char>(cnt&3),31};
    }
}
void Display::objects(NitroVideo& video,bool sub,unsigned control,unsigned y0,unsigned y1) {
    constexpr unsigned widths[3][4]={{8,16,32,64},{16,32,32,64},{8,8,16,32}};
    constexpr unsigned heights[3][4]={{8,16,32,64},{8,8,16,32},{16,32,32,64}};
    const unsigned obase=sub?0x400:0,base=0x04000000+(sub?0x1000:0);
    const unsigned mosaic=r16(video,base+0x4c),mx=((mosaic>>8)&15)+1,my=((mosaic>>12)&15)+1;
    auto byte=[&](unsigned p){return obj_[p&(sub?0x1ffff:0x3ffff)];};
    for(unsigned id=0;id<128;++id) {
        const unsigned a=word(video.oam(),obase+id*8),b=word(video.oam(),obase+id*8+2),c=word(video.oam(),obase+id*8+4);
        const bool affine=(a&256)!=0;
        if((!affine&&(a&512))||(a>>14)==3)continue;
        const unsigned mode=(a>>10)&3;if(mode==2&&!(control&0x8000))continue;
        if(mode==3&&!(c>>12))continue;
        const int w=widths[a>>14][b>>14],h=heights[a>>14][b>>14],rw=w*((affine&&(a&512))?2:1),rh=h*((affine&&(a&512))?2:1);
        int pa=256,pb=0,pc=0,pd=256;
        if(affine) {
            const unsigned m=obase+((b>>9)&31)*32;
            pa=static_cast<std::int16_t>(word(video.oam(),m+6));pb=static_cast<std::int16_t>(word(video.oam(),m+14));
            pc=static_cast<std::int16_t>(word(video.oam(),m+22));pd=static_cast<std::int16_t>(word(video.oam(),m+30));
        }
        for(int dy=0;dy<rh;++dy) {
            const unsigned y=((a&255)+dy)&255;if(y<y0||y>=y1)continue;
            for(int dx=0;dx<rw;++dx) {
                const unsigned x=((b&511)+dx)&511;if(x>=256)continue;
                int u=dx,v=dy;
                if(a&0x1000) {u-=u%mx;v-=v%my;}
                if(affine) {const int ax=u-rw/2,ay=v-rh/2;u=((ax*pa+ay*pb)>>8)+w/2;v=((ax*pc+ay*pd)>>8)+h/2;}
                else {if(b&0x1000)u=w-1-u;if(b&0x2000)v=h-1-v;}
                if(u<0||v<0||u>=w||v>=h)continue;
                unsigned color=0;
                if(mode==3) {
                    unsigned offset;
                    if(control&64)offset=(c&1023)*(128u<<((control>>22)&1))+(v*w+u)*2;
                    else {
                        const unsigned mask=(control&32)?31:15,stride=(mask+1)*8;
                        offset=((c&1023)&mask)*16+((c&1023)&~mask)*128+(v*stride+u)*2;
                    }
                    color=byte(offset)|(unsigned(byte(offset+1))<<8);if(!(color&0x8000))continue;
                } else {
                    const bool eight=(a&0x2000)!=0;const unsigned bytes=eight?64:32;
                    const unsigned start=(c&(eight?1022:1023))*(32u<<((control&16)?((control>>20)&3):0));
                    const unsigned tileOffset=(v/8)*((control&16)?(w/8)*bytes:1024)+(u/8)*bytes;
                    const unsigned pixel=(v&7)*8+(u&7);
                    unsigned index=byte(start+tileOffset+(eight?pixel:pixel/2));
                    if(!eight)index=(index>>((pixel&1)*4))&15;
                    if(!index)continue;
                    if(eight&&(control&0x80000000))color=word(objPalette_,(c>>12)*512+index*2);
                    else color=word(video.palette(),(sub?0x600:0x200)+(index+(!eight?(c>>12)*16:0))*2);
                }
                const auto p=y*256+x;
                if(mode==2) {objWindow_[p]=1;continue;}
                auto& dest=layers_[4][p];const unsigned priority=(c>>10)&3;
                if(dest.color&&priority>=dest.priority)continue;
                dest={rgb(color),static_cast<unsigned char>(priority),static_cast<unsigned char>(mode==3?(c>>12)+1:0),mode==1||mode==3};
            }
        }
    }
}
void Display::engine(NitroVideo& video,bool sub,std::span<const std::uint32_t> three,std::vector<std::uint32_t>& output,
    std::vector<std::uint32_t>& recipes) {
    const unsigned base=0x04000000+(sub?0x1000:0),control=r32(video,base);
    {
    ProfileScope timing("display-read");
    for(auto& layer:layers_)std::fill(layer.begin(),layer.end(),Pixel{});
    std::fill(objWindow_.begin(),objWindow_.end(),static_cast<unsigned char>(0));
    video.readDisplay(sub?0x06200000:0x06000000,bg_);video.readDisplay(sub?0x06600000:0x06400000,obj_);
    video.readExtendedPalette(sub,false,bgPalette_);video.readExtendedPalette(sub,true,objPalette_);
    // Extended palettes would give the pictures other colours: none then.
    hd_=!(control&(1u<<30))&&hdFrameTags(sub,bg_,sub?0x1ffffu:0x7ffffu,video.palette().subspan(sub?0x400:0,0x200),hdBlocks_);
    if(hd_)for(auto& tags:hdTags_)std::fill(tags.begin(),tags.end(),0u);
    }
    ProfileScope composeTiming("display-compose");
    const auto backdrop=rgb(word(video.palette(),sub?0x400:0));
    const unsigned blendControl=r16(video,base+0x50),blendAlpha=r16(video,base+0x52),evy=std::min(16u,r16(video,base+0x54)&31);
    const unsigned eva=std::min(16u,blendAlpha&31),evb=std::min(16u,(blendAlpha>>8)&31),effect=(blendControl>>6)&3;
    const unsigned winin=r16(video,base+0x48),winout=r16(video,base+0x4a);
    const unsigned wh0=r16(video,base+0x40),wh1=r16(video,base+0x42),wv0=r16(video,base+0x44),wv1=r16(video,base+0x46);
    // Recipes treat the 3D layer as unknown pixels, whether or not they are supplied.
    const bool three3d=!sub&&(control&0x108)==0x108;
    // The layer the high-resolution picture replaces: BG0 showing 3D, or the
    // world's captured copy (setWorldBackground).
    // On the sub engine, a direct-colour bitmap (its copy of the world, which
    // may slide while a menu opens) shows the world too.
    const int subBitmap=world_&&sub?directBitmapLayer(video,true):-1;
    const bool worldBitmap=world_&&!sub&&showsBitmapBackdrop(video);
    const int worldLayer=three3d?0:worldBitmap?2:subBitmap;
    const unsigned key3=worldLayer>=0?(r16(video,base+8+2*unsigned(worldLayer))&3)*5+unsigned(worldLayer)+1:0;
    const bool backdropWorld=world_&&sub;
    const unsigned master=r16(video,base+0x6c),masterCode=(control&128)?0:((master>>14)&3)<<5|std::min(16u,master&31);
    const unsigned evyCode=effect>=2?(effect-1)<<5|evy:0;
    // Rows are independent: each band builds its layers, then composes them.
    const auto band=[&](unsigned y0,unsigned y1) {
    for(unsigned bg=0;bg<4;++bg)if(control&(0x100<<bg)) {
        if(!sub&&bg==0&&(control&8)) {
            const unsigned scroll=r16(video,base+0x10)&511,priority=r16(video,base+8)&3;
            for(unsigned y=y0;y<y1;++y)for(unsigned x=0;x<256;++x) {
                const unsigned sx=(x+scroll)&511;if(sx>=256||three.empty())continue;
                const auto color=three[y*256+sx];if(!(color>>24))continue;
                layers_[0][y*256+x]={color,static_cast<unsigned char>(priority),static_cast<unsigned char>(color>>27),false,true};
            }
        } else background(video,sub,bg,control,y0,y1);
    }
    if(control&0x1000)objects(video,sub,control,y0,y1);
    // One pixel's colour and recipe from the layers in `mask`.
    const auto compose=[&](unsigned p,unsigned mask,std::uint32_t& result,std::uint32_t* recipe) {
        unsigned first=5,second=5,key1=99,key2=99;
        // A single OBJ wins before BG composition; sprites cannot alpha-blend
        // against another OBJ. Equal priority puts OBJ first, then BG0..3.
        for(unsigned l=0;l<5;++l) {
            const auto& pixel=layers_[l][p];if(!(mask&(1<<l))||!pixel.color)continue;
            // Behind menus, the sub engine's black lowest-priority sprites only cover the world.
            if(backdropWorld&&l==4&&pixel.priority==3&&!(pixel.color&0xffffff))continue;
            const unsigned key=pixel.priority*5+(l==4?0:l+1);
            if(key<key1) {second=first;key2=key1;first=l;key1=key;}
            else if(key<key2) {second=l;key2=key;}
        }
        const Pixel top=first==5?Pixel{backdrop,4,31}:layers_[first][p];
        const auto below=second==5?backdrop:layers_[second][p].color;
        auto color=top.color;
        if(mask&32) {
            const bool target2=(blendControl&(0x100<<second))!=0,target1=(blendControl&(1<<first))!=0;
            if(target2&&top.three&&top.alpha<31)color=blend(color,below,top.alpha+1,31-top.alpha,5);
            else if(target2&&top.semi)color=top.alpha?blend(color,below,top.alpha,16-top.alpha):blend(color,below,eva,evb);
            // A 3D first target blends by its own alpha only (EVA/EVB are not used).
            else if(target1&&effect==1&&target2&&!top.three)color=blend(color,below,eva,evb);
            else if(target1&&effect>=2)color=brightness(color,effect-1,evy);
        }
        result=color|0xff000000;
        // Plane 1 flag 8 on a final colour: only the backdrop shows there.
        recipe[0]=color&0xffffff;recipe[1]=first==5?8u<<24:0;recipe[2]=(masterCode<<24)|(evb<<11)|(eva<<19);
        if(worldLayer>=0&&(mask&(1u<<worldLayer))) {
            unsigned above=5,aboveKey=99,aboveCount=0,under=5,underKey=99;
            for(unsigned l=0;l<5;++l) {
                const auto& pixel=layers_[l][p];if(int(l)==worldLayer||!(mask&(1<<l))||!pixel.color)continue;
                const unsigned key=pixel.priority*5+(l==4?0:l+1);
                if(key<key3) {++aboveCount;if(key<aboveKey) {above=l;aboveKey=key;}}
                else if(key<underKey) {under=l;underKey=key;}
            }
            const bool effects=(mask&32)!=0;
            if(above==5) {
                // The 3D layer is first; its own alpha decides what shows below.
                const auto underColor=under==5?backdrop:layers_[under][p].color;
                const bool target2=effects&&(blendControl&(0x100<<under)),target1=effects&&(blendControl&(1u<<worldLayer));
                recipe[0]=1u<<24;
                recipe[1]=(underColor&0xffffff)|(target2?1u<<24:0)|(target1&&effect>=2?2u<<24:0);
                recipe[2]|=target1&&effect>=2?evyCode<<8:0;
            } else if(aboveCount==1&&effects) {
                // A single 2D pixel above: it may blend with the 3D layer.
                const auto& pixel=layers_[above][p];
                const bool target2=(blendControl&(0x100u<<worldLayer))!=0,target1=(blendControl&(1<<above))!=0;
                unsigned a=0,b=0;
                if(target2&&pixel.semi) {a=pixel.alpha?pixel.alpha:eva;b=pixel.alpha?16-pixel.alpha:evb;}
                else if(target1&&effect==1&&target2) {a=eva;b=evb;}
                if(a||b) {
                    const auto underColor=under==5?backdrop:layers_[under][p].color;
                    recipe[0]=(pixel.color&0xffffff)|(2u<<24);
                    recipe[1]=(underColor&0xffffff)|((blendControl&(0x100<<under))?1u<<24:0);
                    recipe[2]=(masterCode<<24)|(b<<11)|(a<<19);
                } else recipe[0]=(target1&&effect>=2?brightness(pixel.color,effect-1,evy):pixel.color)&0xffffff;
            }
        } else if(backdropWorld&&first==5) {
            // The world shows through the backdrop, with the backdrop's brightness effect.
            recipe[0]=1u<<24;recipe[1]=backdrop&0xffffff;
            recipe[2]|=(mask&32)&&(blendControl&32)&&effect>=2?evyCode<<8:0;
        }
    };
    for(unsigned y=y0;y<y1;++y)for(unsigned x=0;x<256;++x) {
        const auto p=y*256+x;unsigned mask=63;
        if(control&0xe000) {
            mask=winout&63;
            if((control&0x8000)&&objWindow_[p])mask=(winout>>8)&63;
            if((control&0x4000)&&range(x,wh1)&&range(y,wv1))mask=(winin>>8)&63;
            if((control&0x2000)&&range(x,wh0)&&range(y,wv0))mask=winin&63;
        }
        std::uint32_t color=0,recipe[recipePlanes]={};
        compose(p,mask,color,recipe);
        output[p]=color;
        if(hd_) {
            // A high-resolution picture's pixel, unless a layer above it covers it:
            // the recipe leaves that layer out and the renderer draws the picture.
            int hdLayer=-1;unsigned hdKey=99;
            for(unsigned l=0;l<4;++l)if((mask&(1u<<l))&&hdTags_[l][p]) {
                const unsigned key=layers_[l][p].priority<4?layers_[l][p].priority*5+l+1:(r16(video,base+8+l*2)&3)*5+l+1;
                if(key<hdKey) {hdKey=key;hdLayer=int(l);}
            }
            if(hdLayer>=0) {
                bool covered=false;
                for(unsigned l=0;l<5;++l) {
                    const auto& pixel=layers_[l][p];
                    if(int(l)!=hdLayer&&(mask&(1u<<l))&&pixel.color&&pixel.priority*5+(l==4?0:l+1)<hdKey)covered=true;
                }
                if(!covered) {
                    std::uint32_t ignored=0;
                    layers_[unsigned(hdLayer)][p].color=0;
                    compose(p,mask,ignored,recipe);
                    recipe[3]=hdTags_[unsigned(hdLayer)][p];
                }
            }
        }
        for(unsigned plane=0;plane<recipePlanes;++plane)recipes[plane*count+p]=recipe[plane];
    }
    };
    constexpr unsigned bands=8;
    std::array<unsigned,bands> indices;
    for(unsigned i=0;i<bands;++i)indices[i]=i;
    std::for_each(std::execution::par,indices.begin(),indices.end(),[&](unsigned i) {band(i*192/bands,(i+1)*192/bands);});
    {
        // Pictures placed on the screen this engine shows (their colours are
        // in the main engine's palette, as the menu icons').
        const bool mainTop=(r16(video,0x04000304)&0x8000)!=0;
        hdScreenTags(sub==mainTop?1:0,video.palette().subspan(0,0x200),std::span<std::uint32_t>(recipes.data()+3*count,count));
    }
    if(!sub) {
        graphics_=output; // Capture sees graphics before display mux/brightness.
        std::copy(recipes.begin(),recipes.end(),graphicsRecipes_.begin());
    }
    const unsigned mode=(control>>16)&(sub?1:3);
    if((control&128)||mode==0)std::fill(output.begin(),output.end(),0xffffffff);
    else if(mode==2) {
        const unsigned bank=(control>>18)&3;
        for(unsigned p=0;p<count;++p)output[p]=rgb(video.lcdcBank(bank)?word(video.physicalVram(),bank*0x20000+p*2):0);
    } else if(mode==3)throw std::runtime_error("Main-memory display FIFO is not yet implemented");
    if((control&128)||mode!=1)for(unsigned p=0;p<count;++p) {
        recipes[p]=output[p]&0xffffff;recipes[count+p]=0;recipes[2*count+p]=masterCode<<24;recipes[3*count+p]=0;
    }
    if(!(control&128))for(auto& c:output)c=brightness(c,master>>14,std::min(16u,master&31));
}
int Display::directBitmapLayer(NitroVideo& video,bool sub) {
    const unsigned base=sub?0x04001000:0x04000000,control=r32(video,base),mode=control&7;
    for(unsigned bg=3;bg>=2;--bg) {
        const bool extended=bg==3?mode>=3&&mode<=5:mode==5;
        if(extended&&(control&(0x100u<<bg))&&(r16(video,base+8+2*bg)&0x84)==0x84)return int(bg);
    }
    return -1;
}
bool Display::showsBitmapBackdrop(NitroVideo& video) {
    const unsigned control=r32(video,0x04000000);
    return !(control&8)&&(control&7)>=3&&(control&0x400)&&(r16(video,0x0400000c)&0x84)==0x84&&
        r16(video,0x04000020)==0x100&&r16(video,0x04000022)==0&&r16(video,0x04000024)==0&&r16(video,0x04000026)==0x100&&
        r32(video,0x04000028)==0&&r32(video,0x0400002c)==0;
}
void Display::capture(NitroVideo& video,std::span<const std::uint32_t> three) {
    const unsigned cap=r32(video,0x04000064);if(!(cap&0x80000000))return;
    const unsigned control=r32(video,0x04000000),mode=(cap>>29)&3,bank=(cap>>16)&3;
    if((cap&(1<<25))&&mode)throw std::runtime_error("Capture from display FIFO is not yet implemented");
    const unsigned size=(cap>>20)&3,width=size?256:128,height=size==0?128:size==1?64:size==2?128:192;
    const unsigned readBank=(control>>18)&3,readOffset=((control>>16)&3)==2?0:((cap>>26)&3)*0x8000;
    const unsigned eva=std::min(16u,cap&31),evb=std::min(16u,(cap>>8)&31);
    std::vector<std::uint16_t> pixels(width*height);
    const bool readable=mode!=0&&video.lcdcBank(readBank);
    const auto vram=video.physicalVram();
    for(unsigned y=0;y<height;++y)for(unsigned x=0;x<width;++x) {
        const unsigned p=y*width+x;const auto c=(cap&(1<<24))?(three.empty()?0:three[y*256+x]):graphics_[y*256+x];
        const unsigned a=color15(c)|((c>>24)?0x8000:0);
        const unsigned b=readable?word(vram,readBank*0x20000+((readOffset+p*2)&0x1ffff)):0;
        unsigned result=mode==0?a:mode==1?b:0;
        if(mode>=2) {
            for(unsigned shift:{0,5,10})result|=std::min(31u,((((a>>shift)&31)*(a&0x8000?eva:0)+((b>>shift)&31)*(b&0x8000?evb:0))>>4))<<shift;
            if(((a&0x8000)&&eva)||((b&0x8000)&&evb))result|=0x8000;
        }
        pixels[p]=static_cast<std::uint16_t>(result);
    }
    video.capture(bank,((cap>>18)&3)*0x8000,pixels);
    *static_cast<std::uint32_t*>(video.reg(0x04000064,4))=cap&0x7fffffff;
}
void Display::frame(NitroVideo& video,std::span<const std::uint32_t> three,bool subEngine) {
    if(!three.empty()&&three.size()!=count)throw std::runtime_error("Display requires a 256x192 3D frame");
    engine(video,false,three,main_,mainRecipes_);
    if(subEngine)engine(video,true,{},sub_,subRecipes_);
    const bool mainTop=(r16(video,0x04000304)&0x8000)!=0;mainTop_=mainTop;
    std::copy(main_.begin(),main_.end(),screens_.begin()+(mainTop?0:count));
    std::copy(sub_.begin(),sub_.end(),screens_.begin()+(mainTop?count:0));
    for(unsigned plane=0;plane<recipePlanes;++plane) {
        auto* destination=recipes_.data()+plane*count*2;
        std::copy_n(mainRecipes_.data()+plane*count,count,destination+(mainTop?0:count));
        std::copy_n(subRecipes_.data()+plane*count,count,destination+(mainTop?count:0));
    }
    ProfileScope timing("display-capture");
    capture(video,three);
}
}
