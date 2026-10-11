#pragma once
#include "main/dss/Billboard.hpp"
/* vtable 0x020c1c44 */
struct UnkTextureBillboard : Billboard {
#if !defined(DQIV_NATIVE) && !defined(BILLBOARD_CHARACTER_TU)
    virtual void draw();                        // never defined: key function so the vtable is only emitted by BillboardCharacter.cpp
#endif

    void* texture_;                             // 0xB4

    void unkfunc_02058680(const BillboardVertex* vertex, const BillboardTexCoord* texCoord, void* texture);
    void unkfunc_020586c4();
    void unkfunc_020586d4(int a);
};
