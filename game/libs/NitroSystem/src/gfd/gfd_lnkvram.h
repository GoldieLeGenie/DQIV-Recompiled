#ifndef GFD_LNKVRAM_H
#define GFD_LNKVRAM_H

#include "../nns_internal.h"

typedef struct UnkLnkVramBlock {
    u32 addr;                       /* 0x00 */
    u32 szByte;                     /* 0x04 */
    struct UnkLnkVramBlock* pPrev;  /* 0x08 */
    struct UnkLnkVramBlock* pNext;  /* 0x0C */
} UnkLnkVramBlock;

typedef struct {
    UnkLnkVramBlock* pFreeList;     /* 0x00 */
} UnkLnkVramMan;

void func_020728fc(UnkLnkVramMan* pMgr);
UnkLnkVramBlock* func_02072908(UnkLnkVramBlock* pArray, u32 num);
BOOL func_02072954(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32 baseAddr, u32 szByte);
BOOL func_020729b0(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32* pRetAddr, u32 szByte);
BOOL func_020729c4(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32* pRetAddr, u32 szByte, u32 alignment);
BOOL func_02072b04(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32 baseAddr, u32 szByte);

#endif
