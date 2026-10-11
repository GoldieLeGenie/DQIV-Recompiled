#ifndef FX_TRIG_H_
#define FX_TRIG_H_

#include <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const s16 FX_SinCosTable_[];

#ifdef __cplusplus
}
#endif

static inline s16 FX_SinIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2];
}

static inline s16 FX_CosIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2 + 1];
}

#endif // FX_TRIG_H_
