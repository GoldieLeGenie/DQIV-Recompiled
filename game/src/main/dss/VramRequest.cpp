#include <nitro/native_video.h>
#include "main/dss/VramRequest.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkMemory.hpp"

ARM void UnkOamBuffer::init()
{
}

ARM void UnkOamBuffer::clear()
{
    func_0206785c(0xc0, oam_, sizeof(oam_));
    prevCount_ = count_;
    count_ = 0;
    unk_408 = 0;
}

ARM void UnkOamBuffer::requestTransfer(int type)
{
    requestVramCopy(type, 0, oam_, sizeof(oam_));
}

ARM GXOamAttr* UnkOamBuffer::allocEntry()
{
    GXOamAttr* oam = &oam_[count_];
    count_++;
    return oam;
}

ARM void UnkPaletteBuffer::requestTransfer(int type)
{
    requestVramCopy(type, 0, color_, sizeof(color_));
}

ARM void UnkPaletteBuffer::setColor(int index, int color, unsigned short rgb)
{
    color_[index * 16 + color] = rgb;
}

ARM void UnkPaletteBuffer::loadColors(int index, int color, const void* src, int count)
{
    MI_CpuCopyU32(src, &color_[index * 16 + color], count * 2);
}

ARM void UnkVramRequestQueue::init()
{
    for (int i = 0; i < 128; i++) {
        request_[i].active_ = 0;
    }
    size_ = 0;
    lines_ = 0;
}

UnkVramRequestQueue data_0211c508;

ARM void flushVramRequests()
{
    int start;
    UnkVramRequest* request;
    int size;
    int i;
    size = 0;
    start = *VIDEO_REG(volatile unsigned short, 0x04000006);
    request = data_0211c508.request_;
    for (i = 0; i < 128; i++, request++) {
        if (request->active_ == 1) {
            size += data_0211c508.executeRequest(request);
        }
    }
    int end = *VIDEO_REG(volatile unsigned short, 0x04000006);
    data_0211c508.size_ = size;
    data_0211c508.lines_ = end - start;
}

ARM UnkVramRequest* requestCompressedVramCopy(int type, int offset, void* data, int size)
{
    void* buffer = uncompressAlloc(data);
    UnkVramRequest* request = requestVramCopy(type, offset, buffer, getUncompressedSize(data));
    if (request != NULL) {
        request->buffer_ = buffer;
        return request;
    }
    dssFree(&data_0211a60c, buffer);
    return NULL;
}

ARM UnkVramRequest* requestVramCopy(int type, int offset, const void* src, int size)
{
    for (int i = 0; i < 128; i++) {
        UnkVramRequest* request = &data_0211c508.request_[i];
        if (request->active_ == 0) {
            void* base = data_0211c508.getDestination(type);
            request->active_ = 1;
            request->type_ = type;
            request->offset_ = offset;
            request->base_ = base;
            request->src_ = src;
            request->size_ = size;
            request->buffer_ = NULL;
            return request;
        }
    }
    return NULL;
}

ARM void* UnkVramRequestQueue::getDestination(int type)
{
    switch (type) {
    case 4:
        return func_02064d68();
    case 5:
        return func_02064dbc();
    case 6:
        return func_02064e10();
    case 7:
        return func_02064ea0();
    case 20:
        return func_02064d9c();
    case 21:
        return func_02064df0();
    case 22:
        return func_02064e60();
    case 23:
        return func_02064ef8();
    case 8:
        return func_02064ad0();
    case 9:
        return func_02064b24();
    case 10:
        return func_02064b78();
    case 11:
        return func_02064c70();
    case 24:
        return func_02064b04();
    case 25:
        return func_02064b58();
    case 26:
        return func_02064bfc();
    case 27:
        return func_02064cf4();
    case 15:
        return (void*)0x05000000;
    case 31:
        return (void*)0x05000400;
    case 19:
        return (void*)0x06400000;
    case 35:
        return (void*)0x06600000;
    case 14:
        return (void*)0x05000200;
    case 30:
        return (void*)0x05000600;
    case 18:
        return (void*)0x07000000;
    case 34:
        return (void*)0x07000400;
    }
    return NULL;
}

ARM int UnkVramRequestQueue::executeRequest(UnkVramRequest* request)
{
#ifdef DQIV_NATIVE
    pcVideoCopy((unsigned int)getDestination(request->type_) + request->offset_, request->src_, request->size_);
#else
    MI_CpuCopyU32(request->src_, (unsigned char*)getDestination(request->type_) + request->offset_, request->size_);
#endif
    if (request->buffer_ != NULL) {
        dssFree(&data_0211a60c, request->buffer_);
    }
    request->active_ = 0;
    return request->size_;
}

ARM int getBgCharVramType(int bg)
{
    switch (bg) {
    case 0:
        return 4;
    case 1:
        return 5;
    case 2:
        return 6;
    case 3:
        return 7;
    case 4:
        return 20;
    case 5:
        return 21;
    case 6:
        return 22;
    case 7:
        return 23;
    }
    return -1;
}

ARM int getBgScreenVramType(int bg)
{
    switch (bg) {
    case 0:
        return 8;
    case 1:
        return 9;
    case 2:
        return 10;
    case 3:
        return 11;
    case 4:
        return 24;
    case 5:
        return 25;
    case 6:
        return 26;
    case 7:
        return 27;
    }
    return -1;
}
