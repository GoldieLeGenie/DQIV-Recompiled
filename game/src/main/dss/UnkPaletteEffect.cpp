#pragma ipa file
#include "main/dss/UnkPaletteEffect.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "nitro/os.hpp"

NNSG3dResTex* data_0211d430;
int data_0211d434;
dss::Fix32Vector3 data_0211d438(1, 1, 1);
unsigned short data_0211d444[0x800];

ARM void unkfunc_02085798(NNSG3dResTex* tex)
{
    data_0211d430 = tex;
}

ARM void unkfunc_020857a8(int index, dss::Fix32Vector3 rate)
{
    data_0211d434 = index;
    data_0211d438 = rate;
}

ARM void unkfunc_020857c8(NNSG3dResTex* tex, dss::Fix32Vector3 rate)
{
    unsigned int size = tex->plttInfo.sizePltt << 3;
    unsigned int address = (tex->plttInfo.vramKey & 0xffff) << 3;
    MI_CpuCopyU16((unsigned char*)tex + tex->plttInfo.ofsPlttData, data_0211d444, size);
    unkfunc_02085840(data_0211d444, size, rate);
    DC_CleanAll();
    data_0211e450.unkfunc_02086378(1, data_0211d444, address, size, 0);
}

ARM void unkfunc_02085840(unsigned short* palette, int size, dss::Fix32Vector3& rate)
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

ARM void unkfunc_02085a54(unsigned short* palette, int size, dss::Fix32Vector3& color, dss::Fix32 rate)
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

ARM void unkfunc_02085d88(NNSG3dResTex* tex)
{
    unsigned int offset = tex->plttInfo.ofsPlttData;
    unsigned int size = tex->plttInfo.sizePltt << 3;
    unsigned int address = (tex->plttInfo.vramKey & 0xffff) << 3;
    unkfunc_02085de0(tex);
    DC_CleanAll();
    data_0211e450.unkfunc_02086378(1, (unsigned char*)tex + offset, address, size, 0);
}

ARM void unkfunc_02085de0(NNSG3dResTex* tex)
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
        r2 = unkfunc_02085fe4(r2, 0.0f, 255.0f);
        g2 = unkfunc_02085fe4(g2, 0.0f, 255.0f);
        b2 = unkfunc_02085fe4(b2, 0.0f, 255.0f);
        r2 /= 8.0f;
        g2 /= 8.0f;
        b2 /= 8.0f;
        *palette = (unsigned int)r2 | ((unsigned int)g2 << 5) | ((unsigned int)b2 << 10);
        palette++;
    }
}

ARM float unkfunc_02085fe4(float value, float min, float max)
{
    return unkfunc_02086018(unkfunc_02085ffc(value, min), max);
}

ARM float unkfunc_02085ffc(float a, float b)
{
    return (a > b) ? a : b;
}

ARM float unkfunc_02086018(float a, float b)
{
    return (a < b) ? a : b;
}

ARM void unkfunc_02086034(TextureObject* texture, dss::Fix32* rgb)
{
    int data = texture->unkfunc_02086aac();
    int address = texture->unkfunc_02086aa4();
    int size = texture->unkfunc_02086ab4();
    MI_CpuCopyU16((void*)data, data_0211d444, size);
    unkfunc_02085840(data_0211d444, size, *(dss::Fix32Vector3*)rgb);
    DC_CleanAll();
    data_0211e450.unkfunc_02086378(1, data_0211d444, address, size, 0);
}

ARM void unkfunc_020860b8(NNSG3dResTex* tex, int r, int g, int b, dss::Fix32 rate)
{
    unsigned int size = tex->plttInfo.sizePltt << 3;
    unsigned int address = (tex->plttInfo.vramKey & 0xffff) << 3;
    MI_CpuCopyU16((unsigned char*)tex + tex->plttInfo.ofsPlttData, data_0211d444, size);
    dss::Fix32Vector3 color;
    color.vx = (long)r;
    color.vy = dss::Fix32((long)g);
    color.vz = dss::Fix32((long)b);
    unkfunc_02085a54(data_0211d444, size, color, rate);
    DC_CleanAll();
    data_0211e450.unkfunc_02086378(1, data_0211d444, address, size, 0);
}
