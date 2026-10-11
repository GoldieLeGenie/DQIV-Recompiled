#include "gfd_lnkvram.h"

typedef struct {
    UnkLnkVramMan mgr;              /* 0x00 */
    UnkLnkVramBlock* pBlockPool;    /* 0x04 */
    u32 szByte;                     /* 0x08 */
    void* pWork;                    /* 0x0C */
    u32 szWork;                     /* 0x10 */
} UnkLnkPlttVramManager;

typedef u32 (*UnkAllocPlttVramFunc)(u32 szByte, BOOL is4pltt, u32 opt);
typedef int (*UnkFreePlttVramFunc)(u32 key);

UnkLnkPlttVramManager data_0210fecc;
extern UnkAllocPlttVramFunc data_020c4044;
extern UnkFreePlttVramFunc data_020c4048;

void func_0207310c(void);
u32 func_02072ffc(u32 szByte, BOOL is4pltt, u32 opt);
int func_020730c8(u32 plttKey);

u32 func_02072fa0(u32 numMemBlk)
{
    return numMemBlk * sizeof(UnkLnkVramBlock);
}

void func_02072fa8(u32 szByte, void* pManagementWork, u32 szManagementWork, BOOL useAsDefault)
{
    UnkLnkPlttVramManager* pMgr = &data_0210fecc;

    pMgr->szByte = szByte;
    pMgr->pWork = pManagementWork;
    pMgr->szWork = szManagementWork;
    func_0207310c();

    if (useAsDefault) {
        data_020c4044 = func_02072ffc;
        data_020c4048 = func_020730c8;
    }
}

u32 func_02072ffc(u32 szByte, BOOL is4pltt, u32 opt)
{
    u32 addr;
    BOOL result;
    u32 sz = (szByte == 0) ? 8 : ((szByte + 7) & ~7);

    if (sz >= 0x7fff8) {
        return 0;
    }

    if (is4pltt) {
        result = func_020729c4(&data_0210fecc.mgr, &data_0210fecc.pBlockPool, &addr, sz, 8);
        if (addr + sz > 0x10000) {
            func_02072b04(&data_0210fecc.mgr, &data_0210fecc.pBlockPool, addr, sz);
            return 0;
        }
    } else {
        result = func_020729c4(&data_0210fecc.mgr, &data_0210fecc.pBlockPool, &addr, sz, 16);
    }
    if (!result) {
        return 0;
    }
    return ((sz >> 3) << 16) | ((addr >> 3) & 0xffff);
}

int func_020730c8(u32 plttKey)
{
    u32 addr = (plttKey & 0xffff) << 3;
    u32 szByte = ((plttKey & 0xffff0000) >> 16) << 3;
    BOOL result = func_02072b04(&data_0210fecc.mgr, &data_0210fecc.pBlockPool, addr, szByte);

    return result ? 0 : 1;
}

void func_0207310c(void)
{
    data_0210fecc.pBlockPool = func_02072908(data_0210fecc.pWork, data_0210fecc.szWork / sizeof(UnkLnkVramBlock));
    func_020728fc(&data_0210fecc.mgr);
    func_02072954(&data_0210fecc.mgr, &data_0210fecc.pBlockPool, 0, data_0210fecc.szByte);
}
