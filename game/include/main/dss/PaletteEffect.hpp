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

void setPaletteEffectTexture(NNSG3dResTex* tex);
void setPaletteRgbRate(int index, dss::Fix32Vector3 rate);
void applyModelPaletteRgbRate(NNSG3dResTex* tex, dss::Fix32Vector3 rate);            // rgb rate on the palette of model textures
void applyPaletteRgbRate(unsigned short* palette, int size, dss::Fix32Vector3& rate);   // rgb rate
void blendPaletteToColor(unsigned short* palette, int size, dss::Fix32Vector3& color, dss::Fix32 rate);    // blend toward a color
void applyModelPaletteSepia(NNSG3dResTex* tex);                                    // sepia on the palette of model textures
void applyPaletteSepia(NNSG3dResTex* tex);                                    // sepia
float clampFloat(float value, float min, float max);
float maxFloat(float a, float b);                                    // max
float minFloat(float a, float b);                                    // min
void applyTexturePaletteRgbRate(TextureObject* texture, dss::Fix32* rgb);              // rgb rate on the palette of a texture
void blendModelPaletteToColor(NNSG3dResTex* tex, int r, int g, int b, dss::Fix32 rate);   // blend the palette of model textures
