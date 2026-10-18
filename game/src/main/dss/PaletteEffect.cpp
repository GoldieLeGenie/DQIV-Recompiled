#pragma ipa file
#include "main/dss/PaletteEffect.hpp"
#include "main/dss/VramManager.hpp"
#include "nitro/os.hpp"

NNSG3dResTex* g_paletteEffectTex;
int data_0211d434;
dss::Fix32Vector3 g_paletteRgbRate(1, 1, 1);
unsigned short g_paletteEffectBuffer[0x800];

ARM void setPaletteEffectTexture(NNSG3dResTex* tex)
{
    g_paletteEffectTex = tex;
}

ARM void setPaletteRgbRate(int index, dss::Fix32Vector3 rate)
{
    data_0211d434 = index;
    g_paletteRgbRate = rate;
}

ARM void applyModelPaletteRgbRate(NNSG3dResTex* tex, dss::Fix32Vector3 rate)
{
    unsigned int size = tex->plttInfo.sizePltt << 3;
    unsigned int address = (tex->plttInfo.vramKey & 0xffff) << 3;
    MI_CpuCopyU16((unsigned char*)tex + tex->plttInfo.ofsPlttData, g_paletteEffectBuffer, size);
    applyPaletteRgbRate(g_paletteEffectBuffer, size, rate);
    DC_CleanAll();
    g_VramTransfer.transferToVram(1, g_paletteEffectBuffer, address, size, 0);
}

ARM void applyPaletteRgbRate(unsigned short* palette, int size, dss::Fix32Vector3& rate)
{
    for (int i = 0; i < size / 2; i++) {
        dss::Fix32 r;
        dss::Fix32 g;
        dss::Fix32 b;
        r = (long)(palette[i] & 0x1f);
        g = (long)((palette[i] & 0x3e0) >> 5);
        b = (long)((palette[i] & 0x7c00) >> 10);
        r = rate.vx * r;
        g = rate.vy * g;
        b = rate.vz * b;
        r = dss::clamp<dss::Fix32>(r, 0L, 31L);
        g = dss::clamp<dss::Fix32>(g, 0L, 31L);
        b = dss::clamp<dss::Fix32>(b, 0L, 31L);
        palette[i] = (unsigned char)(r.value >> 12) | ((unsigned char)(g.value >> 12) << 5) | ((unsigned char)(b.value >> 12) << 10);
    }
}

ARM void blendPaletteToColor(unsigned short* palette, int size, dss::Fix32Vector3& color, dss::Fix32 rate)
{
    for (int i = 0; i < size / 2; i++) {
        dss::Fix32 r;
        dss::Fix32 g;
        dss::Fix32 b;
        r = (long)(palette[i] & 0x1f);
        g = (long)((palette[i] & 0x3e0) >> 5);
        b = (long)((palette[i] & 0x7c00) >> 10);
        r = (r * (dss::Fix32(1L) - rate) + color.vx * rate) / dss::Fix32(1L);
        g = (g * (dss::Fix32(1L) - rate) + color.vy * rate) / dss::Fix32(1L);
        b = (b * (dss::Fix32(1L) - rate) + color.vz * rate) / dss::Fix32(1L);
        r = dss::clamp<dss::Fix32>(r, 0L, 31L);
        g = dss::clamp<dss::Fix32>(g, 0L, 31L);
        b = dss::clamp<dss::Fix32>(b, 0L, 31L);
        palette[i] = (unsigned char)(r.value >> 12) | ((unsigned char)(g.value >> 12) << 5) | ((unsigned char)(b.value >> 12) << 10);
    }
}

ARM void applyModelPaletteSepia(NNSG3dResTex* tex)
{
    unsigned int offset = tex->plttInfo.ofsPlttData;
    unsigned int size = tex->plttInfo.sizePltt << 3;
    unsigned int address = (tex->plttInfo.vramKey & 0xffff) << 3;
    applyPaletteSepia(tex);
    DC_CleanAll();
    g_VramTransfer.transferToVram(1, (unsigned char*)tex + offset, address, size, 0);
}

ARM void applyPaletteSepia(NNSG3dResTex* tex)
{
    unsigned int size = tex->plttInfo.sizePltt << 3;
    unsigned short* palette = (unsigned short*)((unsigned char*)tex + tex->plttInfo.ofsPlttData);
    for (unsigned int i = 0; i < size / 2; i++) {
        unsigned short color = *palette;
        float r = color & 0x1f;
        float g = (color & 0x3e0) >> 5;
        float b = (color & 0x7c00) >> 10;
        r *= 8.0f;
        g *= 8.0f;
        b *= 8.0f;
        float cb = -0.091f;
        float cr = 20.0f;
        float y = 0.114f * b + (0.299f * r + 0.587f * g);
        float r2 = y + 1.402f * cr;
        float g2 = y - 0.34414f * cb - 0.71414f * cr;
        float b2 = y + 1.772f * cb;
        r2 = clampFloat(r2, 0.0f, 255.0f);
        g2 = clampFloat(g2, 0.0f, 255.0f);
        b2 = clampFloat(b2, 0.0f, 255.0f);
        r2 /= 8.0f;
        g2 /= 8.0f;
        b2 /= 8.0f;
        *palette = (unsigned int)r2 | ((unsigned int)g2 << 5) | ((unsigned int)b2 << 10);
        palette++;
    }
}

ARM float clampFloat(float value, float min, float max)
{
    return minFloat(maxFloat(value, min), max);
}

ARM float maxFloat(float a, float b)
{
    return (a > b) ? a : b;
}

ARM float minFloat(float a, float b)
{
    return (a < b) ? a : b;
}

ARM void applyTexturePaletteRgbRate(TextureObject* texture, dss::Fix32* rgb)
{
    int data = texture->unkfunc_02086aac();
    int address = texture->unkfunc_02086aa4();
    int size = texture->unkfunc_02086ab4();
    MI_CpuCopyU16((void*)data, g_paletteEffectBuffer, size);
    applyPaletteRgbRate(g_paletteEffectBuffer, size, *(dss::Fix32Vector3*)rgb);
    DC_CleanAll();
    g_VramTransfer.transferToVram(1, g_paletteEffectBuffer, address, size, 0);
}

ARM void blendModelPaletteToColor(NNSG3dResTex* tex, int r, int g, int b, dss::Fix32 rate)
{
    unsigned int size = tex->plttInfo.sizePltt << 3;
    unsigned int address = (tex->plttInfo.vramKey & 0xffff) << 3;
    MI_CpuCopyU16((unsigned char*)tex + tex->plttInfo.ofsPlttData, g_paletteEffectBuffer, size);
    dss::Fix32Vector3 color;
    color.vx = (long)r;
    color.vy = dss::Fix32((long)g);
    color.vz = dss::Fix32((long)b);
    blendPaletteToColor(g_paletteEffectBuffer, size, color, rate);
    DC_CleanAll();
    g_VramTransfer.transferToVram(1, g_paletteEffectBuffer, address, size, 0);
}
