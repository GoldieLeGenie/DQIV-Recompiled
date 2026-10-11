#include "GxGeometry.hpp"
#include "DualScreen.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <string>

namespace pc {
namespace {
std::int32_t signedBits(std::uint32_t value,unsigned bits) {
    const auto sign=1u<<(bits-1);
    return std::bit_cast<std::int32_t>((value&((1u<<bits)-1))^sign)-static_cast<std::int32_t>(sign);
}
std::array<float,4> transform(const std::array<std::int32_t,4>& v,const GxMatrix& m) {
    std::array<float,4> result{};
    for(unsigned col=0;col<4;++col) {
        std::uint64_t sum=0;
        for(unsigned row=0;row<4;++row) sum+=static_cast<std::uint64_t>(std::int64_t(v[row])*m[row*4+col]);
        result[col]=static_cast<float>(std::bit_cast<std::int64_t>(sum)/4096.0/4096.0);
    }
    return result;
}
}
GxMatrix gxIdentity() {return {4096,0,0,0,0,4096,0,0,0,0,4096,0,0,0,0,4096};}
GxMatrix gxMultiply(const GxMatrix& a,const GxMatrix& b) {
    GxMatrix result{};
    for(unsigned row=0;row<4;++row) for(unsigned col=0;col<4;++col) {
        std::uint64_t sum=0;
        for(unsigned k=0;k<4;++k) sum+=static_cast<std::uint64_t>(std::int64_t(a[row*4+k])*b[k*4+col]);
        result[row*4+col]=std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(std::bit_cast<std::int64_t>(sum)>>12));
    }
    return result;
}
GxGeometry::GxGeometry():projection_(gxIdentity()),position_(gxIdentity()),vector_(gxIdentity()),texture_(gxIdentity()) {
    positionStack_.fill(gxIdentity());vectorStack_.fill(gxIdentity());
    projectionStack_.fill(gxIdentity());textureStack_.fill(gxIdentity());
}
unsigned GxGeometry::parameterCount(unsigned op) {
    switch(op) {
    case 0:case 0x11:case 0x15:case 0x41:return 0;
    case 0x10:case 0x12:case 0x13:case 0x14:case 0x20:case 0x21:case 0x22:
    case 0x24:case 0x25:case 0x26:case 0x27:case 0x28:case 0x29:case 0x2a:case 0x2b:
    case 0x30:case 0x31:case 0x32:case 0x33:case 0x40:case 0x50:case 0x60:case 0x72:return 1;
    case 0x23:case 0x71:return 2;
    case 0x1b:case 0x1c:case 0x70:return 3;
    case 0x1a:return 9;
    case 0x17:case 0x19:return 12;
    case 0x16:case 0x18:return 16;
    case 0x34:return 32;
    default:throw std::runtime_error("Unknown geometry command "+std::to_string(op));
    }
}
bool GxGeometry::commandComplete() const {return packedIndex_==4 && argumentCount_==0;}
void GxGeometry::drainEmptyCommands() {
    while(packedIndex_<4 && parameterCount(packed_[packedIndex_])==0) execute(packed_[packedIndex_++],{});
}
void GxGeometry::fifo(std::uint32_t word) {
    if(directCommand_) throw std::runtime_error("FIFO interrupts partial direct command");
    if(packedIndex_==4) {
        for(unsigned i=0;i<4;++i) packed_[i]=static_cast<std::uint8_t>(word>>(i*8));
        packedIndex_=0; drainEmptyCommands(); return;
    }
    const auto op=packed_[packedIndex_];
    arguments_[argumentCount_++]=word;
    if(argumentCount_==parameterCount(op)) {
        execute(op,{arguments_.data(),argumentCount_});argumentCount_=0;++packedIndex_;drainEmptyCommands();
    }
}
void GxGeometry::direct(unsigned op,std::uint32_t word) {
    if(packedIndex_!=4 || (directCommand_ && directCommand_!=op)) throw std::runtime_error("Interleaved geometry commands");
    const auto count=parameterCount(op);
    if(!count) {execute(op,{});return;}
    directCommand_=op;arguments_[argumentCount_++]=word;
    if(argumentCount_==count) {execute(op,{arguments_.data(),count});argumentCount_=0;directCommand_=0;}
}
void GxGeometry::command(unsigned op,std::span<const std::uint32_t> parameters) {
    if(!commandComplete() || parameters.size()!=parameterCount(op)) throw std::runtime_error("Invalid complete geometry command");
    execute(op,parameters);
}
void GxGeometry::displayList(std::span<const std::uint8_t> bytes) {
    if(bytes.size()%4) throw std::runtime_error("Unaligned geometry display list size");
    for(std::size_t i=0;i<bytes.size();i+=4)
        fifo(bytes[i]|(std::uint32_t(bytes[i+1])<<8)|(std::uint32_t(bytes[i+2])<<16)|(std::uint32_t(bytes[i+3])<<24));
    if(!commandComplete()) throw std::runtime_error("Truncated geometry display list");
}
GxMatrix GxGeometry::clipMatrix() const {return gxMultiply(position_,projection_);}
void GxGeometry::clearTriangles() {triangles_.clear();strip_.clear();inside_=false;}
void GxGeometry::boxTest(std::span<const std::uint32_t> p) {
    const std::int32_t origin[3]={signedBits(p[0],16),signedBits(p[0]>>16,16),signedBits(p[1],16)};
    const std::int32_t size[3]={signedBits(p[1]>>16,16),signedBits(p[2],16),signedBits(p[2]>>16,16)};
    std::array<std::array<float,4>,8> corners{};
    const auto m=clipMatrix();
    for(unsigned i=0;i<8;++i)corners[i]=transform({origin[0]+((i&1)?size[0]:0),origin[1]+((i&2)?size[1]:0),origin[2]+((i&4)?size[2]:0),4096},m);
    constexpr unsigned faces[6][4]={{0,1,3,2},{4,6,7,5},{0,4,5,1},{2,3,7,6},{0,2,6,4},{1,5,7,3}};
    boxTestResult_=false;
    // Clip the six faces in homogeneous coordinates, as for submitted geometry.
    for(const auto& face:faces) {
        std::vector<std::array<float,4>> polygon;
        for(auto i:face)polygon.push_back(corners[i]);
        for(unsigned plane=0;plane<6 && !polygon.empty();++plane) {
            auto distance=[&](const auto& v){return v[3]+((plane&1)?-v[plane/2]:v[plane/2]);};
            std::vector<std::array<float,4>> clipped;
            auto previous=polygon.back();auto pd=distance(previous);
            for(const auto& v:polygon) {
                const auto d=distance(v);
                if((d>=0)!=(pd>=0)) {
                    const float t=pd/(pd-d);std::array<float,4> intersection{};
                    for(unsigned k=0;k<4;++k)intersection[k]=previous[k]+(v[k]-previous[k])*t;
                    clipped.push_back(intersection);
                }
                if(d>=0)clipped.push_back(v);
                previous=v;pd=d;
            }
            polygon=std::move(clipped);
        }
        if(!polygon.empty()) {boxTestResult_=true;break;}
    }
}
void GxGeometry::loadMatrix(const GxMatrix& matrix,bool multiply,bool affectVector) {
    auto& current=mode_==0 ? projection_ : mode_==3 ? texture_ : position_;
    current=multiply ? gxMultiply(matrix,current) : matrix;
    if(mode_==2 && affectVector) vector_=multiply ? gxMultiply(matrix,vector_) : matrix;
}
void GxGeometry::vertex() {
    if(!inside_) throw std::runtime_error("Geometry vertex outside primitive");
    auto clip=transform({vertex_[0],vertex_[1],vertex_[2],4096},clipMatrix());
    const bool perspective=projection_[3]||projection_[7]||projection_[11];
    if(perspective) clip[0]*=stretchX();
    const auto generation=material_.texture>>30;
    if(generation==3) textureCoordinate(vertex_,20);
    strip_.push_back({clip,normal_,transformedU_,transformedV_,color_});
    const auto n=strip_.size();
    auto emit=[&](std::size_t a,std::size_t b,std::size_t c,std::uint8_t edges=7) {
        triangles_.push_back({{strip_[a],strip_[b],strip_[c]},material_,viewport_,edges,perspective,static_cast<std::uint8_t>(drawLayer())});
    };
    if(primitive_==0 && n%3==0) emit(n-3,n-2,n-1);
    if(primitive_==1 && n%4==0) {emit(n-4,n-3,n-2,3);emit(n-4,n-2,n-1,6);}
    if(primitive_==2 && n>=3) {if(n&1)emit(n-3,n-2,n-1);else emit(n-2,n-3,n-1);}
    if(primitive_==3 && n>=4 && n%2==0) {emit(n-4,n-3,n-1,3);emit(n-4,n-1,n-2,6);}
}
void GxGeometry::textureCoordinate(const std::array<std::int32_t,3>& source,unsigned shift) {
    float* output[2]={&transformedU_,&transformedV_};
    for(unsigned col=0;col<2;++col) {
        std::int64_t sum=0;
        for(unsigned row=0;row<3;++row)sum+=std::int64_t(source[row])*texture_[row*4+col];
        if(shift==12) sum+=texture_[12+col];
        auto value=sum>>shift;
        if(shift!=12)value+=static_cast<int>((col?v_:u_)*16);
        *output[col]=signedBits(static_cast<std::uint32_t>(value),16)/16.f;
    }
}
void GxGeometry::lightNormal() {
    float channels[3];
    for(unsigned c=0;c<3;++c)channels[c]=float((material_.specularEmission>>(16+5*c))&31);
    for(unsigned light=0;light<4;++light)if(material_.polygon&(1u<<light)) {
        const auto& direction=material_.lightDirections[light];
        float diffuse=0,specular=0;
        for(unsigned axis=0;axis<3;++axis) {
            diffuse-=direction[axis]*normal_[axis];
            specular-=(direction[axis]-(axis==2?1.f:0.f))*.5f*normal_[axis];
        }
        diffuse=std::clamp(diffuse,0.f,1.f);specular=std::clamp(specular,0.f,1.f);specular*=specular;
        if(material_.specularEmission&0x8000)specular=material_.shininess[std::min(127u,unsigned(specular*128))]/256.f;
        for(unsigned c=0;c<3;++c) {
            const auto lightColor=(material_.lightColors[light]>>(5*c))&31;
            channels[c]+=lightColor/32.f*(float((material_.diffuseAmbient>>(5*c))&31)*diffuse+
                float((material_.diffuseAmbient>>(16+5*c))&31)+float((material_.specularEmission>>(5*c))&31)*specular);
        }
    }
    color_=0;for(unsigned c=0;c<3;++c)color_|=std::min(31u,unsigned(std::max(0.f,channels[c])))<<(5*c);
}
void GxGeometry::execute(unsigned op,std::span<const std::uint32_t> p) {
    const auto word=p.empty()?0:p[0];
    auto matrix=[&](unsigned columns) {
        auto value=gxIdentity();
        const unsigned rows=columns==4 ? 4 : p.size()==9 ? 3 : 4;
        for(unsigned r=0;r<rows;++r) for(unsigned c=0;c<columns;++c)
            value[r*4+c]=std::bit_cast<std::int32_t>(p[r*columns+c]);
        return value;
    };
    auto store=[&](unsigned index) {
        if(mode_==0)projectionStack_[index&1]=projection_;
        else if(mode_==3)textureStack_[index&1]=texture_;
        else {positionStack_[index&31]=position_;vectorStack_[index&31]=vector_;}
    };
    auto restore=[&](unsigned index) {
        if(mode_==0)projection_=projectionStack_[index&1];
        else if(mode_==3)texture_=textureStack_[index&1];
        else {position_=positionStack_[index&31];vector_=vectorStack_[index&31];}
    };
    switch(op) {
    case 0:break;
    case 0x10:mode_=word&3;break;
    case 0x11: {
        auto& level=mode_==0?projectionLevel_:mode_==3?textureLevel_:positionLevel_;
        store(level++);level&=(mode_==0 || mode_==3 ? 1 : 31);break;
    }
    case 0x12: {
        auto& level=mode_==0?projectionLevel_:mode_==3?textureLevel_:positionLevel_;
        level=(level-signedBits(word,6))&(mode_==0 || mode_==3 ? 1 : 31);restore(level);break;
    }
    case 0x13:store(word);break;
    case 0x14:restore(word);break;
    case 0x15:loadMatrix(gxIdentity(),false);break;
    case 0x16:case 0x17:case 0x18:case 0x19:case 0x1a:
        loadMatrix(matrix(op==0x16 || op==0x18?4:3),op>=0x18);break;
    case 0x1b:case 0x1c: {
        auto m=gxIdentity();
        for(unsigned i=0;i<3;++i)m[op==0x1b ? i*5 : 12+i]=std::bit_cast<std::int32_t>(p[i]);
        loadMatrix(m,true,false);break;
    }
    case 0x20:color_=word&0x7fff;break;
    case 0x21: {
        const std::array<std::int32_t,3> raw{signedBits(word,10),signedBits(word>>10,10),signedBits(word>>20,10)};
        if((material_.texture>>30)==2)textureCoordinate(raw,17);
        auto transformed=transform({raw[0]*8,raw[1]*8,raw[2]*8,0},vector_);
        normal_={transformed[0],transformed[1],transformed[2]};lightNormal();break;
    }
    case 0x22:
        u_=signedBits(word,16)/16.f;v_=signedBits(word>>16,16)/16.f;
        if((material_.texture>>30)==1)textureCoordinate({signedBits(word,16),signedBits(word>>16,16),1},12);
        else if((material_.texture>>30)==0) {transformedU_=u_;transformedV_=v_;}
        break;
    case 0x23:vertex_={signedBits(word,16),signedBits(word>>16,16),signedBits(p[1],16)};vertex();break;
    case 0x24:vertex_={signedBits(word,10)*64,signedBits(word>>10,10)*64,signedBits(word>>20,10)*64};vertex();break;
    case 0x25:vertex_[0]=signedBits(word,16);vertex_[1]=signedBits(word>>16,16);vertex();break;
    case 0x26:vertex_[0]=signedBits(word,16);vertex_[2]=signedBits(word>>16,16);vertex();break;
    case 0x27:vertex_[1]=signedBits(word,16);vertex_[2]=signedBits(word>>16,16);vertex();break;
    case 0x28:for(unsigned i=0;i<3;++i)vertex_[i]=signedBits(static_cast<std::uint32_t>(vertex_[i]+signedBits(word>>(i*10),10)),16);vertex();break;
    case 0x29:pendingPolygon_=word;break;
    case 0x2a:material_.texture=word;break;
    case 0x2b:material_.palette=word;break;
    case 0x30:material_.diffuseAmbient=word;if(word&0x8000)color_=word&0x7fff;break;
    case 0x31:material_.specularEmission=word;break;
    case 0x32: {
        auto v=transform({signedBits(word,10)*8,signedBits(word>>10,10)*8,signedBits(word>>20,10)*8,0},vector_);
        material_.lightDirections[word>>30]={v[0],v[1],v[2]};break;
    }
    case 0x33:material_.lightColors[word>>30]=word&0x7fff;break;
    case 0x34:for(unsigned i=0;i<128;++i)material_.shininess[i]=static_cast<std::uint8_t>(p[i/4]>>(8*(i%4)));break;
    case 0x40:primitive_=word&3;material_.polygon=pendingPolygon_;strip_.clear();inside_=true;break;
    case 0x41:inside_=false;strip_.clear();break;
    case 0x50:++swaps_;swapParameters_=word&3;break;
    case 0x60:viewport_=word;break;
    case 0x70:boxTest(p);break;
    case 0x71:positionTestResult_=transform({signedBits(word,16),signedBits(word>>16,16),signedBits(p[1],16),4096},clipMatrix());break;
    case 0x72: {
        const auto n=transform({signedBits(word,10)*8,signedBits(word>>10,10)*8,signedBits(word>>20,10)*8,0},vector_);
        vectorTestResult_={n[0],n[1],n[2]};break;
    }
    default:throw std::runtime_error("Geometry query not implemented: "+std::to_string(op));
    }
}
}
