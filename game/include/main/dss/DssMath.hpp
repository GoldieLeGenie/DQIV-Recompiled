#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

extern const dss::Fix32 g_fix32HalfPi;                // pi / 2
extern const dss::Fix32 g_fix32Deg360;                // 360
extern const dss::Fix32 g_fix32Deg180;                // 180
extern const dss::Fix32 g_fix32Deg90;                // 90
extern const dss::Fix32 g_fix32Sixteen;                // 16
extern const dss::Fix32 g_fix32Eight;                // 8
extern const dss::Fix32 g_fix32Pi;                // pi

int packRgba(int r, int g, int b, int a);  // RGBA8
int degreeToAngle(dss::Fix32 degree);           // degree -> 16-bit angle
