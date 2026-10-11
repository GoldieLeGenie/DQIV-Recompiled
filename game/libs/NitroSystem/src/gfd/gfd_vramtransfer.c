#include "../nns_internal.h"

typedef struct {
    u32 type;           /* 0x00 */
    const void* pSrc;   /* 0x04 */
    u32 dstAddr;        /* 0x08 */
    u32 szByte;         /* 0x0C */
} UnkVramTransferTask;

typedef struct {
    UnkVramTransferTask* pTaskArray; /* 0x00 */
    u32 lengthOfArray;               /* 0x04 */
    u16 idxFront;                    /* 0x08 */
    u16 idxRear;                     /* 0x0A */
    u16 numTasks;                    /* 0x0C */
    u32 totalSize;                   /* 0x10 */
} UnkVramTransferManager;

typedef void (*UnkVramLoadFunc)(const void* pSrc, u32 dstAddr, u32 szByte);

UnkVramTransferManager data_0210fe9c;

void func_02066958(void);
void func_020669b4(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066af4(void);
void func_02066b40(void);
void func_02066b74(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066be0(void);
void func_02066c24(void);
void func_02066cb4(const void* pSrc, u32 szByte);
void func_02066d1c(const void* pSrc, u32 szByte);
void func_02066d88(void);
void func_02066290(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066350(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066410(const void* pSrc, u32 dstAddr, u32 szByte);
void func_020664d0(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02065f90(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066050(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066110(const void* pSrc, u32 dstAddr, u32 szByte);
void func_020661d0(const void* pSrc, u32 dstAddr, u32 szByte);
void GX_LoadOBJPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GX_LoadBGPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GX_BeginLoadOBJExtPltt(void);
void GX_LoadOBJExtPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GX_EndLoadOBJExtPltt(void);
void GX_BeginLoadBGExtPltt(void);
void GX_LoadBGExtPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GX_EndLoadBGExtPltt(void);
void GX_LoadOAM(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02065ee0(const void* pSrc, u32 dstAddr, u32 szByte);
void func_020662f0(const void* pSrc, u32 dstAddr, u32 szByte);
void func_020663b0(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066470(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066530(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02065ff0(const void* pSrc, u32 dstAddr, u32 szByte);
void func_020660b0(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066170(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02066230(const void* pSrc, u32 dstAddr, u32 szByte);
void GXS_LoadOBJPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GXS_LoadBGPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GXS_BeginLoadOBJExtPltt(void);
void GXS_LoadOBJExtPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GXS_EndLoadOBJExtPltt(void);
void GXS_BeginLoadBGExtPltt(void);
void GXS_LoadBGExtPltt(const void* pSrc, u32 dstAddr, u32 szByte);
void GXS_EndLoadBGExtPltt(void);
void GXS_LoadOAM(const void* pSrc, u32 dstAddr, u32 szByte);
void func_02065f38(const void* pSrc, u32 dstAddr, u32 szByte);

u16 func_02072434(UnkVramTransferManager* mgr, u16 idx)
{
    return (u16)((idx + 1) % mgr->lengthOfArray);
}

BOOL func_02072454(UnkVramTransferManager* mgr)
{
    return mgr->numTasks == mgr->lengthOfArray;
}

BOOL func_0207246c(UnkVramTransferManager* mgr)
{
    return mgr->numTasks == 0;
}

void func_02072480(const void* pSrc, u32 dstAddr, u32 szByte)
{
    func_02066958();
    func_020669b4(pSrc, dstAddr, szByte);
    func_02066af4();
}

void func_020724ac(const void* pSrc, u32 dstAddr, u32 szByte)
{
    func_02066b40();
    func_02066b74(pSrc, dstAddr, szByte);
    func_02066be0();
}

void func_020724d8(const void* pSrc, u32 dstAddr, u32 szByte)
{
    func_02066c24();
    func_02066cb4(pSrc, szByte);
    func_02066d88();
}

void func_020724fc(const void* pSrc, u32 dstAddr, u32 szByte)
{
    func_02066c24();
    func_02066d1c(pSrc, szByte);
    func_02066d88();
}

void func_02072520(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066290(pSrc, dstAddr, szByte); }
void func_0207252c(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066350(pSrc, dstAddr, szByte); }
void func_02072538(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066410(pSrc, dstAddr, szByte); }
void func_02072544(const void* pSrc, u32 dstAddr, u32 szByte) { func_020664d0(pSrc, dstAddr, szByte); }
void func_02072550(const void* pSrc, u32 dstAddr, u32 szByte) { func_02065f90(pSrc, dstAddr, szByte); }
void func_0207255c(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066050(pSrc, dstAddr, szByte); }
void func_02072568(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066110(pSrc, dstAddr, szByte); }
void func_02072574(const void* pSrc, u32 dstAddr, u32 szByte) { func_020661d0(pSrc, dstAddr, szByte); }
void func_02072580(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066110(pSrc, dstAddr, szByte); }
void func_0207258c(const void* pSrc, u32 dstAddr, u32 szByte) { func_020661d0(pSrc, dstAddr, szByte); }
void func_02072598(const void* pSrc, u32 dstAddr, u32 szByte) { GX_LoadOBJPltt(pSrc, dstAddr, szByte); }
void func_020725a4(const void* pSrc, u32 dstAddr, u32 szByte) { GX_LoadBGPltt(pSrc, dstAddr, szByte); }

void func_020725b0(const void* pSrc, u32 dstAddr, u32 szByte)
{
    GX_BeginLoadOBJExtPltt();
    GX_LoadOBJExtPltt(pSrc, dstAddr, szByte);
    GX_EndLoadOBJExtPltt();
}

void func_020725dc(const void* pSrc, u32 dstAddr, u32 szByte)
{
    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(pSrc, dstAddr, szByte);
    GX_EndLoadBGExtPltt();
}

void func_02072608(const void* pSrc, u32 dstAddr, u32 szByte) { GX_LoadOAM(pSrc, dstAddr, szByte); }
void func_02072614(const void* pSrc, u32 dstAddr, u32 szByte) { func_02065ee0(pSrc, dstAddr, szByte); }
void func_02072620(const void* pSrc, u32 dstAddr, u32 szByte) { func_020662f0(pSrc, dstAddr, szByte); }
void func_0207262c(const void* pSrc, u32 dstAddr, u32 szByte) { func_020663b0(pSrc, dstAddr, szByte); }
void func_02072638(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066470(pSrc, dstAddr, szByte); }
void func_02072644(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066530(pSrc, dstAddr, szByte); }
void func_02072650(const void* pSrc, u32 dstAddr, u32 szByte) { func_02065ff0(pSrc, dstAddr, szByte); }
void func_0207265c(const void* pSrc, u32 dstAddr, u32 szByte) { func_020660b0(pSrc, dstAddr, szByte); }
void func_02072668(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066170(pSrc, dstAddr, szByte); }
void func_02072674(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066230(pSrc, dstAddr, szByte); }
void func_02072680(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066170(pSrc, dstAddr, szByte); }
void func_0207268c(const void* pSrc, u32 dstAddr, u32 szByte) { func_02066230(pSrc, dstAddr, szByte); }
void func_02072698(const void* pSrc, u32 dstAddr, u32 szByte) { GXS_LoadOBJPltt(pSrc, dstAddr, szByte); }
void func_020726a4(const void* pSrc, u32 dstAddr, u32 szByte) { GXS_LoadBGPltt(pSrc, dstAddr, szByte); }

void func_020726b0(const void* pSrc, u32 dstAddr, u32 szByte)
{
    GXS_BeginLoadOBJExtPltt();
    GXS_LoadOBJExtPltt(pSrc, dstAddr, szByte);
    GXS_EndLoadOBJExtPltt();
}

void func_020726dc(const void* pSrc, u32 dstAddr, u32 szByte)
{
    GXS_BeginLoadBGExtPltt();
    GXS_LoadBGExtPltt(pSrc, dstAddr, szByte);
    GXS_EndLoadBGExtPltt();
}

void func_02072708(const void* pSrc, u32 dstAddr, u32 szByte) { GXS_LoadOAM(pSrc, dstAddr, szByte); }
void func_02072714(const void* pSrc, u32 dstAddr, u32 szByte) { func_02065f38(pSrc, dstAddr, szByte); }

/* run one transfer task */
/* VRAM load function per transfer type */
const UnkVramLoadFunc data_020ba628[36] = {
    func_02072480, func_020724ac, func_020724d8, func_020724fc,
    func_02072520, func_0207252c, func_02072538, func_02072544,
    func_02072550, func_0207255c, func_02072568, func_02072574,
    func_02072580, func_0207258c, func_02072598, func_020725a4,
    func_020725b0, func_020725dc, func_02072608, func_02072614,
    func_02072620, func_0207262c, func_02072638, func_02072644,
    func_02072650, func_0207265c, func_02072668, func_02072674,
    func_02072680, func_0207268c, func_02072698, func_020726a4,
    func_020726b0, func_020726dc, func_02072708, func_02072714,
};

void func_02072720(UnkVramTransferTask* task)
{
    UnkVramLoadFunc func = data_020ba628[task->type];
    DC_PurgeRange(task->pSrc, task->szByte);
    func(task->pSrc, task->dstAddr, task->szByte);
}

void func_02072750(UnkVramTransferManager* mgr)
{
    mgr->idxRear = 0;
    mgr->idxFront = 0;
    mgr->numTasks = 0;
    mgr->totalSize = 0;
}

BOOL func_02072768(UnkVramTransferManager* mgr)
{
    if (func_02072454(mgr)) {
        return FALSE;
    }
    mgr->idxRear = func_02072434(mgr, mgr->idxRear);
    mgr->numTasks++;
    return TRUE;
}

UnkVramTransferTask* func_020727a4(UnkVramTransferManager* mgr)
{
    return &mgr->pTaskArray[mgr->idxFront];
}

UnkVramTransferTask* func_020727b4(UnkVramTransferManager* mgr)
{
    return &mgr->pTaskArray[mgr->idxRear];
}

BOOL func_020727c4(UnkVramTransferManager* mgr)
{
    if (func_0207246c(mgr)) {
        return FALSE;
    }
    mgr->idxFront = func_02072434(mgr, mgr->idxFront);
    mgr->numTasks--;
    return TRUE;
}

void func_02072800(UnkVramTransferTask* pTaskArray, u32 lengthOfArray)
{
    data_0210fe9c.pTaskArray = pTaskArray;
    data_0210fe9c.lengthOfArray = lengthOfArray;
    func_02072750(&data_0210fe9c);
}

void func_02072824(void)
{
    UnkVramTransferManager* mgr = &data_0210fe9c;
    UnkVramTransferTask* task = func_020727a4(mgr);

    while (func_020727c4(mgr)) {
        func_02072720(task);
        mgr->totalSize -= task->szByte;
        task = func_020727a4(mgr);
    }
}

BOOL func_02072884(u32 type, u32 dstAddr, const void* pSrc, u32 szByte)
{
    UnkVramTransferManager* mgr = &data_0210fe9c;
    UnkVramTransferTask* task;

    if (func_02072454(mgr)) {
        return FALSE;
    }
    task = func_020727b4(mgr);
    task->type = type;
    task->pSrc = pSrc;
    task->dstAddr = dstAddr;
    task->szByte = szByte;
    func_02072768(mgr);
    mgr->totalSize += task->szByte;
    return TRUE;
}

u32 func_020728ec(void)
{
    return data_0210fe9c.totalSize;
}
