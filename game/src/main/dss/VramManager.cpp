#include <nitro/native_video.h>
#include "main/dss/VramManager.hpp"
#include "nitro/os.hpp"

UnkVramTransfer data_0211e450;

ARM UnkVramTransfer::UnkVramTransfer()
{
    unk_00 = 0;
    unk_810 = 0;
    unk_814 = 1;
}

ARM UnkVramTransfer::~UnkVramTransfer()
{
}

ARM void UnkVramTransfer::initTransferQueue()
{
    func_02072800(tasks_, 0x80);
}

unsigned char data_0211ec68[0x800];
unsigned char data_0211f468[0x800];

ARM void UnkVramTransfer::initVramManagers(int texSize, int plttSize)
{
    texSize_ = texSize;
    unk_00 = 0;
    unk_04 = 0;
    unk_08 = texSize;
    func_02072c5c(texSize, 0, data_0211f468, func_02072c54(0x80), 1);
    if (texSize_ == 0x40000) {
        int handle1 = allocTexVram(0x20000);
        int handle2 = allocTexVram(0x20000);
        freeTexVram(handle1);
        freeTexVram(handle2);
    }
    func_02072fa8(plttSize, data_0211ec68, func_02072fa0(0x80), 1);
}

ARM void UnkVramTransfer::unkfunc_02086278()
{
}

ARM int UnkVramTransfer::allocTexVram(int size)
{
    return (*data_020c403c)(size, 0, 0);
}

ARM void UnkVramTransfer::freeTexVram(unsigned int handle)
{
    (*data_020c4040)(handle);
}

ARM int UnkVramTransfer::getTexVramAddr(unsigned int handle)
{
    return (handle & 0xffff) << 3;
}

ARM int UnkVramTransfer::getTexVramSize(unsigned int handle)
{
    return ((handle & 0x7fff0000) >> 16) << 4;
}

ARM int UnkVramTransfer::allocPlttVram(int size)
{
    int handle = (*data_020c4044)(size, 0, 0);
    unk_04 += size;
    return handle;
}

ARM void UnkVramTransfer::freePlttVram(unsigned int handle)
{
    (*data_020c4048)(handle);
    unk_04 -= getPlttVramSize(handle);
}

ARM int UnkVramTransfer::getPlttVramAddr(unsigned int handle)
{
    return (handle & 0xffff) << 3;
}

ARM int UnkVramTransfer::getPlttVramSize(unsigned int handle)
{
    return ((handle & 0xffff0000) >> 16) << 3;
}

ARM void UnkVramTransfer::transferToVram(int type, void* src, int address, unsigned int size, int flag)
{
    if (unk_814 != 0) {
        func_02072884(type, address, src, size);
        if (flag != 0) {
            doTransfers();
        }
        return;
    }
    if (flag != 0 && func_020728ec() != 0) {
        OS_Wait();
        doTransfers();
        while ((int)*VIDEO_REG(volatile unsigned short, 0x04000006) > 1) {
        }
    }
    unsigned int remain = size;
    do {
        unsigned int length;
        if (remain > 0x8000) {
            length = 0x8000;
            flag = 1;
        } else {
            length = remain;
        }
        func_02072884(type, address, src, length);
        if (flag != 0) {
            OS_Wait();
            doTransfers();
            while ((int)*VIDEO_REG(volatile unsigned short, 0x04000006) > 1) {
            }
        }
        address += length;
        src = (unsigned char*)src + length;
        remain -= length;
    } while (remain != 0);
}

ARM void UnkVramTransfer::doTransfers()
{
    func_02072824();
    unk_810 = 0;
}
