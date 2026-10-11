#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

extern const dss::Fix32 data_020c40ec;                // pi / 2
extern const dss::Fix32 data_020c40e8;                // 360
extern const dss::Fix32 data_020c40e4;                // 180
extern const dss::Fix32 data_020c40e0;                // 90
extern const dss::Fix32 data_020c40dc;                // 16
extern const dss::Fix32 data_020c40d8;                // 8
extern const dss::Fix32 data_020c40f0;                // pi

int unkfunc_02080d80(int r, int g, int b, int a);  // RGBA8
int unkfunc_02080d94(dss::Fix32 degree);           // degree -> 16-bit angle
