#include "fnd_types.h"

#define GET_LINK(list, obj) ((UnkFndLink*)((u32)(obj) + (list)->unk_0a))

void func_02067dec(UnkFndList* list, u16 offset) {
    list->unk_00 = NULL;
    list->unk_04 = NULL;
    list->unk_08 = 0;
    list->unk_0a = offset;
}

/* Inserts the first object into an empty list. */
void func_02067e04(UnkFndList* list, void* obj) {
    UnkFndLink* link = GET_LINK(list, obj);

    link->unk_04 = NULL;
    link->unk_00 = NULL;
    list->unk_00 = obj;
    list->unk_04 = obj;
    list->unk_08++;
}

/* Appends obj at the end of the list. */
void func_02067e30(UnkFndList* list, void* obj) {
    if (list->unk_00 == NULL) {
        func_02067e04(list, obj);
    } else {
        UnkFndLink* link = GET_LINK(list, obj);

        link->unk_00 = list->unk_04;
        link->unk_04 = NULL;
        GET_LINK(list, list->unk_04)->unk_04 = obj;
        list->unk_04 = obj;
        list->unk_08++;
    }
}

/* Prepends obj at the start of the list. */
void func_02067e84(UnkFndList* list, void* obj) {
    if (list->unk_00 == NULL) {
        func_02067e04(list, obj);
    } else {
        UnkFndLink* link = GET_LINK(list, obj);

        link->unk_00 = NULL;
        link->unk_04 = list->unk_00;
        GET_LINK(list, list->unk_00)->unk_00 = obj;
        list->unk_00 = obj;
        list->unk_08++;
    }
}

/* Inserts obj before target (appends when target is NULL). */
void func_02067ed4(UnkFndList* list, void* target, void* obj) {
    if (target == NULL) {
        func_02067e30(list, obj);
    } else if (target == list->unk_00) {
        func_02067e84(list, obj);
    } else {
        UnkFndLink* link = GET_LINK(list, obj);
        void* prev = GET_LINK(list, target)->unk_00;
        UnkFndLink* prevLink = GET_LINK(list, prev);

        link->unk_00 = prev;
        link->unk_04 = target;
        prevLink->unk_04 = obj;
        GET_LINK(list, target)->unk_00 = obj;
        list->unk_08++;
    }
}

/* Unlinks obj from the list. */
void func_02067f38(UnkFndList* list, void* obj) {
    UnkFndLink* link = GET_LINK(list, obj);

    if (link->unk_00 == NULL) {
        list->unk_00 = link->unk_04;
    } else {
        GET_LINK(list, link->unk_00)->unk_04 = link->unk_04;
    }
    if (link->unk_04 == NULL) {
        list->unk_04 = link->unk_00;
    } else {
        GET_LINK(list, link->unk_04)->unk_00 = link->unk_00;
    }
    link->unk_00 = NULL;
    link->unk_04 = NULL;
    list->unk_08--;
}

/* Returns the object after obj (the first object when obj is NULL). */
void* func_02067f98(UnkFndList* list, void* obj) {
    if (obj == NULL) {
        return list->unk_00;
    }
    return GET_LINK(list, obj)->unk_04;
}

/* Returns the object before obj (the last object when obj is NULL). */
void* func_02067fb0(UnkFndList* list, void* obj) {
    if (obj == NULL) {
        return list->unk_04;
    }
    return GET_LINK(list, obj)->unk_00;
}
