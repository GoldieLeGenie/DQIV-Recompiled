#include "GpuRenderer.hpp"
#include "HdPictures.hpp"
#include "GlLoader.hpp"
#include "GxTextures.hpp"
#include "Profile.hpp"
#include <algorithm>
#include <execution>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <stdexcept>
#include <string>

namespace pc {
namespace {
// Profiling only (DQIV_PROFILE=gpu): waits for the GPU, charging its time to `stage`.
void gpuCheckpoint(const char* stage) {
    static const bool enabled=[]{const char* v=std::getenv("DQIV_PROFILE");return v&&std::string(v)=="gpu";}();
    if(enabled) {ProfileScope timing(stage);gl::Finish();}
}
}
namespace {
const char* sceneVertex=R"(#version 330 core
layout(location=0) in vec4 aPosition;
layout(location=1) in vec4 aColor;
layout(location=2) in vec4 aTexture;
layout(location=3) in uvec4 aParams;
layout(location=4) in vec4 aClip;
layout(location=5) in vec4 aRect;
out vec3 vColor;
out vec2 vUv;
flat out vec2 vSize;
flat out uvec4 vParams;
flat out vec4 vRect;
out float vW;
void main() {
    gl_Position=aPosition;vColor=aColor.rgb;vUv=aTexture.xy;vSize=aTexture.zw;vParams=aParams;vW=aPosition.w;vRect=aRect;
    // The DS screen's own edges, for overlays placed inside a larger target.
    gl_ClipDistance[0]=aClip.x;gl_ClipDistance[1]=aClip.y;gl_ClipDistance[2]=aClip.z;gl_ClipDistance[3]=aClip.w;
})";
// DS 3D pixel pipeline: vertex colour, texture blend modes, toon/highlight,
// alpha test. Colour channels are in the hardware's 0..63 range until output.
const char* sceneFragment=R"(#version 330 core
in vec3 vColor;
in vec2 vUv;
flat in vec2 vSize;
flat in uvec4 vParams;
flat in vec4 vRect;
in float vW;
uniform sampler2D uTexture;
uniform int uControl;
uniform float uAlphaReference;
uniform vec3 uToon[32];
uniform int uWBuffer;
uniform int uPass;
layout(location=0) out vec4 oColor;
layout(location=1) out vec4 oAttributes;
void main() {
    uint attributes=vParams.x,flags=vParams.z;
    uint mode=(attributes>>4)&3u;
    float alpha=float((attributes>>16)&31u);
    vec3 color=clamp(vColor*63.0,0.0,63.0),highlight=vec3(0.0);
    if(mode==2u) {
        vec3 shade=uToon[min(31,int(color.r)>>1)];
        if((uControl&2)!=0) {color=vec3(color.r);highlight=shade;} else color=shade;
    }
    if((flags&1u)!=0u) {
        vec2 uv=vUv;
        if((flags&4u)!=0u) {
            // A part cut from a sheet (monster sprites): filtering stays inside
            // its own texels, half an upscaled texel from its edges, or the
            // neighbouring picture's transparent border opens a seam.
            vec2 inset=0.5*vSize/vec2(textureSize(uTexture,0));
            uv=clamp(uv,min(vRect.xy+inset,vRect.zw),max(vRect.zw-inset,vRect.xy));
        }
        // Sheet parts read the full-size texels: smaller mipmap levels mix in
        // the neighbouring pictures of the sheet.
        vec4 texel=(flags&4u)!=0u?textureLod(uTexture,uv/vSize,0.0):texture(uTexture,uv/vSize);
        float textureAlpha=texel.a*31.0;
        vec3 textureColor=texel.a>0.0?texel.rgb/texel.a*63.0:vec3(0.0);
        if(mode==1u) color=mix(color,textureColor,clamp(textureAlpha/31.0,0.0,1.0));
        else {color=((textureColor+1.0)*(color+1.0)-1.0)/64.0;alpha=((textureAlpha+1.0)*(alpha+1.0)-1.0)/32.0;}
    }
    color=min(color+highlight,vec3(63.0));
    if((flags&2u)!=0u) {if(alpha<15.5) discard;alpha=31.0;}
    if(alpha<0.5) discard;
    if((uControl&4)!=0 && alpha<=uAlphaReference) discard;
    float a=clamp(alpha/31.0,0.0,1.0);
    oColor=vec4(color/63.0*a,a);
    bool fog=((attributes>>15)&1u)!=0u;
    if(uPass==0) oAttributes=vec4(fog?1.0:0.0,float((attributes>>24)&63u)/63.0,0.0,1.0);
    else oAttributes=vec4(0.0,0.0,0.0,fog?0.0:1.0);
    // Hardware Z is (z+1)*0x7fffff: the far plane stays below the 0xffffff clear depth.
    float depth=uWBuffer!=0?clamp(vW*4096.0/16777215.0,0.0,1.0):gl_FragCoord.z*(16777214.0/16777215.0);
    if(((attributes>>14)&1u)!=0u) depth-=512.0/16777215.0;
    gl_FragDepth=depth;
})";
const char* fullscreenVertex=R"(#version 330 core
out vec2 vPosition;
void main() {
    vec2 corner=vec2(float(gl_VertexID&1),float((gl_VertexID>>1)&1));
    vPosition=corner;gl_Position=vec4(corner*2.0-1.0,0.0,1.0);
})";
// Two fog settings (lower and upper part of the target) blended over a band,
// for the unified two-screen picture; both are equal otherwise.
const char* fogFragment=R"(#version 330 core
uniform sampler2D uColor;
uniform sampler2D uDepth;
uniform sampler2D uAttributes;
uniform int uEnabled[2];
uniform int uAlphaOnly[2];
uniform vec4 uFogColor[2];
uniform float uFogTable[64];
uniform float uFogOffset[2];
uniform float uFogStep[2];
uniform float uSplit;
uniform float uBand;
out vec4 oColor;
vec4 fog(int i,vec4 color,float depth) {
    if(uEnabled[i]==0) return color;
    float position=uFogStep[i]>0.0?(depth-uFogOffset[i])/uFogStep[i]-1.0:(depth>uFogOffset[i]?31.0:0.0);
    position=clamp(position,0.0,31.0);
    int low=int(position),high=min(31,low+1);
    float density=mix(uFogTable[i*32+low],uFogTable[i*32+high],position-float(low))/128.0;
    vec4 straight=color.a>0.0?vec4(color.rgb/color.a,color.a):vec4(0.0);
    if(uAlphaOnly[i]!=0) straight.a=mix(straight.a,uFogColor[i].a,density);
    else straight=mix(straight,uFogColor[i],density);
    return vec4(straight.rgb*straight.a,straight.a);
}
void main() {
    ivec2 p=ivec2(gl_FragCoord.xy);
    vec4 color=texelFetch(uColor,p,0);
    if(texelFetch(uAttributes,p,0).r>0.5) {
        float depth=texelFetch(uDepth,p,0).r*16777215.0/512.0;
        float upper=uBand>0.0?smoothstep(uSplit-uBand,uSplit+uBand,gl_FragCoord.y):step(uSplit,gl_FragCoord.y);
        color=mix(fog(0,color,depth),fog(1,color,depth),upper);
    }
    oColor=color;
})";
// Interface pictures (premultiplied) placed over the presented frame.
const char* overlayVertex=R"(#version 330 core
uniform vec4 uRect;
out vec2 vPosition;
void main() {
    vec2 corner=vec2(float(gl_VertexID&1),float((gl_VertexID>>1)&1));
    vPosition=vec2(corner.x,1.0-corner.y);
    gl_Position=vec4(mix(uRect.xy,uRect.zw,corner),0.0,1.0);
})";
const char* overlayFragment=R"(#version 330 core
in vec2 vPosition;
uniform sampler2D uPicture;
out vec4 oColor;
void main() {oColor=texture(uPicture,vPosition);})";
const char* reduceFragment=R"(#version 330 core
uniform sampler2D uSource;
uniform vec2 uOrigin;
uniform vec2 uScale;
out vec4 oColor;
void main() {
    vec2 origin=uOrigin+floor(gl_FragCoord.xy)*uScale;
    vec4 sum=vec4(0.0);
    for(int y=0;y<4;++y) for(int x=0;x<4;++x) sum+=texture(uSource,(origin+(vec2(x,y)+0.5)*uScale/4.0)/vec2(textureSize(uSource,0)));
    oColor=sum/16.0;
})";
// Final picture: per-pixel DS composition recipes (see Display) applied to
// the high-resolution 3D layer, then master brightness.
const char* composeFragment=R"(#version 330 core
in vec2 vPosition;
uniform sampler2D uRecipe0;
uniform sampler2D uRecipe1;
uniform sampler2D uRecipe2;
uniform sampler2D uRecipe3;
uniform sampler2D uHdAtlas;
uniform sampler2D uThree0;
uniform sampler2D uThree1;
uniform isampler2D uSideColumns;
uniform vec2 uCanvas;
uniform vec4 uThreeRect0;
uniform vec4 uThreeRect1;
uniform int uSides;
uniform int uBackdrop3D;
uniform vec4 uInset;
uniform int uPlaced;
uniform float uInsetAlpha;
uniform vec4 uScreen0;
uniform vec4 uScreen1;
uniform int uThreeScreenRow;
uniform float uScale;
out vec4 oColor;
vec4 three;
vec3 brightness(vec3 color,float code) {
    int value=int(code*255.0+0.5),mode=value>>5;
    float amount=float(value&31)/16.0;
    if(mode==1) return color+(1.0-color)*amount;
    if(mode==2) return color-color*amount;
    return color;
}
// One DS pixel's recipe applied to this fragment's 3D sample.
int rowFirst=0,rowLast=383;
bool insetCell=false;
vec3 pixel(ivec2 cell) {
    bool side=!insetCell&&(cell.x<0||cell.x>255);
    if(side&&uSides==0) return vec3(0.0);
    ivec2 r=ivec2(clamp(cell.x,0,255),clamp(cell.y,rowFirst,rowLast));
    vec4 r0=texelFetch(uRecipe0,r,0);
    int kind=int(r0.a*255.0+0.5);
    vec4 r1,r2;
    if(side&&kind!=1) {
        // Beside the screens: the 3D scene with the effects of the nearest
        // column of the row where it shows directly (2D windows may cover the
        // edge), found once per row and side (GpuRenderer::present).
        int column=texelFetch(uSideColumns,ivec2(cell.x<0?0:1,r.y),0).r;
        if(column>=0) {r.x=column;r0=texelFetch(uRecipe0,r,0);r1=texelFetch(uRecipe1,r,0);r2=texelFetch(uRecipe2,r,0);}
        else {r1=vec4(0.0);r2=vec4(0.0,0.0,0.0,texelFetch(uRecipe2,r,0).a);}
        kind=1;
    } else {r1=texelFetch(uRecipe1,r,0);r2=texelFetch(uRecipe2,r,0);}
    // A screen's bare backdrop lets the 3D scene through (wide battles); that
    // 3D keeps the fades of the screen it comes from.
    if(uBackdrop3D!=0&&kind==0&&(int(r1.a*255.0+0.5)&8)!=0) {
        ivec2 source=ivec2(r.x,uThreeScreenRow);
        kind=1;
        r1=vec4(0.0,0.0,0.0,float(int(texelFetch(uRecipe1,source,0).a*255.0+0.5)&2)/255.0);
        r2=texelFetch(uRecipe2,source,0);r2.rg=vec2(0.0);
    }
    vec3 color=r0.rgb;
    if(kind!=0) {
        int flags=int(r1.a*255.0+0.5);
        vec3 below=r1.rgb;
        vec3 straight=three.a>0.0?three.rgb/three.a:below;
        vec3 over=three.rgb+below*(1.0-three.a);
        float eva=r2.r*255.0/128.0,evb=r2.g*255.0/128.0;
        if(kind==1) {
            if(three.a<0.999) color=(flags&1)!=0?over:straight;
            else color=straight;
            if(three.a>=0.999 && (flags&2)!=0) color=brightness(color,r2.b);
        } else color=min(vec3(1.0),r0.rgb*eva+over*evb);
    }
    return brightness(color,r2.a);
}
// A high-resolution picture over the DS pixel at `cell` (recipe plane 3),
// sampled where this fragment falls in that pixel.
vec3 hdOver(vec3 color,vec2 cell) {
    vec2 at=cell+0.5;
    ivec2 c=ivec2(floor(at));
    if(!insetCell&&(c.x<0||c.x>255)) return color;
    c=ivec2(clamp(c.x,0,255),clamp(c.y,rowFirst,rowLast));
    uvec4 v=uvec4(texelFetch(uRecipe3,c,0)*255.0+0.5);
    uint tag=(v.a<<24)|(v.r<<16)|(v.g<<8)|v.b;
    if(tag==0u) return color;
    uint picture=(tag>>24)-1u;
    vec2 size=vec2(float(((tag>>18)&63u)+1u),float(((tag>>12)&63u)+1u));
    vec2 uv=clamp((vec2(float((tag>>6)&63u),float(tag&63u))+fract(at))/size,vec2(0.5/128.0),vec2(1.0-0.5/128.0));
    vec2 origin=vec2(float(picture%16u),float(picture/16u))*128.0;
    vec4 hd=texture(uHdAtlas,(origin+uv*128.0)/2048.0);
    if(hd.a<=0.0) return color;
    return mix(color,brightness(hd.rgb/hd.a,texelFetch(uRecipe2,c,0).a),hd.a);
}
// Sharp bilinear sampling of the DS pixel grid around `cell`.
vec3 sharp(vec2 cell,float scale) {
    vec2 base=floor(cell),weight=clamp((cell-base-0.5)*scale+0.5,0.0,1.0);
    ivec2 b=ivec2(base);
    // Most fragments lie inside one DS pixel: neighbours only at its borders.
    vec3 top=pixel(b);
    if(weight.x>0.0) top=mix(top,pixel(b+ivec2(1,0)),weight.x);
    if(weight.y>0.0) {
        vec3 bottom=pixel(b+ivec2(0,1));
        if(weight.x>0.0) bottom=mix(bottom,pixel(b+ivec2(1,1)),weight.x);
        top=mix(top,bottom,weight.y);
    }
    return top;
}
void main() {
    vec2 p=vec2(vPosition.x,1.0-vPosition.y)*uCanvas;
    three=vec4(0.0);
    vec2 t=(p-uThreeRect0.xy)/max(uThreeRect0.zw,vec2(1e-6));
    vec2 u=(p-uThreeRect1.xy)/max(uThreeRect1.zw,vec2(1e-6));
    if(uThreeRect0.z>0.0&&all(greaterThanEqual(t,vec2(0.0)))&&all(lessThan(t,vec2(1.0)))) three=texture(uThree0,vec2(t.x,1.0-t.y));
    else if(uThreeRect1.z>0.0&&all(greaterThanEqual(u,vec2(0.0)))&&all(lessThan(u,vec2(1.0)))) three=texture(uThree1,vec2(u.x,1.0-u.y));
    // Sharp bilinear: DS pixels stay square and crisp at any scale, with
    // one presented pixel of blending at their borders.
    vec2 cell=vec2(p.x-(uCanvas.x-256.0)*0.5,p.y)-0.5;
    float scale=uScale;
    if(uPlaced!=0) {
        // Each screen in its own rectangle; nothing elsewhere.
        vec2 a=(p-uScreen0.xy)/max(uScreen0.zw,vec2(1e-6)),b=(p-uScreen1.xy)/max(uScreen1.zw,vec2(1e-6));
        int s=-1;vec4 rect;vec2 at;
        if(uScreen0.z>0.0&&all(greaterThanEqual(a,vec2(0.0)))&&all(lessThan(a,vec2(1.0)))) {s=0;rect=uScreen0;at=a;}
        else if(uScreen1.z>0.0&&all(greaterThanEqual(b,vec2(0.0)))&&all(lessThan(b,vec2(1.0)))) {s=1;rect=uScreen1;at=b;}
        if(s<0) {oColor=vec4(0.0,0.0,0.0,1.0);return;}
        cell=at*vec2(256.0,192.0)+vec2(0.0,192.0*float(s))-0.5;scale=uScale*rect.z/256.0;
        rowFirst=192*s;rowLast=rowFirst+191;insetCell=true;
    }
    vec3 color=hdOver(sharp(cell,scale),cell);
    vec2 i=(p-uInset.xy)/max(uInset.zw,vec2(1e-6));
    if(uInset.z>0.0&&uInsetAlpha>0.0&&all(greaterThanEqual(i,vec2(0.0)))&&all(lessThan(i,vec2(1.0)))) {
        // The inset: the top screen at a reduced size, over its own 3D,
        // faded in once its picture is ready.
        three=texture(uThree1,vec2(i.x,1.0-i.y));
        rowFirst=384;rowLast=575;insetCell=true;
        vec2 insetPosition=i*vec2(256.0,192.0)+vec2(0.0,384.0)-0.5;
        color=mix(color,hdOver(sharp(insetPosition,uScale*uInset.z/256.0),insetPosition),uInsetAlpha);
    }
    oColor=vec4(color,1.0);
})";

