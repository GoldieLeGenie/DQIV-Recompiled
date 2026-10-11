#include "gfd_lnkvram.h"

typedef struct {
    u32 start;
    u32 end;
} UnkLnkVramRegion;

static inline UnkLnkVramBlock* GetNewBlock_(UnkLnkVramBlock** ppBlockPool)
{
    UnkLnkVramBlock* pBlk = *ppBlockPool;
    if (pBlk != NULL) {
        *ppBlockPool = pBlk->pNext;
    }
    return pBlk;
}

static inline void InitBlock_(UnkLnkVramBlock* pBlk, u32 addr, u32 szByte)
{
    pBlk->addr = addr;
    pBlk->szByte = szByte;
    pBlk->pPrev = NULL;
    pBlk->pNext = NULL;
}

static inline void InsertBlock_(UnkLnkVramBlock** ppList, UnkLnkVramBlock* pBlk)
{
    if (*ppList != NULL) {
        (*ppList)->pPrev = pBlk;
    }
    pBlk->pNext = *ppList;
    pBlk->pPrev = NULL;
    *ppList = pBlk;
}

static inline void RemoveBlock_(UnkLnkVramBlock** ppList, UnkLnkVramBlock* pBlk)
{
    UnkLnkVramBlock* pPrev = pBlk->pPrev;
    UnkLnkVramBlock* pNext = pBlk->pNext;

    if (pPrev != NULL) {
        pPrev->pNext = pNext;
    } else {
        *ppList = pNext;
    }
    if (pNext != NULL) {
        pNext->pPrev = pPrev;
    }
}

void func_020728fc(UnkLnkVramMan* pMgr)
{
    pMgr->pFreeList = NULL;
}

/* links an array of blocks into a pool list */
UnkLnkVramBlock* func_02072908(UnkLnkVramBlock* pArray, u32 num)
{
    u32 i;

    for (i = 0; i < num - 1; i++) {
        pArray[i].pNext = &pArray[i + 1];
        pArray[i + 1].pPrev = &pArray[i];
    }
    pArray[0].pPrev = NULL;
    (pArray + num - 1)->pNext = NULL;
    return pArray;
}

/* adds a free region to the manager */
BOOL func_02072954(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32 baseAddr, u32 szByte)
{
    UnkLnkVramBlock* pBlk = GetNewBlock_(ppBlockPool);

    if (pBlk != NULL) {
        InitBlock_(pBlk, baseAddr, szByte);
        InsertBlock_(&pMgr->pFreeList, pBlk);
        return TRUE;
    }
    return FALSE;
}

BOOL func_020729b0(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32* pRetAddr, u32 szByte)
{
    return func_020729c4(pMgr, ppBlockPool, pRetAddr, szByte, 0);
}

/* allocates szByte from the free list (first fit) */
BOOL func_020729c4(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32* pRetAddr, u32 szByte, u32 alignment)
{
    u32 alignedAddr;
    u32 needSize;
    u32 alignOfs;
    UnkLnkVramBlock* pFound = NULL;
    UnkLnkVramBlock* pBlk = pMgr->pFreeList;

    while (pBlk != NULL) {
        if (alignment > 1) {
            alignedAddr = (pBlk->addr + (alignment - 1)) & ~(alignment - 1);
            alignOfs = alignedAddr - pBlk->addr;
            needSize = szByte + alignOfs;
        } else {
            alignedAddr = pBlk->addr;
            alignOfs = 0;
            needSize = szByte;
        }
        if (pBlk->szByte >= needSize) {
            pFound = pBlk;
            break;
        }
        pBlk = pBlk->pNext;
    }

    if (pFound != NULL) {
        if (alignOfs != 0) {
            UnkLnkVramBlock* pNew = GetNewBlock_(ppBlockPool);
            if (pNew == NULL) {
                goto fail;
            }
            InitBlock_(pNew, pFound->addr, alignOfs);
            InsertBlock_(&pMgr->pFreeList, pNew);
        }
        pFound->szByte -= needSize;
        pFound->addr += needSize;
        if (pFound->szByte == 0) {
            RemoveBlock_(&pMgr->pFreeList, pFound);
            InsertBlock_(ppBlockPool, pFound);
        }
        *pRetAddr = alignedAddr;
        return TRUE;
    }
fail:
    *pRetAddr = 0;
    return FALSE;
}

/* returns a region to the free list, merging it with adjacent free blocks */
BOOL func_02072b04(UnkLnkVramMan* pMgr, UnkLnkVramBlock** ppBlockPool, u32 baseAddr, u32 szByte)
{
    UnkLnkVramRegion region;
    UnkLnkVramRegion newRegion;
    UnkLnkVramRegion* pRegion = &region;
    UnkLnkVramBlock* pBlk;
    UnkLnkVramBlock* pNext;

    region.start = baseAddr;
    region.end = baseAddr + szByte;
    newRegion = region;

    pBlk = pMgr->pFreeList;
    while (pBlk != NULL) {
        pNext = pBlk->pNext;
        if (pBlk->addr == pRegion->end) {
            newRegion.end = pBlk->addr + pBlk->szByte;
            RemoveBlock_(&pMgr->pFreeList, pBlk);
            InsertBlock_(ppBlockPool, pBlk);
        }
        if (pBlk->addr + pBlk->szByte == pRegion->start) {
            newRegion.start = pBlk->addr;
            RemoveBlock_(&pMgr->pFreeList, pBlk);
            InsertBlock_(ppBlockPool, pBlk);
        }
        pBlk = pNext;
    }

    pBlk = GetNewBlock_(ppBlockPool);
    if (pBlk == NULL) {
        return FALSE;
    }
    InitBlock_(pBlk, newRegion.start, newRegion.end - newRegion.start);
    InsertBlock_(&pMgr->pFreeList, pBlk);
    return TRUE;
}
