#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

void setWorldPosition(int index, dss::Fix32Vector3* position);
dss::Vector2<int>* getScreenPosition(int index);
void updateScreenPositions();

extern "C" {
    int func_0206dfcc(dss::Fix32Vector3* world, int* x, int* y);           /* world pos -> screen pos */
}
