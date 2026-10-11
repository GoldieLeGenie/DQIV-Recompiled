#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

void unkfunc_0205710c(int index, dss::Fix32Vector3* position);
dss::Vector2<int>* unkfunc_02057128(int index);
void unkfunc_0205714c();

extern "C" {
    int func_0206dfcc(dss::Fix32Vector3* world, int* x, int* y);           /* world pos -> screen pos */
}
