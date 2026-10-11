#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/dss/TextureObject.hpp"
#include "nnsys/g3d.hpp"

// palette color effects (rgb rate, blend toward a color, sepia)
extern NNSG3dResTex* data_0211d430;             // last loaded model textures
extern int data_0211d434;
extern dss::Fix32Vector3 data_0211d438;         // rgb rate
extern unsigned short data_0211d444[0x800];     // palette work

void unkfunc_02085798(NNSG3dResTex* tex);
void unkfunc_020857a8(int index, dss::Fix32Vector3 rate);
void unkfunc_020857c8(NNSG3dResTex* tex, dss::Fix32Vector3 rate);            // rgb rate on the palette of model textures
void unkfunc_02085840(unsigned short* palette, int size, dss::Fix32Vector3& rate);   // rgb rate
void unkfunc_02085a54(unsigned short* palette, int size, dss::Fix32Vector3& color, dss::Fix32 rate);    // blend toward a color
void unkfunc_02085d88(NNSG3dResTex* tex);                                    // sepia on the palette of model textures
void unkfunc_02085de0(NNSG3dResTex* tex);                                    // sepia
float unkfunc_02085fe4(float value, float min, float max);
float unkfunc_02085ffc(float a, float b);                                    // max
float unkfunc_02086018(float a, float b);                                    // min
void unkfunc_02086034(TextureObject* texture, dss::Fix32* rgb);              // rgb rate on the palette of a texture
void unkfunc_020860b8(NNSG3dResTex* tex, int r, int g, int b, dss::Fix32 rate);   // blend the palette of model textures
