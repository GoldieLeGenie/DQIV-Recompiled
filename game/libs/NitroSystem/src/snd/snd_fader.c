// Linear fader.
#include "snd_internal.h"

typedef struct UnkSndFader {
    /* 0x00 */ s32 origin;
    /* 0x04 */ s32 target;
    /* 0x08 */ s32 counter;
    /* 0x0C */ s32 frames;
} UnkSndFader;

s32 func_02077284(UnkSndFader* fader);

void func_02077244(UnkSndFader* fader) {
    fader->target = 0;
    fader->origin = 0;
    fader->frames = 0;
    fader->counter = 0;
}

void func_0207725c(UnkSndFader* fader, s32 target, s32 frames) {
    fader->origin = func_02077284(fader);
    fader->target = target;
    fader->frames = frames;
    fader->counter = 0;
}

s32 func_02077284(UnkSndFader* fader) {
    if (fader->counter >= fader->frames) {
        return fader->target;
    }
    return fader->origin + (fader->target - fader->origin) * fader->counter / fader->frames;
}

void func_020772b8(UnkSndFader* fader) {
    if (fader->counter < fader->frames) {
        fader->counter++;
    }
}

BOOL func_020772d0(UnkSndFader* fader) {
    return fader->counter >= fader->frames;
}