unsigned compile(unsigned type,const char* source) {
    const auto shader=gl::CreateShader(type);
    gl::ShaderSource(shader,1,&source,nullptr);gl::CompileShader(shader);
    int ok=0;gl::GetShaderiv(shader,GL_COMPILE_STATUS,&ok);
    if(!ok) {
        char log[2048]{};gl::GetShaderInfoLog(shader,sizeof log,nullptr,log);gl::DeleteShader(shader);
        throw std::runtime_error(std::string("Shader compilation failed: ")+log);
    }
    return shader;
}
unsigned link(const char* vertex,const char* fragment) {
    const auto program=gl::CreateProgram(),v=compile(GL_VERTEX_SHADER,vertex),f=compile(GL_FRAGMENT_SHADER,fragment);
    gl::AttachShader(program,v);gl::AttachShader(program,f);gl::LinkProgram(program);
    gl::DeleteShader(v);gl::DeleteShader(f);
    int ok=0;gl::GetProgramiv(program,GL_LINK_STATUS,&ok);
    if(!ok) {
        char log[2048]{};gl::GetProgramInfoLog(program,sizeof log,nullptr,log);gl::DeleteProgram(program);
        throw std::runtime_error(std::string("Shader link failed: ")+log);
    }
    return program;
}
int uniform(unsigned program,const char* name) {return gl::GetUniformLocation(program,name);}
float six(unsigned five) {return float(five?five*2+1:0);}
std::uint64_t hashPixels(const std::vector<std::uint32_t>& pixels) {
    std::uint64_t hash=1469598103934665603ull;
    for(auto p:pixels) {hash^=p;hash*=1099511628211ull;}
    return hash;
}
// Scale2x (EPX): doubles a pixel-art image, continuing its edges instead of blurring them.
std::vector<std::uint32_t> scale2x(const std::vector<std::uint32_t>& source,int width,int height) {
    std::vector<std::uint32_t> result(source.size()*4);
    auto at=[&](int x,int y){return source[std::size_t(std::clamp(y,0,height-1))*width+std::clamp(x,0,width-1)];};
    for(int y=0;y<height;++y)for(int x=0;x<width;++x) {
        const auto p=at(x,y),a=at(x,y-1),b=at(x+1,y),c=at(x-1,y),d=at(x,y+1);
        auto* out=result.data()+std::size_t(y*2)*width*2+x*2;
        out[0]=c==a&&c!=d&&a!=b?a:p;
        out[1]=a==b&&a!=c&&b!=d?b:p;
        out[width*2]=d==c&&d!=b&&c!=a?c:p;
        out[width*2+1]=b==d&&b!=a&&d!=c?d:p;
    }
    return result;
}
// A texture's pixels as uploaded: upscaled (Scale2x passes up to `scale`)
// and premultiplied.
std::vector<std::uint32_t> texturePixels(const Texture& texture,int scale,int& width,int& height) {
    auto pixels=texture.pixels;width=texture.width;height=texture.height;
    for(int factor=1;factor<scale&&width*height<=256*256;factor*=2) {
        pixels=scale2x(pixels,width,height);width*=2;height*=2;
    }
    for(auto& p:pixels) {
        const auto a=p>>24;
        auto channel=[&](unsigned shift){return (((p>>shift)&255)*a+127)/255<<shift;};
        p=(a<<24)|channel(16)|channel(8)|channel(0);
    }
    return pixels;
}
struct GpuVertex {float position[4],color[4],texture[4];std::uint32_t params[4];float clip[4],rect[4];};
struct Prepared {
    std::array<GpuVertex,3> vertices;
    unsigned texture=0,wrap=0,attributes=0;
    float minY=0,maxY=0;
    bool translucent=false;
    // Orthographic pixel art (2D maps, sprites) samples its upscaled texels
    // directly: filtering would blend neighbouring tiles of a texture atlas.
    bool crisp=false;
};
std::array<float,4> apply(const GpuMatrix& m,const std::array<float,4>& v) {
    std::array<float,4> r{};
    for(unsigned row=0;row<4;++row)r[row]=m[row*4]*v[0]+m[row*4+1]*v[1]+m[row*4+2]*v[2]+m[row*4+3]*v[3];
    return r;
}
}
GpuMatrix gpuIdentity() {return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};}
GpuMatrix gpuMultiply(const GpuMatrix& a,const GpuMatrix& b) {
    GpuMatrix r{};
    for(unsigned row=0;row<4;++row)for(unsigned col=0;col<4;++col)
        for(unsigned k=0;k<4;++k)r[row*4+col]+=a[row*4+k]*b[k*4+col];
    return r;
}
GpuMatrix gpuPlacement(float left,float top,float right,float bottom) {
    const float sx=(right-left)/2,sy=(top-bottom)/2,cx=(right+left)/2,cy=(top+bottom)/2;
    return {sx,0,0,cx,0,sy,0,cy,0,0,1,0,0,0,0,1};
}
GpuRenderer::GpuRenderer() {
    program_=link(sceneVertex,sceneFragment);
    fogProgram_=link(fullscreenVertex,fogFragment);
    composeProgram_=link(fullscreenVertex,composeFragment);
    reduceProgram_=link(fullscreenVertex,reduceFragment);
    overlayProgram_=link(overlayVertex,overlayFragment);
    gl::GenVertexArrays(1,&vertexArray_);gl::GenVertexArrays(1,&emptyArray_);gl::GenBuffers(1,&vertexBuffer_);
    gl::BindVertexArray(vertexArray_);gl::BindBuffer(GL_ARRAY_BUFFER,vertexBuffer_);
    for(unsigned i=0;i<3;++i) {
        gl::EnableVertexAttribArray(i);
        gl::VertexAttribPointer(i,4,GL_FLOAT,GL_FALSE,sizeof(GpuVertex),reinterpret_cast<void*>(std::uintptr_t(i*16)));
    }
    gl::EnableVertexAttribArray(3);
    gl::VertexAttribIPointer(3,4,GL_UNSIGNED_INT,sizeof(GpuVertex),reinterpret_cast<void*>(std::uintptr_t(48)));
    gl::EnableVertexAttribArray(4);
    gl::VertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,sizeof(GpuVertex),reinterpret_cast<void*>(std::uintptr_t(64)));
    gl::EnableVertexAttribArray(5);
    gl::VertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,sizeof(GpuVertex),reinterpret_cast<void*>(std::uintptr_t(80)));
    gl::BindVertexArray(0);
    gl::GenTextures(4,recipeTextures_);
    for(auto texture:recipeTextures_) {
        gl::BindTexture(GL_TEXTURE_2D,texture);
        gl::TexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,256,GpuLayout::recipeRows,0,GL_BGRA,GL_UNSIGNED_BYTE,nullptr);
        gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }
    gl::GenQueries(4,frameQueries_);
    gl::GenTextures(1,&hdAtlas_);gl::BindTexture(GL_TEXTURE_2D,hdAtlas_);
    gl::TexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,hdCellSize*hdAtlasCells,hdCellSize*hdAtlasCells,0,GL_BGRA,GL_UNSIGNED_BYTE,nullptr);
    gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    gl::GenTextures(1,&sideColumns_);gl::BindTexture(GL_TEXTURE_2D,sideColumns_);
    gl::TexImage2D(GL_TEXTURE_2D,0,GL_R32I,2,384,0,GL_RED_INTEGER,GL_INT,nullptr);
    gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}
