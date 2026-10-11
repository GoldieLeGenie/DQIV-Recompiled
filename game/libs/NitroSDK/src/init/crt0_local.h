#ifndef CRT0_LOCAL_H
#define CRT0_LOCAL_H

// Module parameter block at 0x02000b64 (0x3c bytes, in the startup code area)
typedef struct UnkBuildInfo {
    /* 0x00 */ void*         autoload_list;         // autoload block list {dest, size, bss size}
    /* 0x04 */ void*         autoload_list_end;
    /* 0x08 */ void*         autoload_start;        // data of the first autoload block
    /* 0x0c */ void*         bss_start;
    /* 0x10 */ void*         bss_end;
    /* 0x14 */ void*         compressed_static_end; // 0 = static module not compressed
    /* 0x18 */ unsigned long sdk_version;
    /* 0x1c */ unsigned long magic_be;
    /* 0x20 */ unsigned long magic_le;
    /* 0x24 */ char          backup_id[24];         // backup device id string
} UnkBuildInfo;

extern UnkBuildInfo BuildInfo;

// Autoload block list: written by the ROM builder after the static module (inside the .bss address range)
extern unsigned char data_020c4d80[];
extern unsigned char data_020c4d98[];

// Static .bss bounds (linker script symbols)
extern unsigned char ARM9_BSS_START[];
extern unsigned char ARM9_BSS_END[];

// DTCM base (0x027e0000) and DTCM base + 0x21 (protection region 2 setting), linker-provided addresses
extern unsigned char data_027e0000[];

void func_0200641c(void);
void func_02007eec(void);
void __call_static_initializers(void);
void func_01ff8138(void);
int  main(void);

void Entry(void);
void func_0200093c(unsigned long value, void* dst, unsigned long size);
void MIi_UncompressBackward(void* bottom);
void do_autoload(void);
void AutoloadCallback(void);
void init_cp15(void);
void func_02000b60(void);

#endif
