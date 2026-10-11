#pragma once
#include <array>
#include <cstdint>
#include <span>
#include <vector>

namespace pc {
using GxMatrix=std::array<std::int32_t,16>;
struct GxVertex {
    std::array<float,4> clip;
    std::array<float,3> normal;
    float u=0,v=0;
    std::uint32_t color=0x7fff;
};
struct GxMaterial {
    std::uint32_t polygon=0x1f00c0,texture=0,palette=0,diffuseAmbient=0,specularEmission=0;
    std::array<std::array<float,3>,4> lightDirections{};
    std::array<std::uint32_t,4> lightColors{};
    std::array<std::uint8_t,128> shininess{};
};
struct GxTriangle {
    std::array<GxVertex,3> vertices;
    GxMaterial material;
    std::uint32_t viewport=0xbfff0000;
    // Real polygon edges; triangulating a quad must not add a wireframe diagonal.
    std::uint8_t edges=7;
    // Drawn through a perspective projection (scene geometry), not an orthographic overlay.
    bool perspective=false;
    // Presentation layer the game drew it in (DualScreen.hpp drawLayer).
    std::uint8_t layer=0;
};
struct GxRasterState {
    std::uint32_t clearColor=0,fogColor=0;
    std::uint16_t clearDepth=0x7fff,fogOffset=0;
    bool fogEnabled=false,fogAlphaOnly=false;
    unsigned fogSlope=0;
    std::array<std::uint8_t,32> fogTable{};
    std::array<std::uint16_t,8> edgeTable{};
    std::array<std::uint16_t,32> toonTable{};
};
// Stateful native geometry front end used by both SDK API calls and display lists.
// It retains clip coordinates and lighting/material inputs for the PC rasterizer.
class GxGeometry {
public:
    GxGeometry();
    void fifo(std::uint32_t word);
    void direct(unsigned command,std::uint32_t word=0);
    void command(unsigned command,std::span<const std::uint32_t> parameters={});
    void displayList(std::span<const std::uint8_t> bytes);
    bool commandComplete() const;
    void clearTriangles();
    const std::vector<GxTriangle>& triangles() const {return triangles_;}
    GxMatrix clipMatrix() const;
    const GxMatrix& positionMatrix() const {return position_;}
    const GxMatrix& vectorMatrix() const {return vector_;}
    unsigned positionStackLevel() const {return positionLevel_;}
    unsigned projectionStackLevel() const {return projectionLevel_;}
    unsigned swaps() const {return swaps_;}
    unsigned swapParameters() const {return swapParameters_;}
    bool boxTestResult() const {return boxTestResult_;}
    std::array<float,4> positionTestResult() const {return positionTestResult_;}
    std::array<float,3> vectorTestResult() const {return vectorTestResult_;}
    static unsigned parameterCount(unsigned command);
    GxRasterState raster;
private:
    void execute(unsigned,std::span<const std::uint32_t>);
    void drainEmptyCommands();
    void loadMatrix(const GxMatrix&,bool multiply,bool affectVector=true);
    void vertex();
    void textureCoordinate(const std::array<std::int32_t,3>& source,unsigned shift);
    void lightNormal();
    void boxTest(std::span<const std::uint32_t>);
    GxMatrix projection_,position_,vector_,texture_;
    std::array<GxMatrix,32> positionStack_,vectorStack_;
    std::array<GxMatrix,2> projectionStack_,textureStack_;
    unsigned mode_=0,positionLevel_=0,projectionLevel_=0,textureLevel_=0;
    std::array<std::uint8_t,4> packed_{};
    unsigned packedIndex_=4,argumentCount_=0,directCommand_=0;
    std::array<std::uint32_t,32> arguments_{};
    std::array<std::int32_t,3> vertex_{};
    std::array<float,3> normal_{};
    float u_=0,v_=0;
    float transformedU_=0,transformedV_=0;
    std::uint32_t color_=0x7fff,viewport_=0xbfff0000;
    unsigned primitive_=0,swaps_=0;
    unsigned swapParameters_=0,pendingPolygon_=0x1f00c0;
    bool inside_=false;
    bool boxTestResult_=false;
    std::array<float,4> positionTestResult_{};
    std::array<float,3> vectorTestResult_{};
    GxMaterial material_;
    std::vector<GxVertex> strip_;
    std::vector<GxTriangle> triangles_;
};
GxMatrix gxIdentity();
GxMatrix gxMultiply(const GxMatrix&,const GxMatrix&);
}
