#include "main/effect/UnkScreenEffect_0202c678.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/effect/UnkEffectCamera.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/object/DSSAObject.hpp"
#include "nnsys/g3d.hpp"
#include "nitro/g3.hpp"
#include "nitro/fx/fx_trig.h"

THUMB void UnkScreenEffect_0202c678::start()
{
    columns_ = 1;
    rows_ = 1;
    alpha_ = 31;
    scale_ = dss::Fix32(0x8000);
    unk_20 = 0xffff;
    unk_28 = 0x8000;
    dss::memset(unk_30, 0, sizeof(unk_30));
    unk_24 = 0;
    unk_2c = 0x8000;
    unk_50 = 0;
    unk_5c = 0;
    UnkScreenEffect::start();
}

THUMB void UnkScreenEffect_0202c678::draw()
{
    unkfunc_0202c6e8();
}

THUMB bool UnkScreenEffect_0202c678::isEnd()
{
    return unk_50 > 60 || unk_30[0] > 60;
}

#pragma push
#pragma opt_unroll_instr_count 127
#pragma opt_unroll_loops on
THUMB void UnkScreenEffect_0202c678::unkfunc_0202c6e8()
{
    if (flag_.check(2) != false) {
        dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(4, 0xc);
        alpha_ = 31;
        UnkEffectCamera::getSingleton()->draw();
        {
            VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
            VecFx32 trans = { 0, 0, 0 };
            MtxFx33 rot;
            func_02061b88(&rot);
            func_0206ae30(&scale);
            func_02067940(&rot, &data_0210cf28.prmBaseRot);
            data_0210cf28.flag &= ~0xa4;
            func_0206ae08((dss::Fix32Vector3*)&trans);
            func_0206adcc();
        }
        func_0206dcf0();
        unk_2c = (unk_50 * 10) >> 1;
        unk_50++;
        unk_24 += 20;
        if (unk_24 >= 0x10000) {
            unk_24 -= 0x10000;
        }
        int polygonID = 0;
        for (int i = 0; i < 2; i++) {
            for (int k = 0; k < 4; k++) {
                int dx;
                int dy = unk_50 * ((i + 1) * 10);
                dx = dy * 3;
                int corner;
                if (i == 1) {
                    corner = k;
                } else {
                    corner = 3 - k;
                }
                G3_PushMtx();
                switch (corner) {
                    case 0:
                        G3_Translate(dx + tiles_[0].x, dy + tiles_[0].y, tiles_[0].z);
                        break;
                    case 1:
                        G3_Translate(dx + tiles_[0].x, tiles_[0].y - dy, tiles_[0].z);
                        break;
                    case 2:
                        G3_Translate(tiles_[0].x - dx, tiles_[0].y - dy, tiles_[0].z);
                        break;
                    case 3:
                        G3_Translate(tiles_[0].x - dx, dy + tiles_[0].y, tiles_[0].z);
                        break;
                }
                func_02065c9c(FX_SinIdx(unk_24), FX_CosIdx(unk_24));
                G3_Scale(0x8000, 0x8000, 0x8000);
                int alpha = 15 - k * 4 - (unk_50 >> 3);
                if (alpha < 0) {
                    alpha = 0;
                }
                alpha_ = alpha;
                unkfunc_0202b510(polygonID);
                polygonID++;
                G3_Begin(1);
                int v = 3;
                do {
                    G3_Color(0x7fff);
                    G3_TexCoord(tiles_[0].texCoord[3 - v][0], tiles_[0].texCoord[3 - v][1]);
                    G3_Vtx(tiles_[0].vertex[3 - v].vx, tiles_[0].vertex[3 - v].vy, tiles_[0].vertex[3 - v].vz);
                    v--;
                } while (v >= 0);
                G3_PopMtx(1);
            }
        }
    }
}

#pragma pop

THUMB void UnkScreenEffect_0202c9b8::start()
{
    columns_ = 32;
    rows_ = 24;
    alpha_ = 31;
    scale_ = dss::Fix32(0x8000);
    for (int i = 0; i < 10; i++) {
        unk_54[i] = 0x180 + i * 2;
    }
    unk_24 = 0;
    unk_20 = 0;
    unk_28 = -250;
    UnkScreenEffect::start();
}

