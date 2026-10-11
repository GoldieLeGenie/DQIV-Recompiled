#ifndef FND_TYPES_H
#define FND_TYPES_H

#include <nitro/types.h>

/* Intrusive doubly linked list: every object embeds a UnkFndLink at UnkFndList.unk_0a bytes. */
typedef struct UnkFndLink {
    void* unk_00;               // 0x00 previous object
    void* unk_04;               // 0x04 next object
} UnkFndLink;

typedef struct UnkFndList {
    void* unk_00;               // 0x00 first object
    void* unk_04;               // 0x04 last object
    u16 unk_08;                 // 0x08 object count
    u16 unk_0a;                 // 0x0A offset of the link inside an object
} UnkFndList;

/* Common heap header (0x24 bytes), shared by the expanded / frame / unit heaps. */
typedef struct UnkFndHeapHead {
    u32 unk_00;                 // 0x00 signature
    UnkFndLink unk_04;          // 0x04 link in the parent heap's child list
    UnkFndList unk_0c;          // 0x0C child heaps
    void* unk_18;               // 0x18 heap start
    void* unk_1c;               // 0x1C heap end
    u32 unk_20;                 // 0x20 attributes (low byte: option flags)
} UnkFndHeapHead;

void func_02067dec(UnkFndList* list, u16 offset);
void func_02067e30(UnkFndList* list, void* obj);
void func_02067f38(UnkFndList* list, void* obj);
void* func_02067f98(UnkFndList* list, void* obj);

void func_02068054(UnkFndHeapHead* heap, u32 signature, void* start, void* end, u16 option);
void func_020680d0(UnkFndHeapHead* heap);

extern void func_0206785c(u32 value, void* dest, u32 size);

#endif