GpuRenderer::~GpuRenderer() {
    for(auto& [key,texture]:textures_)gl::DeleteTextures(1,&texture.name);
    release(scene_[0]);release(scene_[1]);release(fogged_[0]);release(fogged_[1]);release(native_);release(output_);release(world_);
    if(readBuffers_[0])gl::DeleteBuffers(2,readBuffers_);
    if(queryActive_)gl::EndQuery(GL_TIME_ELAPSED);
    gl::DeleteQueries(4,frameQueries_);
    gl::DeleteTextures(4,recipeTextures_);gl::DeleteTextures(1,&sideColumns_);gl::DeleteTextures(1,&hdAtlas_);
    gl::DeleteBuffers(1,&vertexBuffer_);gl::DeleteVertexArrays(1,&vertexArray_);gl::DeleteVertexArrays(1,&emptyArray_);
    for(auto program:{program_,fogProgram_,composeProgram_,reduceProgram_,overlayProgram_})gl::DeleteProgram(program);
    for(auto& [id,picture]:overlays_)gl::DeleteTextures(1,&picture.name);
}
void GpuRenderer::drawOverlay(unsigned id,std::span<const std::uint32_t> pixels,int width,int height,bool changed,
    int x,int y,int windowWidth,int windowHeight) {
    auto& picture=overlays_[id];
    if(!picture.name) {
        gl::GenTextures(1,&picture.name);gl::BindTexture(GL_TEXTURE_2D,picture.name);
        gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        changed=true;
    }
    gl::ActiveTexture(GL_TEXTURE0);gl::BindTexture(GL_TEXTURE_2D,picture.name);
    if(changed||picture.width!=width||picture.height!=height) {
        gl::PixelStorei(GL_UNPACK_ALIGNMENT,4);
        gl::TexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_BGRA,GL_UNSIGNED_BYTE,pixels.data());
        picture.width=width;picture.height=height;
    }
    gl::BindFramebuffer(GL_FRAMEBUFFER,0);gl::Viewport(0,0,windowWidth,windowHeight);
    gl::UseProgram(overlayProgram_);gl::BindVertexArray(emptyArray_);
    gl::Uniform1i(uniform(overlayProgram_,"uPicture"),0);
    const float x0=2.f*x/windowWidth-1,x1=2.f*(x+width)/windowWidth-1,y0=1-2.f*(y+height)/windowHeight,y1=1-2.f*y/windowHeight;
    gl::Uniform4f(uniform(overlayProgram_,"uRect"),x0,y0,x1,y1);
    gl::Enable(GL_BLEND);gl::BlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    gl::DrawArrays(GL_TRIANGLE_STRIP,0,4);
    gl::Disable(GL_BLEND);
}
int GpuRenderer::supersample() const {return std::clamp(settings.autoQuality?std::min(settings.supersample,autoSupersample_):settings.supersample,1,4);}
void GpuRenderer::beginFrameTiming() {
    if(queryActive_||queriesPending_==4)return;
    gl::BeginQuery(GL_TIME_ELAPSED,frameQueries_[queryHead_]);queryActive_=true;
}
void GpuRenderer::endFrameTiming() {
    if(queryActive_) {gl::EndQuery(GL_TIME_ELAPSED);queryActive_=false;queryHead_=(queryHead_+1)%4;++queriesPending_;}
    // Results arrive a few frames later; collect those that are ready.
    while(queriesPending_) {
        const auto query=frameQueries_[(queryHead_+4-queriesPending_)%4];
        int available=0;gl::GetQueryObjectiv(query,GL_QUERY_RESULT_AVAILABLE,&available);
        if(!available)break;
        GLuint64 time=0;gl::GetQueryObjectui64v(query,GL_QUERY_RESULT,&time);
        --queriesPending_;gpuTime_+=double(time);++gpuSamples_;
        if(profiling())profileAdd("gpu-frame",double(time)/1000);
    }
    if(gpuSamples_<30)return;
    // Over 11 ms of GPU work per frame leaves too little margin for 60 frames
    // per second: lower the supersampling. Raise it again after a long quiet spell.
    const double milliseconds=gpuTime_/gpuSamples_/1e6;
    const int current=supersample();
    gpuTime_=0;gpuSamples_=0;
    if(!settings.autoQuality)return;
    // Lower after two busy half-seconds; raise when the next factor (cost
    // growing with its square) would still leave a wide margin.
    const double next=milliseconds*double((current+1)*(current+1))/double(current*current);
    // Thresholds for 60 frames per second (12 and 7 ms), scaled to the frame rate.
    const double budget=settings.frameMilliseconds/(1000.0/60);
    if(milliseconds>12*budget&&current>1) {
        quietWindows_=0;
        if(++busyWindows_>=2) {
            autoSupersample_=current-1;busyWindows_=0;
            std::printf("GPU %.1f ms par image: supersampling x%d\n",milliseconds,autoSupersample_);
        }
    } else if(next<7*budget&&current<settings.supersample) {
        busyWindows_=0;
        if(++quietWindows_>=4) {
            autoSupersample_=current+1;quietWindows_=0;
            std::printf("GPU %.1f ms par image: supersampling x%d\n",milliseconds,autoSupersample_);
        }
    } else quietWindows_=busyWindows_=0;
}
void GpuRenderer::keepWorld() {
    const auto& source=fogApplied_[0]?fogged_[0]:scene_[0];
    if(!source.framebuffer)return;
    resize(world_,source.width,source.height,false,false);
    gl::BindFramebuffer(GL_READ_FRAMEBUFFER,source.framebuffer);gl::BindFramebuffer(GL_DRAW_FRAMEBUFFER,world_.framebuffer);
    gl::BlitFramebuffer(0,0,source.width,source.height,0,0,source.width,source.height,GL_COLOR_BUFFER_BIT,GL_NEAREST);
    gl::BindFramebuffer(GL_FRAMEBUFFER,0);
}
unsigned GpuRenderer::threeColor(unsigned slot) const {return fogApplied_[slot]?fogged_[slot].color:scene_[slot].color;}
void GpuRenderer::release(Target& target) {
    if(target.framebuffer)gl::DeleteFramebuffers(1,&target.framebuffer);
    for(auto* texture:{&target.color,&target.attributes,&target.depth})if(*texture)gl::DeleteTextures(1,texture);
    target=Target{};
}
void GpuRenderer::resize(Target& target,int width,int height,bool attributes,bool depth) {
    if(target.framebuffer&&target.width==width&&target.height==height)return;
    release(target);
    target.width=width;target.height=height;
    auto texture=[&](unsigned& name,int internal,unsigned format,unsigned type,bool linear) {
        gl::GenTextures(1,&name);gl::BindTexture(GL_TEXTURE_2D,name);
        gl::TexImage2D(GL_TEXTURE_2D,0,internal,width,height,0,format,type,nullptr);
        gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,linear?GL_LINEAR:GL_NEAREST);
        gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,linear?GL_LINEAR:GL_NEAREST);
        gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    };
    gl::GenFramebuffers(1,&target.framebuffer);gl::BindFramebuffer(GL_FRAMEBUFFER,target.framebuffer);
    texture(target.color,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,true);
    gl::FramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,target.color,0);
    if(attributes) {
        texture(target.attributes,GL_RGBA8,GL_RGBA,GL_UNSIGNED_BYTE,false);
        gl::FramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,target.attributes,0);
    }
    if(depth) {
        texture(target.depth,GL_DEPTH24_STENCIL8,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,false);
        gl::FramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D,target.depth,0);
    }
    const unsigned buffers[2]={GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
    gl::DrawBuffers(attributes?2:1,buffers);
    if(gl::CheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)throw std::runtime_error("Incomplete OpenGL framebuffer");
}
unsigned GpuRenderer::upload(const Texture& texture,std::uint64_t key) {
    auto& entry=textures_[key];entry.lastUse=renders_;
    // Decoded pixels only change with a new decode (a new version).
    if(entry.name&&entry.generation==texture.version)return entry.name;
    entry.generation=texture.version;
    ProfileScope timing("texture-check");
    const auto hash=hashPixels(texture.pixels);
    if(entry.name&&entry.hash==hash&&entry.width==texture.width&&entry.height==texture.height)return entry.name;
    int width=0,height=0;
    const auto premultiplied=texturePixels(texture,settings.textureScale,width,height);
    finishUpload(entry,premultiplied,width,height,hash,texture,true);
    return entry.name;
}
void GpuRenderer::finishUpload(GpuTexture& entry,const std::vector<std::uint32_t>& premultiplied,int width,int height,
    std::uint64_t hash,const Texture& texture,bool mipmaps) {
    ProfileScope uploadTiming("texture-upload");
    if(!entry.name)gl::GenTextures(1,&entry.name);
    gl::BindTexture(GL_TEXTURE_2D,entry.name);
    gl::PixelStorei(GL_UNPACK_ALIGNMENT,4);
    gl::TexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_BGRA,GL_UNSIGNED_BYTE,premultiplied.data());
    // Mipmaps and anisotropic filtering for surfaces seen at grazing angles;
    // a provisional texture (upscaled in a later frame) has its level 0 only.
    gl::TexParameteri(GL_TEXTURE_2D,0x813d,mipmaps?1000:0); // GL_TEXTURE_MAX_LEVEL
    if(mipmaps)gl::GenerateMipmap(GL_TEXTURE_2D);
    if(!anisotropyChecked_) {
        gl::TexParameteri(GL_TEXTURE_2D,0x84fe,8); // GL_TEXTURE_MAX_ANISOTROPY, where supported
        anisotropy_=gl::GetError()==GL_NO_ERROR;anisotropyChecked_=true;
        while(gl::GetError()!=GL_NO_ERROR) {}
    } else if(anisotropy_)gl::TexParameteri(GL_TEXTURE_2D,0x84fe,8);
    entry.hash=hash;entry.width=texture.width;entry.height=texture.height;entry.generation=texture.version;
}
void GpuRenderer::prepareTextures(std::span<const GpuBatch> batches,GxTextures& textures,const GxRenderOptions& options) {
    // The textures this frame shows for the first time (a new map brings
    // dozens): their upscaling runs in parallel before the uploads.
    if(!(options.displayControl&1))return;
    struct Pending {std::uint64_t key;GpuTexture* entry;const Texture* texture;std::uint64_t hash;std::vector<std::uint32_t> pixels;int width=0,height=0;};
    std::vector<Pending> pending;
    std::uint64_t lastKey=~0ull;
    for(const auto& batch:batches)for(const auto& triangle:batch.triangles) {
        if(!(triangle.perspective?batch.perspective:batch.orthographic))continue;
        if(batch.layer>=0&&triangle.layer!=batch.layer)continue;
        const auto parameters=triangle.material.texture,format=(parameters>>26)&7;
        if(!format||(format==7&&batch.worldScreen>=0))continue;
        const std::uint64_t key=(std::uint64_t(triangle.material.palette)<<32)|(parameters&0x3ff0ffffu);
        if(key==lastKey)continue;
        lastKey=key;
        auto& entry=textures_[key];
        const Texture* texture=nullptr;
        try {texture=&textures.texture(parameters,triangle.material.palette);} catch(const std::exception&) {continue;}
        if(entry.name&&entry.generation==texture->version)continue;
        if(std::any_of(pending.begin(),pending.end(),[&](const Pending& p){return p.entry==&entry;}))continue;
        const auto hash=hashPixels(texture->pixels);
        if(entry.name&&entry.hash==hash&&entry.width==texture->width&&entry.height==texture->height) {
            entry.generation=texture->version;
            continue;
        }
        pending.push_back({key,&entry,texture,hash});
    }
    // A new map shows dozens of new textures at once: a few are uploaded
    // upscaled, the others as they are, upscaled over the next frames.
    constexpr std::size_t fullPerFrame=2;
    ProfileScope timing("texture-prepare");
    std::for_each(std::execution::par,pending.begin(),pending.end(),[&](Pending& p) {
        const bool full=std::size_t(&p-pending.data())<fullPerFrame;
        p.pixels=texturePixels(*p.texture,full?settings.textureScale:1,p.width,p.height);
    });
    for(auto& p:pending) {
        const bool provisional=std::size_t(&p-pending.data())>=fullPerFrame&&settings.textureScale>1;
        finishUpload(*p.entry,p.pixels,p.width,p.height,p.hash,*p.texture,!provisional);
        if(provisional)
            upgrades_.push_back({p.key,*p.texture,p.hash});
    }
}
void GpuRenderer::upgradeTextures() {
    // Textures first uploaded as they are: a few more upscaled each frame.
    constexpr std::size_t perFrame=4;
    if(upgrades_.empty())return;
    ProfileScope timing("texture-upgrade");
    const std::size_t count=std::min(perFrame,upgrades_.size());
    std::vector<std::vector<std::uint32_t>> pixels(count);
    std::vector<std::array<int,2>> sizes(count);
    std::vector<std::size_t> indices(count);
    for(std::size_t i=0;i<count;++i)indices[i]=i;
    std::for_each(std::execution::par,indices.begin(),indices.end(),[&](std::size_t i) {
        pixels[i]=texturePixels(upgrades_[i].texture,settings.textureScale,sizes[i][0],sizes[i][1]);
    });
    for(std::size_t i=0;i<count;++i) {
        auto& upgrade=upgrades_[i];
        // Unless the texture was replaced or released meanwhile.
        const auto found=textures_.find(upgrade.key);
        if(found!=textures_.end()&&found->second.hash==upgrade.hash&&found->second.generation==upgrade.texture.version)
            finishUpload(found->second,pixels[i],sizes[i][0],sizes[i][1],upgrade.hash,upgrade.texture,true);
    }
    upgrades_.erase(upgrades_.begin(),upgrades_.begin()+std::ptrdiff_t(count));
}
void GpuRenderer::render3D(std::span<const GpuBatch> batches,const GxRasterState& state,GxTextures& textures,
    const GxRenderOptions& options,int width,int height,const GpuFogSplit& fog,unsigned slot) {
    // Supersampling stays within a 4096-pixel target (4K windows render at 1x).
    beginFrameTiming();
    int ss=supersample();
    while(ss>1&&std::max(width,height)*ss>4096)--ss;
    auto& fogged=fogged_[slot&1];lastSlot_=slot&1;++renders_;
    auto& scene=scene_[slot&1];
    resize(scene,width*ss,height*ss,true,true);resize(fogged,width*ss,height*ss,false,false);
    if(!betweenFrame) {
        prepareTextures(batches,textures,options);
        upgradeTextures();
    }
    std::vector<Prepared> polygons;
    std::optional<ProfileScope> stage(std::in_place,"r3d-prepare");
    std::uint32_t cachedViewport=0;const GpuMatrix* cachedTarget=nullptr;GpuMatrix cachedMatrix{};
    std::uint64_t cachedTexture=~0ull;unsigned cachedName=0;float cachedSize[2]={1,1};
    for(const auto& batch:batches)for(const auto& triangle:batch.triangles) {
        if(!(triangle.perspective?batch.perspective:batch.orthographic))continue;
        if(batch.layer>=0&&triangle.layer!=batch.layer)continue;
        const auto& placement=batch.placement;
        const auto attributes=triangle.material.polygon,alpha=(attributes>>16)&31;
        if(!(attributes&0xc0)||!alpha)continue;
        bool outsideFar=false;
        for(const auto& v:triangle.vertices)outsideFar|=v.clip[2]>v.clip[3];
        if(outsideFar&&!(attributes&(1<<12)))continue;
        const auto viewport=triangle.viewport;
        const float vx=float(viewport&255),vy=float((viewport>>8)&255);
        const float vw=float(((viewport>>16)&255)+1)-vx,vh=float((viewport>>24)+1)-vy;
        if(vw<=0||vh<=0)continue;
        const GpuMatrix viewportMatrix{vw/256,0,0,vx/128+vw/256-1, 0,vh/192,0,vy/96+vh/192-1, 0,0,1,0, 0,0,0,1};
        const GpuMatrix* target=&placement.world;
        if(!triangle.perspective) {
            float left=1e9f,right=-1e9f;
            for(const auto& v:triangle.vertices) {
                const auto p=apply(viewportMatrix,v.clip);
                if(p[3]!=0) {left=std::min(left,p[0]/p[3]);right=std::max(right,p[0]/p[3]);}
            }
            // Only plain full-width quads (fades, flashes) widen; textured ones are pictures.
            bool plain=!((triangle.material.texture>>26)&7)||!(options.displayControl&1);
            if(!plain) {
                // A texture stretched from a texel or two is a flat colour too.
                float u0=1e9f,u1=-1e9f,v0=1e9f,v1=-1e9f;
                for(const auto& v:triangle.vertices) {u0=std::min(u0,v.u);u1=std::max(u1,v.u);v0=std::min(v0,v.v);v1=std::max(v1,v.v);}
                plain=u1-u0<=2&&v1-v0<=2;
            }
            target=plain&&left<=-0.999f&&right>=0.999f?&placement.screenWide:&placement.screen;
        }
        // Consecutive triangles mostly share their viewport and placement.
        if(viewport!=cachedViewport||target!=cachedTarget) {
            cachedMatrix=gpuMultiply(*target,viewportMatrix);cachedViewport=viewport;cachedTarget=target;
        }
        const auto& matrix=cachedMatrix;
        Prepared polygon;polygon.attributes=attributes;
        const auto parameters=triangle.material.texture,format=(parameters>>26)&7,mode=(attributes>>4)&3;
        const bool textured=format&&(options.displayControl&1);
        float size[2]={1,1};
        // A screen effect's captured picture: the kept world picture instead,
        // its rows from the top (render targets store rows bottom-up).
        const bool fromWorld=textured&&batch.worldScreen>=0&&world_.color&&format==7&&
            ((parameters>>20)&7)==5&&((parameters>>23)&7)==5;
        float worldRow=0;
        if(fromWorld) {
            polygon.texture=world_.color;size[0]=256;size[1]=-384;worldRow=192.f*float(batch.worldScreen)-384;
            polygon.wrap=0;polygon.crisp=true;
        } else if(textured) {
            const std::uint64_t key=(std::uint64_t(triangle.material.palette)<<32)|(parameters&0x3ff0ffffu);
            if(key!=cachedTexture) {
                const auto& decoded=textures.texture(parameters,triangle.material.palette);
                cachedName=upload(decoded,key);cachedSize[0]=float(decoded.width);cachedSize[1]=float(decoded.height);cachedTexture=key;
            }
            polygon.texture=cachedName;size[0]=cachedSize[0];size[1]=cachedSize[1];
            polygon.wrap=(parameters>>16)&15;
            polygon.crisp=!triangle.perspective;
        }
        polygon.translucent=(alpha<31)||(textured&&!(mode&1)&&(format==1||format==6));
        bool visible=true,allFront=true;
        float screen[3][2]{};
        polygon.minY=1e9f;polygon.maxY=-1e9f;
        // The texels the triangle covers: a texture that doesn't repeat is
        // sampled only inside them (parts of a sprite sheet).
        float rect[4]={1e9f,1e9f,-1e9f,-1e9f};
        for(const auto& v:triangle.vertices) {
            rect[0]=std::min(rect[0],v.u);rect[1]=std::min(rect[1],v.v+worldRow);
            rect[2]=std::max(rect[2],v.u);rect[3]=std::max(rect[3],v.v+worldRow);
        }
        const bool keepInside=textured&&!fromWorld&&!(((parameters>>16)&3));
        // Pictures cut from a sheet (a monster's parts, characters): drawn at
        // a higher resolution, parts meeting on a DS pixel edge can leave a
        // hairline between them. Opaque ones grow by half a DS pixel all
        // around, their texture carried on (the margin repeats their edge
        // texels), so neighbouring parts overlap instead.
        std::array<GxVertex,3> grown=triangle.vertices;
        if(keepInside&&!polygon.translucent) {
            const auto& t=triangle.vertices;
            bool front=true;
            for(unsigned i=0;i<3&&front;++i)front=t[i].clip[3]>0;
            if(front) {
                // DS screen pixels (texture coordinates taken as affine over half a pixel).
                float x[3],y[3];
                for(unsigned i=0;i<3;++i) {x[i]=t[i].clip[0]/t[i].clip[3]*128;y[i]=t[i].clip[1]/t[i].clip[3]*96;}
                const float ex1=x[1]-x[0],ey1=y[1]-y[0],ex2=x[2]-x[0],ey2=y[2]-y[0];
                const float det=ex1*ey2-ex2*ey1;
                if(std::abs(det)>0.5f) {
                    // Texture coordinates follow the screen (affine on a card).
                    const float du1=t[1].u-t[0].u,dv1=t[1].v-t[0].v,du2=t[2].u-t[0].u,dv2=t[2].v-t[0].v;
                    const float dudx=(du1*ey2-du2*ey1)/det,dudy=(du2*ex1-du1*ex2)/det;
                    const float dvdx=(dv1*ey2-dv2*ey1)/det,dvdy=(dv2*ex1-dv1*ex2)/det;
                    const float side=det>0?1.f:-1.f;
                    constexpr float grow=0.5f;
                    for(unsigned i=0;i<3;++i) {
                        // Each corner moves out along both its edges' normals.
                        const unsigned a=(i+2)%3,b=(i+1)%3;
                        auto normal=[&](unsigned from,unsigned to,float& nx,float& ny) {
                            const float dx=x[to]-x[from],dy=y[to]-y[from],length=std::sqrt(dx*dx+dy*dy);
                            nx=side*dy/length;ny=-side*dx/length;
                        };
                        float n1x,n1y,n2x,n2y;normal(a,i,n1x,n1y);normal(i,b,n2x,n2y);
                        const float dot=std::max(n1x*n2x+n1y*n2y,-0.5f);
                        const float ox=grow*(n1x+n2x)/(1+dot),oy=grow*(n1y+n2y)/(1+dot);
                        grown[i].clip[0]+=ox/128*t[i].clip[3];grown[i].clip[1]+=oy/96*t[i].clip[3];
                        grown[i].u+=dudx*ox+dudy*oy;grown[i].v+=dvdx*ox+dvdy*oy;
                    }
                }
            }
        }
        for(unsigned i=0;i<3;++i) {
            const auto& v=grown[i];
            const auto p=apply(matrix,v.clip);
            auto& out=polygon.vertices[i];
            std::copy(p.begin(),p.end(),out.position);
            for(unsigned c=0;c<3;++c)out.color[c]=six((v.color>>(5*c))&31)/63.f;
            out.color[3]=1;
            out.texture[0]=v.u;out.texture[1]=v.v+worldRow;out.texture[2]=size[0];out.texture[3]=size[1];
            out.params[0]=attributes;out.params[1]=parameters;
            // Overlays placed on one screen of a larger target stop at that
            // screen's edges, as the hardware clips them.
            if(target==&placement.screen) {
                const float w=v.clip[3];
                out.clip[0]=w+v.clip[0];out.clip[1]=w-v.clip[0];out.clip[2]=w+v.clip[1];out.clip[3]=w-v.clip[1];
            } else out.clip[0]=out.clip[1]=out.clip[2]=out.clip[3]=1;
            out.params[2]=(textured?1u:0u)|(polygon.translucent?0u:2u)|(keepInside?4u:0u);out.params[3]=0;
            std::copy(rect,rect+4,out.rect);
            if(p[3]<=0)allFront=false;
            else {screen[i][0]=p[0]/p[3];screen[i][1]=-p[1]/p[3];}
            const float y=p[3]>0?-p[1]/p[3]:0;
            polygon.minY=std::min(polygon.minY,y);polygon.maxY=std::max(polygon.maxY,y);
        }
        if(allFront) {
            // Orientation in a y-down screen, as the hardware tests it.
            const float area=(screen[1][0]-screen[0][0])*(screen[2][1]-screen[0][1])-(screen[1][1]-screen[0][1])*(screen[2][0]-screen[0][0]);
            if(area==0)visible=false;
            else if(area<0?!(attributes&0x80):!(attributes&0x40))visible=false;
        }
        if(visible)polygons.push_back(polygon);
    }
    stage.emplace("r3d-sort");
    // Opaque polygons first in submission order, then translucent ones by
    // their screen extent (unless the game sorts them itself).
    std::vector<std::uint32_t> order;order.reserve(polygons.size());
    for(std::uint32_t i=0;i<polygons.size();++i)if(!polygons[i].translucent)order.push_back(i);
    const auto opaque=order.size();
    for(std::uint32_t i=0;i<polygons.size();++i)if(polygons[i].translucent)order.push_back(i);
    if(!options.manualSort)
        std::stable_sort(order.begin()+std::ptrdiff_t(opaque),order.end(),[&](std::uint32_t i,std::uint32_t j) {
            const auto& a=polygons[i];const auto& b=polygons[j];
            return a.maxY!=b.maxY?a.maxY<b.maxY:a.minY<b.minY;
        });
    std::vector<GpuVertex> vertices;vertices.reserve(polygons.size()*3);
    for(const auto i:order)vertices.insert(vertices.end(),polygons[i].vertices.begin(),polygons[i].vertices.end());

    stage.emplace("r3d-draw");
    gl::BindFramebuffer(GL_FRAMEBUFFER,scene.framebuffer);
    gl::Viewport(0,0,scene.width,scene.height);
    gl::Disable(GL_SCISSOR_TEST);gl::Disable(GL_CULL_FACE);
    gl::ColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);gl::DepthMask(GL_TRUE);gl::StencilMask(0xff);
    const unsigned clear=state.clearColor,clearAlpha=(clear>>16)&31;
    const float ca=clearAlpha/31.f;
    const float clearColor[4]={six(clear&31)/63.f*ca,six((clear>>5)&31)/63.f*ca,six((clear>>10)&31)/63.f*ca,ca};
    const float clearAttributes[4]={(clear>>15)&1?1.f:0.f,float((clear>>24)&63)/63.f,0,1};
    gl::ClearBufferfv(GL_COLOR,0,clearColor);gl::ClearBufferfv(GL_COLOR,1,clearAttributes);
    gl::ClearBufferfi(GL_DEPTH_STENCIL,0,float((unsigned(state.clearDepth)<<9)|511)/16777215.f,0);
    if(!vertices.empty()) {
        gl::UseProgram(program_);
        gl::Uniform1i(uniform(program_,"uTexture"),0);
        gl::Uniform1i(uniform(program_,"uControl"),int(options.displayControl));
        gl::Uniform1f(uniform(program_,"uAlphaReference"),float(options.alphaTest&31));
        gl::Uniform1i(uniform(program_,"uWBuffer"),options.wBuffer?1:0);
        float toon[96];
        for(unsigned i=0;i<32;++i)for(unsigned c=0;c<3;++c)toon[i*3+c]=six((state.toonTable[i]>>(5*c))&31);
        gl::Uniform3fv(uniform(program_,"uToon"),32,toon);
        const int passLocation=uniform(program_,"uPass");
        gl::BindVertexArray(vertexArray_);gl::BindBuffer(GL_ARRAY_BUFFER,vertexBuffer_);
        gl::BufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GpuVertex),vertices.data(),GL_STREAM_DRAW);
        gl::Enable(GL_DEPTH_TEST);gl::Enable(GL_STENCIL_TEST);gl::ActiveTexture(GL_TEXTURE0);
        for(unsigned plane=0;plane<4;++plane)gl::Enable(GL_CLIP_DISTANCE0+plane);
        const bool blending=(options.displayControl&8)!=0;
        const int filter=settings.smoothTextures?GL_LINEAR:GL_NEAREST;
        std::size_t start=0;
        while(start<order.size()) {
            const auto& first=polygons[order[start]];
            const auto attributes=first.attributes,mode=(attributes>>4)&3,id=(attributes>>24)&63;
            const unsigned kind=mode==3?(id?2:1):0;
            auto same=[&](const Prepared& p) {
                const auto a=p.attributes,m=(a>>4)&3,i=(a>>24)&63;
                return p.texture==first.texture&&p.wrap==first.wrap&&p.crisp==first.crisp&&p.translucent==first.translucent&&
                    (m==3?(i?2u:1u):0u)==kind&&((a^attributes)&((1u<<14)|(1u<<11)))==0&&(!first.translucent&&kind==0||i==id);
            };
            std::size_t end=start+1;while(end<order.size()&&same(polygons[order[end]]))++end;
            if(first.texture) {
                gl::BindTexture(GL_TEXTURE_2D,first.texture);
                const bool smooth=settings.smoothTextures&&!first.crisp;
                // Sampling parameters are only sent when they change for this texture.
                const unsigned sampling=first.wrap|(smooth?16u:0u)|(filter==GL_LINEAR?32u:0u);
                auto& known=samplingState_[first.texture];
                if(known!=sampling+1) {
                    known=sampling+1;
                    auto wrap=[&](bool repeat,bool flip){return repeat?(flip?GL_MIRRORED_REPEAT:GL_REPEAT):GL_CLAMP_TO_EDGE;};
                    gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap(first.wrap&1,first.wrap&4));
                    gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap(first.wrap&2,first.wrap&8));
                    gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,smooth?GL_LINEAR_MIPMAP_LINEAR:GL_NEAREST);
                    gl::TexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,smooth?filter:GL_NEAREST);
                }
            }
            gl::DepthFunc(GL_LESS);
            gl::ColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            if(kind==1) {
                // Shadow volume mask: flag pixels where the depth test fails.
                gl::Uniform1i(passLocation,1);gl::Disable(GL_BLEND);gl::ColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                gl::DepthMask(GL_FALSE);gl::StencilFunc(GL_ALWAYS,0x80,0xff);gl::StencilMask(0x80);
                gl::StencilOp(GL_KEEP,GL_REPLACE,GL_KEEP);
            } else if(kind==2) {
                gl::Uniform1i(passLocation,1);
                if(first.translucent&&blending) {gl::Enable(GL_BLEND);gl::BlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);} else gl::Disable(GL_BLEND);
                gl::DepthMask((attributes&(1<<11))?GL_TRUE:GL_FALSE);
                gl::StencilFunc(GL_EQUAL,0x80,0x80);gl::StencilMask(0x80);gl::StencilOp(GL_KEEP,GL_KEEP,GL_ZERO);
            } else if(first.translucent) {
                gl::Uniform1i(passLocation,1);
                if(blending) {gl::Enable(GL_BLEND);gl::BlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);} else gl::Disable(GL_BLEND);
                gl::DepthMask((attributes&(1<<11))?GL_TRUE:GL_FALSE);
                gl::StencilFunc(GL_NOTEQUAL,int(0x40|id),0x7f);gl::StencilMask(0x7f);gl::StencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
            } else {
                gl::Uniform1i(passLocation,0);gl::Disable(GL_BLEND);gl::DepthMask(GL_TRUE);
                gl::StencilFunc(GL_ALWAYS,0,0xff);gl::StencilMask(0x7f);gl::StencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
            }
            gl::DrawArrays(GL_TRIANGLES,int(start*3),int((end-start)*3));
            start=end;
        }
        gl::Disable(GL_BLEND);gl::Disable(GL_DEPTH_TEST);gl::Disable(GL_STENCIL_TEST);
        for(unsigned plane=0;plane<4;++plane)gl::Disable(GL_CLIP_DISTANCE0+plane);
        gl::DepthMask(GL_TRUE);gl::StencilMask(0xff);gl::ColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    }
    gpuCheckpoint("gpu-scene");
    for(auto it=textures_.begin();it!=textures_.end();) {
        // Textures unused for a few seconds are released.
        if(renders_-it->second.lastUse>240) {
            samplingState_.erase(it->second.name);gl::DeleteTextures(1,&it->second.name);it=textures_.erase(it);
        } else ++it;
    }
    // Fog uses the stored depth and the per-pixel fog flag. Without fog the
    // scene itself is the result.
    const GxRasterState* fogs[2]={&state,fog.upper?fog.upper:&state};
    fogApplied_[slot&1]=fogs[0]->fogEnabled||fogs[1]->fogEnabled;
    if(!fogApplied_[slot&1])return;
    gl::BindFramebuffer(GL_FRAMEBUFFER,fogged.framebuffer);gl::Viewport(0,0,fogged.width,fogged.height);
    gl::UseProgram(fogProgram_);gl::BindVertexArray(emptyArray_);
    gl::ActiveTexture(GL_TEXTURE0);gl::BindTexture(GL_TEXTURE_2D,scene.color);
    gl::ActiveTexture(GL_TEXTURE1);gl::BindTexture(GL_TEXTURE_2D,scene.depth);
    gl::ActiveTexture(GL_TEXTURE2);gl::BindTexture(GL_TEXTURE_2D,scene.attributes);
    gl::Uniform1i(uniform(fogProgram_,"uColor"),0);gl::Uniform1i(uniform(fogProgram_,"uDepth"),1);gl::Uniform1i(uniform(fogProgram_,"uAttributes"),2);
    int enabled[2],alphaOnly[2];float colors[8],table[64],offsets[2],steps[2];
    for(unsigned i=0;i<2;++i) {
        const auto& f=*fogs[i];const auto c=f.fogColor;
        enabled[i]=f.fogEnabled?1:0;alphaOnly[i]=f.fogAlphaOnly?1:0;
        colors[i*4]=six(c&31)/63.f;colors[i*4+1]=six((c>>5)&31)/63.f;colors[i*4+2]=six((c>>10)&31)/63.f;colors[i*4+3]=((c>>16)&31)/31.f;
        for(unsigned n=0;n<32;++n)table[i*32+n]=f.fogTable[n]==127?128.f:float(f.fogTable[n]&127);
        offsets[i]=float(f.fogOffset);steps[i]=float(0x400u>>std::min(15u,f.fogSlope));
    }
    gl::Uniform1iv(uniform(fogProgram_,"uEnabled"),2,enabled);gl::Uniform1iv(uniform(fogProgram_,"uAlphaOnly"),2,alphaOnly);
    gl::Uniform4fv(uniform(fogProgram_,"uFogColor"),2,colors);gl::Uniform1fv(uniform(fogProgram_,"uFogTable"),64,table);
    gl::Uniform1fv(uniform(fogProgram_,"uFogOffset"),2,offsets);gl::Uniform1fv(uniform(fogProgram_,"uFogStep"),2,steps);
    gl::Uniform1f(uniform(fogProgram_,"uSplit"),fog.split*float(fogged.height));
    gl::Uniform1f(uniform(fogProgram_,"uBand"),fog.band*float(fogged.height));
    gl::DrawArrays(GL_TRIANGLE_STRIP,0,4);
    gl::ActiveTexture(GL_TEXTURE0);
    gpuCheckpoint("gpu-fog");
}
void GpuRenderer::readNative(std::vector<std::uint32_t>& pixels,bool immediate,const std::array<float,4>& source,unsigned key) {
    ProfileScope timing(immediate?"readNative-sync":"readNative");
    resize(native_,256,192,false,false);
    gl::BindFramebuffer(GL_FRAMEBUFFER,native_.framebuffer);gl::Viewport(0,0,256,192);
    gl::UseProgram(reduceProgram_);gl::BindVertexArray(emptyArray_);
    const auto& fogged=fogged_[lastSlot_];
    gl::ActiveTexture(GL_TEXTURE0);gl::BindTexture(GL_TEXTURE_2D,threeColor(lastSlot_));
    gl::Uniform1i(uniform(reduceProgram_,"uSource"),0);
    // Source rectangle in target fractions, y from the top (GL rows start at the bottom).
    gl::Uniform2f(uniform(reduceProgram_,"uOrigin"),source[0]*fogged.width,(1-source[1]-source[3])*fogged.height);
    gl::Uniform2f(uniform(reduceProgram_,"uScale"),source[2]*fogged.width/256.f,source[3]*fogged.height/192.f);
    gl::DrawArrays(GL_TRIANGLE_STRIP,0,4);
    std::vector<std::uint32_t> raw(256*192);
    gl::PixelStorei(GL_PACK_ALIGNMENT,4);
    key&=1;
    if(immediate||!readPending_[key]) gl::ReadPixels(0,0,256,192,GL_BGRA,GL_UNSIGNED_BYTE,raw.data());
    if(immediate) readPending_[key]=false;
    else {
        if(!readBuffers_[0]) {
            gl::GenBuffers(2,readBuffers_);
            for(auto buffer:readBuffers_) {
                gl::BindBuffer(GL_PIXEL_PACK_BUFFER,buffer);
                gl::BufferData(GL_PIXEL_PACK_BUFFER,raw.size()*4,nullptr,GL_STREAM_READ);
            }
        }
        // Each key (screen) collects what its previous call queued, then queues this frame.
        gl::BindBuffer(GL_PIXEL_PACK_BUFFER,readBuffers_[key]);
        if(readPending_[key]) {
            if(const auto* mapped=static_cast<const std::uint32_t*>(gl::MapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY))) {
                std::copy_n(mapped,raw.size(),raw.data());gl::UnmapBuffer(GL_PIXEL_PACK_BUFFER);
            }
        }
        gl::ReadPixels(0,0,256,192,GL_BGRA,GL_UNSIGNED_BYTE,nullptr);
        gl::BindBuffer(GL_PIXEL_PACK_BUFFER,0);
        readPending_[key]=true;
    }
    pixels.resize(raw.size());
    for(unsigned y=0;y<192;++y)for(unsigned x=0;x<256;++x) {
        const auto p=raw[(191-y)*256+x],a=p>>24;
        auto channel=[&](unsigned shift){return a?std::min(255u,(((p>>shift)&255)*255+a/2)/a):0u;};
        pixels[y*256+x]=(a<<24)|(channel(16)<<16)|(channel(8)<<8)|channel(0);
    }
}
void GpuRenderer::present(const GpuLayout& layout,std::span<const std::uint32_t> recipes,int windowWidth,int windowHeight,bool stretch) {
    constexpr std::size_t plane=std::size_t(256)*GpuLayout::recipeRows;
    if(recipes.size()!=plane*4)throw std::runtime_error("Composition recipes have an unexpected size");
    for(unsigned plane=0;plane<4;++plane) {
        gl::BindTexture(GL_TEXTURE_2D,recipeTextures_[plane]);
        gl::PixelStorei(GL_UNPACK_ALIGNMENT,4);
        gl::TexSubImage2D(GL_TEXTURE_2D,0,0,0,256,GpuLayout::recipeRows,GL_BGRA,GL_UNSIGNED_BYTE,recipes.data()+plane*std::size_t(256)*GpuLayout::recipeRows);
    }
    std::optional<ProfileScope> stage(std::in_place,"present-upload");
    for(auto& upload:takeHdUploads()) {
        // Scale2x twice, then premultiplied for filtering.
        auto pixels=scale2x(scale2x(upload.pixels,hdSourceSize,hdSourceSize),hdSourceSize*2,hdSourceSize*2);
        for(auto& p:pixels) {
            const unsigned a=p>>24;
            auto channel=[&](unsigned shift){return (((p>>shift)&255)*a+127)/255;};
            p=(a<<24)|(channel(16)<<16)|(channel(8)<<8)|channel(0);
        }
        gl::BindTexture(GL_TEXTURE_2D,hdAtlas_);gl::PixelStorei(GL_UNPACK_ALIGNMENT,4);
        gl::TexSubImage2D(GL_TEXTURE_2D,0,(upload.cell%hdAtlasCells)*hdCellSize,(upload.cell/hdAtlasCells)*hdCellSize,
            hdCellSize,hdCellSize,GL_BGRA,GL_UNSIGNED_BYTE,pixels.data());
    }
    if(layout.sides) {
        // Per row, the column whose 3D effects continue beside the screens: the
        // nearest one showing the 3D layer directly (recipe kind 1), searched
        // from that side for 33 columns, then from the other edge for 32.
        std::array<std::int32_t,2*384> columns;
        auto direct=[&](unsigned y,int x){return (recipes[y*256+x]>>24)==1;};
        for(unsigned y=0;y<384;++y)for(unsigned side=0;side<2;++side) {
            int found=-1;
            for(int i=0;i<65&&found<0;++i) {
                const int along=i<33?i:255-(i-33),x=side?255-along:along;
                if(direct(y,x))found=x;
            }
            columns[y*2+side]=found;
        }
        gl::BindTexture(GL_TEXTURE_2D,sideColumns_);
        gl::TexSubImage2D(GL_TEXTURE_2D,0,0,0,2,384,GL_RED_INTEGER,GL_INT,columns.data());
    }
    stage.emplace("present-draw");
    beginFrameTiming();
    // A new window size starts again from the highest quality.
    if(output_.width!=windowWidth||output_.height!=windowHeight) {autoSupersample_=2;gpuTime_=0;gpuSamples_=0;}
    // Composed off screen, so a hidden window can still be read back.
    resize(output_,windowWidth,windowHeight,false,false);
    gl::BindFramebuffer(GL_FRAMEBUFFER,output_.framebuffer);
    gl::Viewport(0,0,windowWidth,windowHeight);
    gl::ClearColor(0,0,0,1);gl::Clear(GL_COLOR_BUFFER_BIT);
    int x=0,y=0,w=windowWidth,h=windowHeight;
    if(!stretch) {
        const float scale=std::min(windowWidth/layout.canvasWidth,windowHeight/layout.canvasHeight);
        w=int(std::lround(layout.canvasWidth*scale));h=int(std::lround(layout.canvasHeight*scale));
        x=(windowWidth-w)/2;y=(windowHeight-h)/2;
    }
    gl::Viewport(x,y,w,h);
    gl::UseProgram(composeProgram_);gl::BindVertexArray(emptyArray_);
    for(unsigned plane=0;plane<3;++plane) {
        gl::ActiveTexture(GL_TEXTURE0+plane);gl::BindTexture(GL_TEXTURE_2D,recipeTextures_[plane]);
        gl::Uniform1i(uniform(composeProgram_,("uRecipe"+std::to_string(plane)).c_str()),int(plane));
    }
    for(unsigned slot=0;slot<2;++slot) {
        const auto& rect=layout.threeRects[slot];
        const bool used=fogged_[slot].framebuffer&&rect[2]>0&&rect[3]>0;
        gl::ActiveTexture(GL_TEXTURE3+slot);gl::BindTexture(GL_TEXTURE_2D,fogged_[slot].framebuffer?threeColor(slot):0);
        const auto suffix=std::to_string(slot);
        gl::Uniform1i(uniform(composeProgram_,("uThree"+suffix).c_str()),int(3+slot));
        gl::Uniform4f(uniform(composeProgram_,("uThreeRect"+suffix).c_str()),rect[0],rect[1],used?rect[2]:0.f,used?rect[3]:0.f);
    }
    gl::ActiveTexture(GL_TEXTURE5);gl::BindTexture(GL_TEXTURE_2D,sideColumns_);
    gl::Uniform1i(uniform(composeProgram_,"uSideColumns"),5);
    gl::ActiveTexture(GL_TEXTURE6);gl::BindTexture(GL_TEXTURE_2D,recipeTextures_[3]);
    gl::Uniform1i(uniform(composeProgram_,"uRecipe3"),6);
    gl::ActiveTexture(GL_TEXTURE7);gl::BindTexture(GL_TEXTURE_2D,hdAtlas_);
    gl::Uniform1i(uniform(composeProgram_,"uHdAtlas"),7);
    gl::Uniform2f(uniform(composeProgram_,"uCanvas"),layout.canvasWidth,layout.canvasHeight);
    gl::Uniform1i(uniform(composeProgram_,"uSides"),layout.sides&&fogged_[0].framebuffer?1:0);
    gl::Uniform1i(uniform(composeProgram_,"uPlaced"),layout.placed?1:0);
    gl::Uniform1f(uniform(composeProgram_,"uInsetAlpha"),layout.insetAlpha);
    gl::Uniform4f(uniform(composeProgram_,"uScreen0"),layout.screens[0][0],layout.screens[0][1],layout.screens[0][2],layout.screens[0][3]);
    gl::Uniform4f(uniform(composeProgram_,"uScreen1"),layout.screens[1][0],layout.screens[1][1],layout.screens[1][2],layout.screens[1][3]);
    gl::Uniform4f(uniform(composeProgram_,"uInset"),layout.inset[0],layout.inset[1],fogged_[1].framebuffer?layout.inset[2]:0.f,layout.inset[3]);
    gl::Uniform1i(uniform(composeProgram_,"uBackdrop3D"),layout.backdrop3D&&fogged_[0].framebuffer?1:0);
    gl::Uniform1i(uniform(composeProgram_,"uThreeScreenRow"),layout.threeScreenBottom?288:96);
    gl::Uniform1f(uniform(composeProgram_,"uScale"),float(w)/layout.canvasWidth);
    gpuCheckpoint("gpu-upload");
    gl::DrawArrays(GL_TRIANGLE_STRIP,0,4);
    gl::ActiveTexture(GL_TEXTURE0);
    gpuCheckpoint("gpu-compose");
    gl::BindFramebuffer(GL_READ_FRAMEBUFFER,output_.framebuffer);gl::BindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    gl::BlitFramebuffer(0,0,windowWidth,windowHeight,0,0,windowWidth,windowHeight,GL_COLOR_BUFFER_BIT,GL_NEAREST);
    gpuCheckpoint("gpu-blit");
    endFrameTiming();
    gl::BindFramebuffer(GL_FRAMEBUFFER,0);
}
std::vector<std::uint32_t> GpuRenderer::readPresented(int& width,int& height) {
    width=output_.width;height=output_.height;
    std::vector<std::uint32_t> raw(std::size_t(width)*height),pixels(raw.size());
    gl::BindFramebuffer(GL_FRAMEBUFFER,output_.framebuffer);gl::PixelStorei(GL_PACK_ALIGNMENT,4);
    gl::ReadPixels(0,0,width,height,GL_BGRA,GL_UNSIGNED_BYTE,raw.data());
    gl::BindFramebuffer(GL_FRAMEBUFFER,0);
    for(int row=0;row<height;++row)std::copy_n(raw.data()+std::size_t(height-1-row)*width,width,pixels.data()+std::size_t(row)*width);
    return pixels;
}
}
