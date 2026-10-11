#pragma once
#include "main/dss/DssUtils.hpp"

// Default constructors of the generic dss vectors. Included at the END of a .cpp:
// the original instantiates them after the TU's functions/vtables, which fixes the
// order of their literal temps in .data (CasinoPokerDraw, PokerCard).
namespace dss {
    template <typename T>
    inline Vector3<T>::Vector3() : vz(0L), vy(0L), vx(0L) {}
    template <typename T>
    inline Vector2<T>::Vector2() : vy(0L), vx(0L) {}
}