THUMB void UnkScreenEffect_0202c9b8::draw()
{
    if (flag_.check(2) != false) {
        dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(2, 0xe);
        UnkEffectCamera::getSingleton()->draw();
        {
            VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
            VecFx32 trans = { 0, 0, 0 };
            MtxFx33 rot;
            func_02061b88(&rot);
            func_0206ae30(&scale);
            func_02067940(&rot, &data_0210cf28.prmBaseRot);
            data_0210cf28.flag &= ~0xa4;
            func_0206ae08((dss::Fix32Vector3*)&trans);
            func_0206adcc();
        }
        func_0206dcf0();
        for (int i = 1; i < 23; i++) {
            for (int j = 1; j < 31; j++) {
                int index = j + i * 32;
                for (int k = 0; k < 4; k++) {
                    short z = tiles_[index].vertex[k].vz;
                    short average = (tiles_[index - 1].vertex[k].vz + tiles_[index + 1].vertex[k].vz +
                                     tiles_[index - 24].vertex[k].vz + tiles_[index + 24].vertex[k].vz) >> 2;
                    z += (short)(average - z) >> 2;
                    unkfunc_0202cd34(index, k, z);
                }
            }
        }
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 24; j++) {
                int index = j + i * 24;
                G3_PushMtx();
                G3_Scale(0x8000, 0x8000, 0x8000);
                tiles_[index].z += 80;
                if (unkfunc_02081254() & 1) {
                    G3_Translate(tiles_[index].x, tiles_[index].y + unk_24, tiles_[index].z);
                } else {
                    G3_Translate(tiles_[index].x, tiles_[index].y - unk_24, tiles_[index].z);
                }
                unkfunc_0202b510(0x18);
                G3_Begin(1);
                for (int k = 0; k < 4; k++) {
                    tiles_[index].vertex[k].vz -= 40;
                    int color = dss::clamp<int>(tiles_[index].vertex[k].vz / 100 + 31, 0, 31);
                    if (unk_68 != 0) {
                        G3_Color((color << 10) | 0x3ff);
                    } else {
                        G3_Color(color | (color << 5) | (31 << 10));
                    }
                    G3_TexCoord(tiles_[index].texCoord[k][0], tiles_[index].texCoord[k][1]);
                    G3_Vtx(tiles_[index].vertex[k].vx, tiles_[index].vertex[k].vy, tiles_[index].vertex[k].vz);
                }
                G3_PopMtx(1);
            }
        }
        unk_20++;
        unk_24++;
        for (int i = 0; i < 10; i++) {
            unkfunc_0202cd34(unk_54[i], 0, unk_2c);
            unkfunc_0202cd34(unk_54[i], 1, unk_2c);
            unkfunc_0202cd34(unk_54[i], 2, unk_2c);
            unkfunc_0202cd34(unk_54[i], 3, unk_2c);
        }
        unk_2c += unk_28;
        if (unk_2c < -10000 || (unk_2c >= -2000 && unk_28 > 0)) {
            unk_28 = -unk_28;
        }
    }
}

THUMB bool UnkScreenEffect_0202c9b8::isEnd()
{
    return unk_24 > 60;
}

THUMB void UnkScreenEffect_0202c9b8::unkfunc_0202cd34(int index, int vertex, short z)
{
    int up = index - 24;
    int down = index + 24;
    tiles_[index].vertex[vertex].vz = z;
    switch (vertex) {
        case 0:
            tiles_[up].vertex[1].vz = z;
            tiles_[up + 1].vertex[2].vz = z;
            tiles_[index + 1].vertex[3].vz = z;
            break;
        case 1:
            tiles_[down].vertex[0].vz = z;
            tiles_[index + 1].vertex[2].vz = z;
            tiles_[down + 1].vertex[3].vz = z;
            break;
        case 2:
            tiles_[down - 1].vertex[0].vz = z;
            tiles_[index - 1].vertex[1].vz = z;
            tiles_[down].vertex[3].vz = z;
            break;
        case 3:
            tiles_[index - 1].vertex[0].vz = z;
            tiles_[up - 1].vertex[1].vz = z;
            tiles_[up].vertex[2].vz = z;
            break;
    }
}

THUMB void UnkScreenEffect_0202ce0c::start()
{
    columns_ = 1;
    rows_ = 1;
    alpha_ = 31;
    scale_ = dss::Fix32(0x8000);
    dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(4, 0xc);
    unk_20 = 0;
    unk_28 = 0x10000;
    unk_24 = 0;
    unk_2c = 0x8000;
    unk_30 = 0;
    UnkScreenEffect::start();
}

THUMB void UnkScreenEffect_0202ce0c::draw()
{
    if (flag_.check(2) != false) {
        dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(4, 0xc);
        UnkEffectCamera::getSingleton()->draw();
        {
            VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
            VecFx32 trans = { 0, 0, 0 };
            MtxFx33 rot;
            func_02061b88(&rot);
            func_0206ae30(&scale);
            func_02067940(&rot, &data_0210cf28.prmBaseRot);
            data_0210cf28.flag &= ~0xa4;
            func_0206ae08((dss::Fix32Vector3*)&trans);
            func_0206adcc();
        }
        func_0206dcf0();
        if (unkfunc_02081254() & 1) {
            G3_PushMtx();
            G3_Translate(tiles_[0].x, unk_24 + tiles_[0].y, tiles_[0].z - unk_20);
            const s16* sinCos = &FX_SinCosTable_[(unk_28 >> 4) * 2];
            func_02065c24(sinCos[0], sinCos[1]);
            G3_Scale(unk_2c, unk_2c, unk_2c);
            unkfunc_0202b510(0x18);
            G3_Begin(1);
            for (int k = 0; k < 4; k++) {
                G3_Color(0x7fff);
                G3_TexCoord(tiles_[0].texCoord[k][0], tiles_[0].texCoord[k][1]);
                G3_Vtx(tiles_[0].vertex[k].vx, tiles_[0].vertex[k].vy, tiles_[0].vertex[k].vz);
            }
            G3_PopMtx(1);
            unk_20 += 0x800;
            unk_28 -= 400;
            unk_24 -= unk_30 * 16 + 250;
            if (unk_2c > 0) {
                unk_2c -= 0x300;
            } else {
                unk_2c = 0;
            }
            unk_30++;
            if (unk_20 > 0x14000) {
                unk_20 = 0;
                unk_28 = 0x10000;
                unk_24 = 0;
                unk_2c = 0x8000;
                unk_30 = 0;
            }
        }
    }
}

THUMB bool UnkScreenEffect_0202ce0c::isEnd()
{
    return unk_20 > 0x14000;
}
