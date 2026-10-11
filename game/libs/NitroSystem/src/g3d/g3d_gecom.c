#include <nitro/native_g3.h>
#include "g3d_internal.h"

UnkGeState data_0210fe90;

// Sets the geometry command buffer (only when none is set)
void func_0206dcb0(NNSG3dGeBuffer* buf) {
    if (data_0210fe90.buf == NULL) {
        buf->idx = 0;
        data_0210fe90.buf = buf;
    }
}

// Flushes and releases the geometry command buffer
NNSG3dGeBuffer* func_0206dcd0(void) {
    NNSG3dGeBuffer* buf;
    func_0206dcf0();
    buf = data_0210fe90.buf;
    data_0210fe90.buf = NULL;
    return buf;
}

// Sends the buffered geometry commands to the FIFO
void func_0206dcf0(void) {
    if (data_0210fe90.busy) {
        func_0206dd4c();
    }
    if (data_0210fe90.buf && data_0210fe90.buf->idx) {
        G3_SEND(&data_0210fe90.buf->data[0], data_0210fe90.buf->idx * 4);
        data_0210fe90.buf->idx = 0;
    }
}

// Waits for the end of a display list transfer
void func_0206dd4c(void) {
    while (data_0210fe90.busy) {
    }
}

// Display list transfer end callback
void func_0206dd64(void* arg) {
    *(u32*)arg = 0;
}

void func_0206dd70(u32 flag) {
    data_0210fe90.useDmaAsync = flag;
}

// Sends a display list
void func_0206dd80(const u32* dl, u32 size) {
    if (size < 0x100 || data_020c3dbc == (u32)-1) {
        func_0206de34(*dl, dl + 1, (size >> 2) - 1);
    } else {
        func_0206dcf0();
        data_0210fe90.busy = 1;
        if (data_0210fe90.useDmaAsync) {
            func_02067744(data_020c3dbc, dl, size, func_0206dd64, (void*)&data_0210fe90.busy);
        } else {
            func_02067540(data_020c3dbc, dl, size, func_0206dd64, (void*)&data_0210fe90.busy);
        }
    }
}

// Sends (or buffers) a geometry command with its parameters
void func_0206de34(u32 op, const void* args, u32 num) {
    if (data_0210fe90.buf) {
        if (data_0210fe90.busy) {
            u32 idx = data_0210fe90.buf->idx;
            if (idx + 1 + num <= 0xc0) {
                data_0210fe90.buf->idx = idx + 1;
                data_0210fe90.buf->data[idx] = op;
                if (num) {
                    MI_CpuCopy(args, &data_0210fe90.buf->data[data_0210fe90.buf->idx], num * 4);
                    data_0210fe90.buf->idx += num;
                }
                return;
            }
        }
        if (data_0210fe90.buf->idx) {
            func_0206dcf0();
        } else if (data_0210fe90.busy) {
            func_0206dd4c();
        }
    } else if (data_0210fe90.busy) {
        func_0206dd4c();
    }
    G3_WRITE(0x0, op);
    G3_SEND(args, num * 4);
}

// Gets the current position/vector matrices
void func_0206df18(MtxFx43* m, MtxFx33* n) {
    MtxFx44 tmp;

    func_0206dcf0();
    G3_WRITE(0x10, 0);
    G3_WRITE(0x11, 0);
    G3_WRITE(0x15, 0);

    if (m) {
        while (func_020653a8(&tmp)) {
        }
        func_0206276c(&tmp, m);
    }
    if (n) {
        while (func_020653d8(n)) {
        }
    }

    G3_WRITE(0x12, 1);
    G3_WRITE(0x10, 2);
}
