// Keep the complete SDK sound state, including its reserved work buffer.
#pragma define_section sdk_state ".data" ".bss.keep" ".rodata" RW
#include "../nns_internal.h"

typedef void (*UnkSleepCallbackFunc)(void* arg);

typedef struct UnkSleepCallbackInfo {
    UnkSleepCallbackFunc func;              /* 0x00 */
    void* arg;                              /* 0x04 */
    struct UnkSleepCallbackInfo* next;      /* 0x08 */
} UnkSleepCallbackInfo;

typedef struct UnkSndMainUnk {
    u8 unk_00[0x23d8];
} UnkSndMainUnk;

/* definition order chosen for the .bss layout (the compiler sorts the objects by size) */
NITRO_SDK_STATE s8 data_0210fee0;
NITRO_SDK_STATE u32 data_0210fee4;
NITRO_SDK_STATE UnkSleepCallbackInfo data_0210fefc;         /* post-sleep callback */
NITRO_SDK_STATE u32 data_0210fee8;
NITRO_SDK_STATE BOOL data_0210feec;                         /* initialized */
NITRO_SDK_STATE UnkSleepCallbackInfo data_0210fef0;         /* pre-sleep callback */
NITRO_SDK_STATE UnkSndMainUnk data_0210ff08;                /* not referenced by the code in the ROM */

void SND_Init(void);
void func_0207c1dc(UnkSleepCallbackInfo* info);
void func_0207c1f4(UnkSleepCallbackInfo* info);
void func_02073354(void);
void func_0207425c(void);
void func_02073598(void);
BOOL func_0207a818(u32 flag);
void func_0207364c(void);
void func_02074274(void);
void func_02075d68(void);
BOOL func_0207a9e8(u32 flag);
void func_020743ec(void);
void func_0207a478(u32 a, u32 b, u32 c, u32 d);
u32 func_0207ac10(void);
void func_0207aba4(u32 tag);
void func_0207444c(void);

void func_02073214(void* arg);
void func_0207324c(void* arg);

void func_0207315c(void)
{
    if (data_0210feec) {
        return;
    }
    data_0210feec = TRUE;

    SND_Init();

    data_0210fef0.func = func_02073214;
    data_0210fef0.arg = NULL;
    data_0210fefc.func = func_0207324c;
    data_0210fefc.arg = NULL;
    func_0207c1dc(&data_0210fef0);
    func_0207c1f4(&data_0210fefc);

    func_02073354();
    func_0207425c();
    func_02073598();

    data_0210fee0 = -1;
    data_0210fee4 = 1;
}

void func_020731e4(void)
{
    while (func_0207a818(0)) {
    }

    func_0207364c();
    func_02074274();
    func_02075d68();

    func_0207a9e8(0);
}

/* pre-sleep callback */
void func_02073214(void* arg)
{
    u32 tag;

    func_020743ec();
    func_0207a478(0, 0, 0, 0);
    tag = func_0207ac10();
    func_0207a9e8(1);
    func_0207aba4(tag);
}

/* post-sleep callback */
void func_0207324c(void* arg)
{
    func_0207444c();
}

/* Uses data_0210fee8 like the library code that is not linked into the ROM (keeps it in the pooled .bss). */
static void unkfunc_unused_24(void)
{
    data_0210fee8 = 0;
}
