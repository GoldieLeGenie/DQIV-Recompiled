#include "HdPictures.hpp"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

namespace pc {
namespace {
struct Picture {
    bool sub=false;
    unsigned offset=0;
    std::vector<std::uint8_t> tiles;
    int columns=0,rows=0;
    std::array<std::uint8_t,hdSourceSize*hdSourceSize> indices{}; // row-major
};
constexpr int cellCount=hdAtlasCells*hdAtlasCells;
struct State {
    std::vector<Picture> pictures;
    std::unordered_map<std::uint64_t,int> cells; // picture colours -> atlas cell
    std::array<std::uint64_t,cellCount> cellKeys{};
    std::array<std::uint64_t,cellCount> cellUse{};
    std::uint64_t frame=0;
    std::vector<HdUpload> uploads;
};
State& state() {static State s;return s;}
std::uint32_t colour(std::span<const std::uint8_t> palette,unsigned index) {
    const unsigned c=palette[index*2]|(palette[index*2+1]<<8);
    const auto expand=[](unsigned v){return (v<<3)|(v>>2);};
    return 0xff000000u|expand(c&31)<<16|expand((c>>5)&31)<<8|expand((c>>10)&31);
}
}
namespace {
// The atlas cell of a picture (32 x 32 palette indices) in `palette`'s
// colours, or -1 when every cell is in use this frame.
int cellFor(const std::array<std::uint8_t,hdSourceSize*hdSourceSize>& indices,std::span<const std::uint8_t> palette) {
    auto& s=state();
    std::vector<std::uint32_t> pixels(hdSourceSize*hdSourceSize);
    std::uint64_t key=1469598103934665603ull;
    for(std::size_t i=0;i<pixels.size();++i) {
        const auto index=indices[i];
        pixels[i]=index?colour(palette,index):0;
        key=(key^pixels[i])*1099511628211ull;
    }
    int cell;
    if(auto found=s.cells.find(key);found!=s.cells.end())cell=found->second;
    else {
        // The cell used longest ago, except by this frame.
        cell=int(std::min_element(s.cellUse.begin(),s.cellUse.end())-s.cellUse.begin());
        if(s.cellUse[cell]==s.frame)return -1;
        if(s.cellUse[cell])s.cells.erase(s.cellKeys[cell]);
        s.cells[key]=cell;s.cellKeys[cell]=key;
        s.uploads.push_back({cell,std::move(pixels)});
    }
    s.cellUse[cell]=s.frame;
    return cell;
}
// The atlas cell of a full-colour picture (hdCellSize x hdCellSize), or -1.
int cellFor(std::uint64_t key,const std::vector<std::uint32_t>& pixels) {
    auto& s=state();
    key^=0x9e3779b97f4a7c15ull; // apart from the palette pictures' keys
    int cell;
    if(auto found=s.cells.find(key);found!=s.cells.end())cell=found->second;
    else {
        cell=int(std::min_element(s.cellUse.begin(),s.cellUse.end())-s.cellUse.begin());
        if(s.cellUse[cell]==s.frame)return -1;
        if(s.cellUse[cell])s.cells.erase(s.cellKeys[cell]);
        s.cells[key]=cell;s.cellKeys[cell]=key;
        s.uploads.push_back({cell,pixels});
    }
    s.cellUse[cell]=s.frame;
    return cell;
}
std::array<std::uint8_t,hdSourceSize*hdSourceSize> linear(std::span<const std::uint8_t> source) {
    std::array<std::uint8_t,hdSourceSize*hdSourceSize> indices{};
    for(int y=0;y<hdSourceSize;++y)for(int x=0;x<hdSourceSize;++x)
        indices[y*hdSourceSize+x]=source[((y/8)*(hdSourceSize/8)+x/8)*64+(y%8)*8+x%8];
    return indices;
}
}
void registerHdPicture(bool sub,unsigned vramOffset,std::span<const std::uint8_t> tiles,int columns,int rows,
    std::span<const std::uint8_t> source) {
    if(columns<=0||rows<=0||columns*8>64||rows*8>64||tiles.size()<std::size_t(columns*rows*64)||
        source.size()<std::size_t(hdSourceSize*hdSourceSize))return;
    auto& pictures=state().pictures;
    const unsigned end=vramOffset+unsigned(columns*rows*64);
    // The new tiles replace whatever was registered where they are written.
    std::erase_if(pictures,[&](const Picture& p){
        return p.sub==sub&&p.offset<end&&vramOffset<p.offset+unsigned(p.tiles.size());
    });
    Picture picture;
    picture.sub=sub;picture.offset=vramOffset;picture.columns=columns;picture.rows=rows;
    picture.tiles.assign(tiles.begin(),tiles.begin()+columns*rows*64);
    picture.indices=linear(source);
    pictures.push_back(std::move(picture));
}
bool hdFrameTags(bool sub,std::span<const std::uint8_t> vram,unsigned vramMask,std::span<const std::uint8_t> palette,
    std::vector<std::uint32_t>& tags) {
    auto& s=state();
    bool any=false;
    ++s.frame;
    for(const auto& picture:s.pictures) {
        if(picture.sub!=sub)continue;
        const unsigned size=unsigned(picture.tiles.size());
        if(((picture.offset+size-1)&vramMask)>=vram.size()||(picture.offset&vramMask)+size>vram.size()||
            std::memcmp(vram.data()+(picture.offset&vramMask),picture.tiles.data(),size)!=0) {
            continue;
        }
        const int cell=cellFor(picture.indices,palette);
        if(cell<0)continue;
        if(!any) {std::fill(tags.begin(),tags.end(),0u);any=true;}
        const std::uint32_t base=std::uint32_t(cell+1)<<24|std::uint32_t(picture.columns*8-1)<<18|
            std::uint32_t(picture.rows*8-1)<<12;
        for(int t=0;t<picture.columns*picture.rows;++t) {
            const unsigned block=((picture.offset&vramMask)>>6)+unsigned(t);
            if(block<tags.size())tags[block]=base|std::uint32_t((t%picture.columns)*8)<<6|std::uint32_t((t/picture.columns)*8);
        }
    }
    return any;
}
namespace {
struct Placed {
    int x=0,y=0,width=0,height=0;
    std::array<std::uint8_t,hdSourceSize*hdSourceSize> indices{};
    std::uint64_t key=0;
    std::shared_ptr<const std::vector<std::uint32_t>> argb; // full colour instead of indices
};
std::array<std::vector<Placed>,2>& placed() {static std::array<std::vector<Placed>,2> p;return p;}
}
void placeHdPicture(int screen,int x,int y,int width,int height,std::span<const std::uint8_t> source) {
    if(screen<0||screen>1||width<=0||height<=0||width>64||height>64||source.size()<std::size_t(hdSourceSize*hdSourceSize))return;
    auto& list=placed()[screen];
    std::erase_if(list,[&](const Placed& p){return p.x==x&&p.y==y;});
    list.push_back({x,y,width,height,linear(source)});
}
void placeHdPicture(int screen,int x,int y,int width,int height,std::uint64_t key,
    std::shared_ptr<const std::vector<std::uint32_t>> argb) {
    if(screen<0||screen>1||width<=0||height<=0||width>64||height>64||!argb||argb->size()!=std::size_t(hdCellSize*hdCellSize))return;
    auto& list=placed()[screen];
    std::erase_if(list,[&](const Placed& p){return p.x==x&&p.y==y;});
    Placed picture;
    picture.x=x;picture.y=y;picture.width=width;picture.height=height;picture.key=key;picture.argb=std::move(argb);
    list.push_back(std::move(picture));
}
void clearHdPictures(int screen) {
    if(screen>=0&&screen<=1)placed()[screen].clear();
}
void hdScreenTags(int screen,std::span<const std::uint8_t> palette,std::span<std::uint32_t> tags) {
    for(const auto& picture:placed()[screen]) {
        const int cell=picture.argb?cellFor(picture.key,*picture.argb):cellFor(picture.indices,palette);
        if(cell<0)continue;
        const std::uint32_t base=std::uint32_t(cell+1)<<24|std::uint32_t(picture.width-1)<<18|std::uint32_t(picture.height-1)<<12;
        for(int y=std::max(0,picture.y);y<std::min(192,picture.y+picture.height);++y)
            for(int x=std::max(0,picture.x);x<std::min(256,picture.x+picture.width);++x) {
                auto& tag=tags[std::size_t(y)*256+x];
                if(!tag)tag=base|std::uint32_t(x-picture.x)<<6|std::uint32_t(y-picture.y);
            }
    }
}
std::vector<HdUpload> takeHdUploads() {
    std::vector<HdUpload> uploads;
    uploads.swap(state().uploads);
    return uploads;
}
}
