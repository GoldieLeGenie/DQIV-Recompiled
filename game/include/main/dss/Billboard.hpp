#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/dss/Position.hpp"
#include "main/dss/Render.hpp"
#include "main/dss/TextureObject.hpp"

/* vtable 0x020c43c4 */
struct RenderObject {
    virtual void draw() {}                      // slot 0, 0x0204a044 (weak copy kept from the BillboardCharacter TU)
    virtual void setPolygonID(int id);          // slot 1
    virtual void setAlpha(int alpha);           // slot 2
    virtual void setRender(Render* render);     // slot 3
    virtual void removeRender();                // slot 4
    virtual Render* getRender();                // slot 5

    Render* render_;                            // 0x04
    void* texture_;                             // 0x08
    unsigned char polygonID_;                   // 0x0C
    unsigned char alpha_;                       // 0x0D
    int enable_;                                // 0x10

    RenderObject();                             // C2 0x02083658
    void unkfunc_02083680();                    // polygon attributes
    void unkfunc_020836a8();                    // shadow mask polygon attributes
    void unkfunc_020836c0();                    // shadow polygon attributes
};

/* vtable 0x020c1c88 */
struct RenderObject3D : RenderObject, Position {
#if !defined(DQIV_NATIVE) && !defined(BILLBOARD_CHARACTER_TU) && !defined(RENDER_OBJECT_TU)
    virtual void draw();                        // never defined: key function so the vtable is only emitted by BillboardCharacter.cpp
#endif
};

struct BillboardVertex {
    dss::Vector2<dss::Fix16> v[4];

    BillboardVertex() {}
    BillboardVertex(const dss::Vector2<dss::Fix16>& v0, const dss::Vector2<dss::Fix16>& v1,
                    const dss::Vector2<dss::Fix16>& v2, const dss::Vector2<dss::Fix16>& v3)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }
};

struct BillboardTexCoord {
    dss::Vector2<dss::Fix32> v[4];

    BillboardTexCoord() {}
    BillboardTexCoord(const dss::Vector2<dss::Fix32>& v0, const dss::Vector2<dss::Fix32>& v1,
                      const dss::Vector2<dss::Fix32>& v2, const dss::Vector2<dss::Fix32>& v3)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }
};

/* vtable 0x020c4404 */
struct Billboard : RenderObject3D {
    virtual void draw();                        // 0x020842c0

    BillboardVertex vertex_;                    // 0x48
    BillboardTexCoord texCoord_;                // 0x58
    BillboardVertex drawVertex_;                // 0x78
    BillboardTexCoord drawTexCoord_;            // 0x88
    dss::Vector2<dss::Fix32> unk_a8;            // 0xA8 texture coordinate offset
    unsigned short color_;                      // 0xB0

    Billboard();                                // C2 0x02083da8
    ~Billboard() {}
    void unkfunc_020840c8(const BillboardVertex* vertex);       // setVertex
    BillboardVertex* unkfunc_02084134();                        // getVertex
    void unkfunc_0208413c(const BillboardTexCoord* texCoord);   // setTexCoord
    BillboardTexCoord* unkfunc_020841a8();                      // getTexCoord
    void unkfunc_020841b0(const dss::Vector2<dss::Fix32>* offset);  // setTexCoordOffset
    void unkfunc_020841d4();                                    // calc the draw vertex/texcoord
    void unkfunc_020842b8(int color);                           // setColor
};


#ifndef BILLBOARD_CHARACTER_TU
#include "main/dss/UnkTextureBillboard.hpp"
#endif
