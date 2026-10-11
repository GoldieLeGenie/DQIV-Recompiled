#include <nitro/types.h>

/* String splitters: read one character code from a string and advance the pointer. */

/* UTF-16 */
u16 func_0206a174(const void** str) {
    const u16* p = (const u16*)*str;
    u16 c = *p++;

    *str = p;
    return c;
}

/* UTF-8 */
u16 func_0206a188(const void** str) {
    const u8* p = (const u8*)*str;
    u16 c;

    if ((p[0] & 0x80) == 0) {
        c = p[0];
        *str = p + 1;
    } else if ((p[0] & 0xe0) == 0xc0) {
        c = (u16)(((p[0] & 0x1f) << 6) | (p[1] & 0x3f));
        *str = p + 2;
    } else {
        c = (u16)(((p[0] & 0x1f) << 12) | ((p[1] & 0x3f) << 6) | (p[2] & 0x3f));
        *str = p + 3;
    }
    return c;
}

/* Shift JIS */
u16 func_0206a204(const void** str) {
    const u8** pp = (const u8**)str;
    u16 c = **pp;

    if ((c >= 0x81 && c < 0xa0) || c >= 0xe0) {
        c = (u16)((c << 8) | (*pp)[1]);
        *pp += 2;
    } else {
        *pp += 1;
    }
    return c;
}

/* single byte */
u16 func_0206a258(const void** str) {
    const u8* p = (const u8*)*str;
    u8 c = *p++;

    *str = p;
    return c;
}
