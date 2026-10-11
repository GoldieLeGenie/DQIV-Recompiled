#include <nitro/types.h>

/* Bit stream reader used to unpack glyph images. */

typedef struct UnkBitReader {
    u8* unk_00;                 // 0x00 source
    s8 unk_04;                  // 0x04 bits left in unk_05
    u8 unk_05;                  // 0x05 current byte
} UnkBitReader;

/* Reads the next nBits (<= 8) bits, most significant first. */
u32 func_0206a114(UnkBitReader* reader, int nBits) {
    int avail = reader->unk_04;
    u32 bits = reader->unk_05;
    u32 val;

    if (avail < nBits) {
        int rest = nBits - avail;

        reader->unk_05 = *reader->unk_00++;
        reader->unk_04 = 8;
        val = func_0206a114(reader, rest) | (bits << rest);
    } else {
        int rest = avail - nBits;

        val = bits >> rest;
        reader->unk_04 = rest;
    }
    return val & (0xff >> (8 - nBits));
}
