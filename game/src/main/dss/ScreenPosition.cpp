#include "main/dss/ScreenPosition.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/dss/Camera.hpp"

static dss::Fix32Vector3 s_worldPosition[64];
static dss::Vector2<int> s_screenPosition[64][2];

ARM void unkfunc_0205710c(int index, dss::Fix32Vector3* position)
{
    s_worldPosition[index] = *position;
}

ARM dss::Vector2<int>* unkfunc_02057128(int index)
{
    int buf = unkfunc_02081254() & 1;
    return &s_screenPosition[index][buf];
}

ARM void unkfunc_0205714c()
{
    int buf = (unkfunc_02081254() & 1) ? 0 : 1;
    for (int i = 0; i < 64; i++) {
        if (func_0206dfcc(&s_worldPosition[i], &s_screenPosition[i][buf].vx, &s_screenPosition[i][buf].vy) == -1) {
            s_screenPosition[i][buf].vx = -1;
            s_screenPosition[i][buf].vy = -1;
        }
    }
}
