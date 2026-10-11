#include "main/effect/UnkScreenEffect_0202ff7c.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/effect/UnkEffectCamera.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/HaveEquipment.hpp"
#include "main/dss/Random.hpp"
#include "nnsys/g3d.hpp"
#include "nitro/g3.hpp"
#include "nitro/fx/fx_trig.h"

THUMB void UnkScreenEffect_0202ff7c::start()
{
    columns_ = 32;
    rows_ = 24;
    alpha_ = 31;
    scale_ = dss::Fix32(0x8000);
    unk_24 = 0;
    unk_28 = 0xffff;
    unk_2c = 100;
    unk_20 = 0;
    UnkScreenEffect::start();
}

THUMB void UnkScreenEffect_0202ff7c::draw()
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
        int frame = unk_24;
        if (unk_20 != 0) {
            frame = 50 - frame;
        }
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 24; j++) {
                G3_PushMtx();
                G3_Scale(0x8000, 0x8000, 0x8000);
                func_02065c9c(FX_SinIdx(unk_28), FX_CosIdx(unk_28));
                int index = j + i * 24;
                G3_Translate(tiles_[index].x, tiles_[index].y, tiles_[index].z);
                unkfunc_0202b510(0x18);
                G3_Begin(1);
                for (int k = 0; k < 4; k++) {
                    VecFx16 dir;
                    VecFx16 pos;
                    pos.x = tiles_[index].x + tiles_[index].vertex[k].vx;
                    pos.y = tiles_[index].y + tiles_[index].vertex[k].vy;
                    pos.z = tiles_[index].z + tiles_[index].vertex[k].vz;
                    func_02063204(&pos, &dir);
                    int dx = (dir.x >> 8) * frame;
                    short x = tiles_[index].vertex[k].vx - dx;
                    short y = tiles_[index].vertex[k].vy - (dir.y >> 8) * frame;
                    short cx = status::BaseActionStatus::abs(pos.x) - status::HaveEquipment::getAbsoluteValue(dx);
                    short cy = status::BaseActionStatus::abs(pos.y) - status::HaveEquipment::getAbsoluteValue((dir.y >> 8) * frame);
                    if (cx < 0) {
                        x = -pos.x;
                    }
                    if (cy < 0) {
                        y = -pos.y;
                    }
                    G3_Color(0x7fff);
                    G3_TexCoord(tiles_[index].texCoord[k][0], tiles_[index].texCoord[k][1]);
                    G3_Vtx(x, y, tiles_[index].vertex[k].vz);
                }
                G3_PopMtx(1);
            }
        }
        unk_24++;
        unk_28 += unk_2c;
        if (unk_28 > 0xffff) {
            unk_28 -= 0xffff;
        }
        if (unk_28 < 0) {
            unk_28 += 0xffff;
        }
    }
}

THUMB bool UnkScreenEffect_0202ff7c::isEnd()
{
    return unk_24 > 50;
}

THUMB void UnkScreenEffect_0202ff7c::unkfunc_02030278(int flag)
{
    unk_20 = flag;
    if (flag) {
        unk_2c = 100;
        unk_28 = 0xffff - unk_2c * 49;
    } else {
        unk_2c = -100;
        unk_28 = 0xffff;
    }
}

THUMB void UnkScreenEffect_020302a0::start()
{
    UnkScreenEffect::start();
    columns_ = 32;
    rows_ = 24;
    alpha_ = 31;
    scale_ = dss::Fix32(0x8000);
    unk_20 = 0;
    unk_24 = 0;
    unk_30 = 0xffff;
    unk_34 = 0xffff;
    unk_2c = 0xffff;
}

THUMB void UnkScreenEffect_020302a0::unkfunc_020302e0(short column, short row, short index, short* sinValue, short* cosValue)
{
    const int unused[1] = {2};  // unreferenced, only its .rodata copy remains
    int which = dssrand::rand(2);
    int count = which ? unk_20 : unk_24;
    int angle = which ? unk_30 : unk_34;
    int column2 = column * 2;
    if (count > column2) {
        tiles_[index].x -= 2;
        int diff = count - column2;
        tiles_[index].y -= (diff + (32 - row)) * 2;
        *sinValue = FX_SinIdx(angle + column2 * 0x78);
        *cosValue = FX_CosIdx(angle + column2 * 0x78);
    }
}

THUMB void UnkScreenEffect_020302a0::draw()
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
        short count = 0;
        for (short i = 0; i < 32; i++) {
            for (short j = 0; j < 24; j++) {
                short index = j + i * 24;
                G3_PushMtx();
                G3_Scale(0x8000, 0x8000, 0x8000);
                G3_Translate(tiles_[index].x, tiles_[index].y, tiles_[index].z);
                short sinValue = FX_SinIdx(0);
                short cosValue = FX_CosIdx(0);
                unkfunc_020302e0(j, i, index, &sinValue, &cosValue);
                func_02065c60(sinValue, cosValue);
                unkfunc_0202b510(0x18);
                G3_Begin(1);
                for (int k = 0; k < 4; k++) {
                    G3_Color(0x7fff);
                    G3_TexCoord(tiles_[index].texCoord[k][0], tiles_[index].texCoord[k][1]);
                    G3_Vtx(tiles_[index].vertex[k].vx, tiles_[index].vertex[k].vy, tiles_[index].vertex[k].vz);
                }
                G3_PopMtx(1);
            }
            if (count > 2) {
                count = 0;
            } else {
                count++;
            }
        }
        unk_20 += 2;
        if (unk_20 > 20) {
            unk_24 += 2;
            if (unk_34 > 0xc000) {
                unk_34 -= 0xf0;
            } else {
                unk_34 = 0xc000;
            }
        }
        if (unk_30 > 0xc000) {
            unk_30 -= 0xf0;
        } else {
            unk_30 = 0xc000;
        }
        if (unk_2c < 0) {
            unk_2c += 0xffff;
        }
    }
}

THUMB bool UnkScreenEffect_020302a0::isEnd()
{
    return unk_20 > 75;
}
