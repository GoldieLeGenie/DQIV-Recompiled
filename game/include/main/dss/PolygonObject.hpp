#pragma once
#include <globaldefs.h>
#include "main/dss/Billboard.hpp"

struct PolygonVertex {
    dss::Vector3<dss::Fix16> v[4];

    PolygonVertex() {}
    PolygonVertex(const dss::Vector3<dss::Fix16>& v0, const dss::Vector3<dss::Fix16>& v1,
                  const dss::Vector3<dss::Fix16>& v2, const dss::Vector3<dss::Fix16>& v3)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }
};

/* vtable 0x020c4440 */
struct PolygonObject : RenderObject3D {
    virtual void draw();                        // 0x0208383c

    PolygonVertex vertex_;                      // 0x48
    BillboardTexCoord texCoord_;                // 0x60
    dss::Vector3<char> unk_80;                  // 0x80 color (r, g, b)

    void unkfunc_02083734(const PolygonVertex* vertex);         // setVertex
    void unkfunc_020837d0(const BillboardTexCoord* texCoord);   // setTexCoord
    void unkfunc_02083848();                                    // draw the textured quad
};

// shadow volume: quad (draw) or 8 quads (unkfunc_02083b1c) drawn as shadow polygons
/* vtable 0x020c447c */
struct UnkShadowPolygon : PolygonObject {
    virtual void draw();                        // 0x020839bc
    virtual void unkfunc_02083b1c();            // draw unk_84

    PolygonVertex unk_84[8];                    // 0x84

    void unkfunc_02083d00(int index, const PolygonVertex* vertex);
};
