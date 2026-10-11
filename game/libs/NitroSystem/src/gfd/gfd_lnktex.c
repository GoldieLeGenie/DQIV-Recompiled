#include "gfd_lnkvram.h"

typedef struct {
    UnkLnkVramMan mgr;              /* 0x00 */
    UnkLnkVramMan mgrFor4x4;        /* 0x04 */
    UnkLnkVramBlock* pBlockPool;    /* 0x08 */
    u32 szByte;                     /* 0x0C */
    u32 szByteFor4x4;               /* 0x10 */
    void* pWork;                    /* 0x14 */
    u32 szWork;                     /* 0x18 */
} UnkLnkTexVramManager;

typedef struct {
    u32 szFree;                     /* 0x00 */
    u32 szNrm;                      /* 0x04 */
    u32 sz4x4;                      /* 0x08 */
} UnkLnkTexSlot;

typedef struct {
    UnkLnkTexSlot slot[4];
} UnkLnkTexSlotArray;

typedef u32 (*UnkAllocTexVramFunc)(u32 szByte, BOOL is4x4comp, u32 opt);
typedef int (*UnkFreeTexVramFunc)(u32 key);

UnkLnkTexVramManager data_0210feb0;
extern UnkAllocTexVramFunc data_020c403c;
extern UnkFreeTexVramFunc data_020c4040;

void func_02072db8(void);
u32 func_02072cb4(u32 szByte, BOOL is4x4comp, u32 opt);
int func_02072d44(u32 texKey);


static inline u32 KeyAddr_(u32 key) { return (u32)((key & 0xffff) << 3); }
static inline u32 KeySize_(u32 key) { return (u32)(((key & 0x7fff0000) >> 16) << 4); }
static inline BOOL Key4x4_(u32 key) { return (BOOL)((key & 0x80000000) >> 31); }

u32 func_02072c54(u32 numMemBlk)
{
    return numMemBlk * sizeof(UnkLnkVramBlock);
}

void func_02072c5c(u32 szByte, u32 szByteFor4x4, void* pManagementWork, u32 szManagementWork, BOOL useAsDefault)
{
    UnkLnkTexVramManager* pMgr = &data_0210feb0;

    pMgr->szByte = szByte;
    pMgr->szByteFor4x4 = szByteFor4x4;
    pMgr->pWork = pManagementWork;
    pMgr->szWork = szManagementWork;
    func_02072db8();

    if (useAsDefault) {
        data_020c403c = func_02072cb4;
        data_020c4040 = func_02072d44;
    }
}

u32 func_02072cb4(u32 szByte, BOOL is4x4comp, u32 opt)
{
    u32 addr;
    BOOL result;
    u32 sz = (szByte == 0) ? 16 : ((szByte + 15) & ~15);

    if (sz >= 0x7fff0) {
        return 0;
    }

    if (is4x4comp) {
        result = func_020729b0(&data_0210feb0.mgrFor4x4, &data_0210feb0.pBlockPool, &addr, sz);
    } else {
        result = func_020729b0(&data_0210feb0.mgr, &data_0210feb0.pBlockPool, &addr, sz);
    }
    if (!result) {
        return 0;
    }
    return ((sz >> 4) << 16) | ((addr >> 3) & 0xffff) | (is4x4comp << 31);
}

int func_02072d44(u32 texKey)
{
    u32 addr = KeyAddr_(texKey);
    u32 szByte = KeySize_(texKey);
    BOOL is4x4comp = Key4x4_(texKey);
    BOOL result;

    if (szByte != 0) {
        if (is4x4comp) {
            result = func_02072b04(&data_0210feb0.mgrFor4x4, &data_0210feb0.pBlockPool, addr, szByte);
        } else {
            result = func_02072b04(&data_0210feb0.mgr, &data_0210feb0.pBlockPool, addr, szByte);
        }
        if (result) {
            return 0;
        } else {
            return 1;
        }
    }
    return 2;
}

void func_02072db8(void)
{
    UnkLnkTexSlotArray slots = {{{0x20000, 0, 0}, {0x20000, 0, 0}, {0x20000, 0, 0}, {0x20000, 0, 0}}};
    UnkLnkTexSlot* pSlot;
    u32 szNrm;
    u32 sz4x4;
    u32 i;
    u32 sz;
    u32 szIdx;
    UnkLnkTexSlot* pSlot2;
    u32 ofs;

    sz4x4 = data_0210feb0.szByteFor4x4;
    szNrm = data_0210feb0.szByte - (sz4x4 + sz4x4 / 2);
    szIdx = sz4x4 / 2;

    pSlot = slots.slot;
    for (i = 0; i < 4; i++, pSlot++) {
        if (i == 0 || i == 2) {
            if (pSlot->szFree != 0 && sz4x4 != 0) {
                sz = pSlot->szFree;
                if (sz > sz4x4) {
                    sz = sz4x4;
                }
                pSlot->sz4x4 += sz;
                sz4x4 -= sz;
                pSlot->szFree -= sz;
            }
        }
    }
    slots.slot[1].szFree -= szIdx;

    pSlot2 = slots.slot;
    for (i = 0; i < 4; i++, pSlot2++) {
        if (pSlot2->szFree != 0 && szNrm != 0) {
            sz = pSlot2->szFree;
            if (sz > szNrm) {
                sz = szNrm;
            }
            pSlot2->szNrm += sz;
            szNrm -= sz;
            pSlot2->szFree -= sz;
        }
    }

    func_020728fc(&data_0210feb0.mgr);
    func_020728fc(&data_0210feb0.mgrFor4x4);
    data_0210feb0.pBlockPool = func_02072908(data_0210feb0.pWork, data_0210feb0.szWork / sizeof(UnkLnkVramBlock));

    if (slots.slot[0].sz4x4 != 0) {
        func_02072954(&data_0210feb0.mgrFor4x4, &data_0210feb0.pBlockPool, 0, slots.slot[0].sz4x4);
    }
    ofs = slots.slot[0].sz4x4;
    if (slots.slot[0].szNrm != 0) {
        func_02072954(&data_0210feb0.mgr, &data_0210feb0.pBlockPool, ofs, slots.slot[0].szNrm);
    }
    if (slots.slot[2].sz4x4 != 0) {
        func_02072954(&data_0210feb0.mgrFor4x4, &data_0210feb0.pBlockPool, 0x40000, slots.slot[2].sz4x4);
    }
    ofs = slots.slot[2].sz4x4;
    if (slots.slot[2].szNrm != 0) {
        func_02072954(&data_0210feb0.mgr, &data_0210feb0.pBlockPool, 0x40000 + ofs, slots.slot[2].szNrm);
    }
    if (slots.slot[3].szNrm != 0) {
        func_02072954(&data_0210feb0.mgr, &data_0210feb0.pBlockPool, 0x60000, slots.slot[3].szNrm);
    }
    if (slots.slot[1].szNrm != 0) {
        func_02072954(&data_0210feb0.mgr, &data_0210feb0.pBlockPool, 0x20000 + szIdx, slots.slot[1].szNrm);
    }
}
