#include "GxRasterizer.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace pc {
namespace {
unsigned six(unsigned five) {return five?five*2+1:0;}
unsigned eight(unsigned value) {return (value<<2)|(value>>4);}
std::uint32_t rgb(unsigned packed,unsigned alpha) {
    return (((alpha<<3)|(alpha>>2))<<24)|(eight(six(packed&31))<<16)|
        (eight(six((packed>>5)&31))<<8)|eight(six((packed>>10)&31));
}
struct Vertex {
    std::array<float,4> clip;
    float x=0,y=0,inverse=0,z=0,u=0,v=0;
    std::array<float,3> color{};
    bool edge=true;
};
Vertex mix(const Vertex& a,const Vertex& b,float t) {
    Vertex out;
    for(unsigned c=0;c<4;++c)out.clip[c]=a.clip[c]+(b.clip[c]-a.clip[c])*t;
    out.u=a.u+(b.u-a.u)*t;out.v=a.v+(b.v-a.v)*t;
    for(unsigned c=0;c<3;++c)out.color[c]=a.color[c]+(b.color[c]-a.color[c])*t;
    return out;
}
float edge(const Vertex& a,const Vertex& b,float x,float y) {return (b.x-a.x)*(y-a.y)-(b.y-a.y)*(x-a.x);}
bool inclusive(const Vertex& a,const Vertex& b) {return b.y<a.y || (b.y==a.y && b.x>a.x);}
int coordinate(float v,int size,bool repeat,bool mirror) {
    if(!repeat)return int(std::clamp(std::floor(v),0.f,float(size-1)));
    auto n=std::fmod(std::floor(v),float(size*(mirror?2:1)));if(n<0)n+=size*(mirror?2:1);
    const auto index=int(n);return mirror && index>=size?size*2-1-index:index;
}
struct Polygon {
    std::vector<Vertex> vertices;
    const GxTriangle* source;
    const Texture* texture=nullptr;
    float minY=0,maxY=0;
    bool translucent=false;
};
}
void GxRasterizer::draw(std::span<const GxTriangle> triangles,const GxRasterState& state,GxTextures& textures,GxRenderOptions options) {
    const int width=options.width,height=options.height;
    if(width<1 || height<1 || width>8192 || height>8192)throw std::runtime_error("Invalid native game render dimensions");
    const auto size=std::size_t(width)*height;
    const unsigned clearDepth=(unsigned(state.clearDepth)<<9)|511;
    pixels_.assign(size,rgb(state.clearColor,(state.clearColor>>16)&31));
    depth_.assign(size,clearDepth);polygon_.assign(size,(state.clearColor>>24)&63);
    translucent_.assign(size,255);fog_.assign(size,(state.clearColor>>15)&1);shadow_.assign(size,0);drawn_=0;
    std::vector<Polygon> polygons;
    for(const auto& triangle:triangles) {
        const auto attributes=triangle.material.polygon;
        if(!(attributes&0xc0))continue;
        Polygon polygon;polygon.source=&triangle;
        bool outsideFar=false;
        for(unsigned i=0;i<3;++i) {
            const auto& v=triangle.vertices[i];Vertex out;out.clip=v.clip;out.u=v.u;out.v=v.v;
            for(float value:v.clip)if(!std::isfinite(value))throw std::runtime_error("Nonfinite native clip coordinate");
            if(!std::isfinite(v.u) || !std::isfinite(v.v))throw std::runtime_error("Nonfinite native texture coordinate");
            for(unsigned c=0;c<3;++c)out.color[c]=float(six((v.color>>(5*c))&31));
            out.edge=(triangle.edges&(1u<<((i+2)%3)))!=0;
            polygon.vertices.push_back(out);outsideFar|=v.clip[2]>v.clip[3];
        }
        if(outsideFar && !(attributes&(1<<12)))continue;
        for(unsigned plane=0;plane<7 && !polygon.vertices.empty();++plane) {
            auto distance=[&](const Vertex& v) {return plane==6?v.clip[3]-0.000001f:v.clip[3]+((plane&1)?-v.clip[plane/2]:v.clip[plane/2]);};
            std::vector<Vertex> clipped;auto previous=polygon.vertices.back();auto pd=distance(previous);
            for(const auto& v:polygon.vertices) {
                const auto d=distance(v);
                if((d>=0)!=(pd>=0)) {auto intersection=mix(previous,v,pd/(pd-d));intersection.edge=pd>=0?v.edge:true;clipped.push_back(intersection);}
                if(d>=0)clipped.push_back(v);
                previous=v;pd=d;
            }
            polygon.vertices=std::move(clipped);
        }
        if(polygon.vertices.size()<3)continue;
        const auto viewport=triangle.viewport;
        const float vx=float(viewport&255),vy=float((viewport>>8)&255);
        const float vw=float(((viewport>>16)&255)+1)-vx,vh=float((viewport>>24)+1)-vy;
        if(vw<=0 || vh<=0)continue;
        polygon.minY=float(height);polygon.maxY=0;
        for(auto& v:polygon.vertices) {
            v.inverse=1/v.clip[3];v.z=v.clip[2]*v.inverse;
            v.x=(vx+(v.clip[0]*v.inverse+1)*vw*.5f)*width/256;
            v.y=(192-vy-(v.clip[1]*v.inverse+1)*vh*.5f)*height/192;
            polygon.minY=std::min(polygon.minY,v.y);polygon.maxY=std::max(polygon.maxY,v.y);
        }
        const auto format=(triangle.material.texture>>26)&7,mode=(attributes>>4)&3,alpha=(attributes>>16)&31;
        if(format && (options.displayControl&1))polygon.texture=&textures.texture(triangle.material.texture,triangle.material.palette);
        polygon.translucent=(alpha>0 && alpha<31) || (polygon.texture && !(mode&1) && (format==1 || format==6));
        polygons.push_back(std::move(polygon));
    }
    std::stable_sort(polygons.begin(),polygons.end(),[&](const auto& a,const auto& b) {
        if(a.translucent!=b.translucent)return !a.translucent;
        if(!a.translucent || options.manualSort)return false;
        return a.maxY!=b.maxY?a.maxY<b.maxY:a.minY<b.minY;
    });
    for(const auto& polygon:polygons) {
        const auto attributes=polygon.source->material.polygon,param=polygon.source->material.texture;
        const unsigned mode=(attributes>>4)&3,id=(attributes>>24)&63;
        const unsigned vertexAlpha=(attributes>>16)&31;
        for(std::size_t k=2;k<polygon.vertices.size();++k) {
            auto a=polygon.vertices[0],b=polygon.vertices[k-1],c=polygon.vertices[k];
            bool ab=k==2 && b.edge,bc=c.edge,ca=k+1==polygon.vertices.size() && a.edge;
            float area=edge(a,b,c.x,c.y);
            if(std::abs(area)<0.00001f)continue;
            if(area<0 ? !(attributes&0x80) : !(attributes&0x40))continue;
            if(area<0) {std::swap(b,c);std::swap(ab,ca);area=-area;}
            const int minX=int(std::clamp(std::floor(std::min({a.x,b.x,c.x})),0.f,float(width-1)));
            const int maxX=int(std::clamp(std::ceil(std::max({a.x,b.x,c.x})),0.f,float(width-1)));
            const int minY=int(std::clamp(std::floor(std::min({a.y,b.y,c.y})),0.f,float(height-1)));
            const int maxY=int(std::clamp(std::ceil(std::max({a.y,b.y,c.y})),0.f,float(height-1)));
            const float lengths[3]={std::hypot(c.x-b.x,c.y-b.y),std::hypot(a.x-c.x,a.y-c.y),std::hypot(b.x-a.x,b.y-a.y)};
            const bool include[3]={inclusive(b,c),inclusive(c,a),inclusive(a,b)};
            for(int y=minY;y<=maxY;++y)for(int x=minX;x<=maxX;++x) {
                const float px=x+.5f,py=y+.5f;
                const float edges[3]={edge(b,c,px,py),edge(c,a,px,py),edge(a,b,px,py)};
                if(edges[0]<0 || edges[1]<0 || edges[2]<0)continue;
                if((edges[0]==0&&!include[0]) || (edges[1]==0&&!include[1]) || (edges[2]==0&&!include[2]))continue;
                if(!vertexAlpha && !(bc && edges[0]<lengths[0] || ca && edges[1]<lengths[1] || ab && edges[2]<lengths[2]))continue;
                const float w0=edges[0]/area,w1=edges[1]/area,w2=edges[2]/area;
                const float inverse=w0*a.inverse+w1*b.inverse+w2*c.inverse;
                if(inverse<=0)continue;
                const float perspectiveB=w1*b.inverse/inverse,perspectiveC=w2*c.inverse/inverse;
                const float depth=options.wBuffer?4096/inverse:(w0*a.z+w1*b.z+w2*c.z+1)*0x7fffff;
                const auto z=static_cast<unsigned>(std::clamp(depth,0.f,16777215.f));
                const auto index=std::size_t(y)*width+x;
                const bool passes=attributes&(1<<14)?std::abs(std::int64_t(z)-depth_[index])<=512:z<depth_[index];
                if(mode==3 && !id) {if(!passes)shadow_[index]=1;continue;}
                if(!passes || (mode==3 && (!shadow_[index] || polygon_[index]==id)))continue;
                unsigned alpha=vertexAlpha?vertexAlpha:31;
                std::array<unsigned,3> color;
                for(unsigned channel=0;channel<3;++channel)color[channel]=unsigned(std::clamp(a.color[channel]+
                    (b.color[channel]-a.color[channel])*perspectiveB+(c.color[channel]-a.color[channel])*perspectiveC,0.f,63.f));
                std::array<unsigned,3> highlight{};
                if(mode==2) {
                    const auto shade=state.toonTable[color[0]>>1];
                    if(options.displayControl&2) {
                        color[1]=color[2]=color[0];
                        for(unsigned channel=0;channel<3;++channel)highlight[channel]=six((shade>>(5*channel))&31);
                    } else for(unsigned channel=0;channel<3;++channel)color[channel]=six((shade>>(5*channel))&31);
                }
                if(polygon.texture) {
                    const float u=a.u+(b.u-a.u)*perspectiveB+(c.u-a.u)*perspectiveC;
                    const float v=a.v+(b.v-a.v)*perspectiveB+(c.v-a.v)*perspectiveC;
                    const int tx=coordinate(u,polygon.texture->width,(param&(1<<16))!=0,(param&(1<<18))!=0);
                    const int ty=coordinate(v,polygon.texture->height,(param&(1<<17))!=0,(param&(1<<19))!=0);
                    const auto texel=polygon.texture->pixels[std::size_t(ty)*polygon.texture->width+tx];
                    const auto textureAlpha=texel>>27;
                    for(unsigned channel=0;channel<3;++channel) {
                        const auto textureColor=(texel>>(16-channel*8))&255;
                        const auto t=six(textureColor>>3),original=color[channel];
                        if(mode&1)color[channel]=textureAlpha==31?t:textureAlpha==0?original:(t*textureAlpha+original*(31-textureAlpha))/32;
                        else color[channel]=((t+1)*(original+1)-1)/64;
                    }
                    if(!(mode&1))alpha=((textureAlpha+1)*(alpha+1)-1)/32;
                }
                for(unsigned channel=0;channel<3;++channel)color[channel]=std::min(63u,color[channel]+highlight[channel]);
                if(!alpha || ((options.displayControl&4) && alpha<=(options.alphaTest&31)))continue;
                if(alpha<31 && translucent_[index]==id)continue;
                const unsigned oldAlpha=pixels_[index]>>27;
                if(alpha<31 && oldAlpha && (options.displayControl&8)) {
                    for(unsigned channel=0;channel<3;++channel)color[channel]=(color[channel]*(alpha+1)+((pixels_[index]>>(16-channel*8))&255)/4*(31-alpha))/32;
                }
                const auto outputAlpha=(alpha<31 && oldAlpha && (options.displayControl&8))?std::max(alpha,oldAlpha):alpha;
                pixels_[index]=((outputAlpha<<3)|(outputAlpha>>2))<<24|eight(color[0])<<16|eight(color[1])<<8|eight(color[2]);++drawn_;
                if(alpha==31 || (attributes&(1<<11)))depth_[index]=z;
                if(alpha==31) {polygon_[index]=id;translucent_[index]=255;fog_[index]=(attributes>>15)&1;}
                else {translucent_[index]=id;fog_[index]&=(attributes>>15)&1;}
                if(mode==3)shadow_[index]=0;
            }
        }
    }
    if(state.fogEnabled)for(std::size_t i=0;i<size;++i)if(fog_[i]) {
        const unsigned step=0x400u>>std::min(15u,state.fogSlope);
        const float position=step?(float(depth_[i]>>9)-state.fogOffset)/step-1:((depth_[i]>>9)>state.fogOffset?31.f:0.f);
        const float clamped=std::clamp(position,0.f,31.f);const unsigned lo=unsigned(clamped),hi=std::min(31u,lo+1);
        auto density=[&](unsigned n){return state.fogTable[n]==127?128.f:float(state.fogTable[n]&127);};
        const auto d=unsigned(density(lo)+(density(hi)-density(lo))*(clamped-lo));
        auto value=pixels_[i];
        for(unsigned c=0;c<4;++c) {
            if(state.fogAlphaOnly && c<3)continue;
            const auto shift=c==3?24:16-c*8;
            const unsigned target=c==3?((state.fogColor>>16)&31)*255/31:eight(six((state.fogColor>>(5*c))&31));
            const auto channel=(target*d+((value>>shift)&255)*(128-d))/128;
            value=(value&~(255u<<shift))|(channel<<shift);
        }
        pixels_[i]=value;
    }
}
}
