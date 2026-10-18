#include "main/dss/ScreenPosition.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/Camera.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#endif

static dss::Fix32Vector3 s_worldPosition[64];
static dss::Vector2<int> s_screenPosition[64][2];

ARM void setWorldPosition(int index, dss::Fix32Vector3* position)
{
    s_worldPosition[index] = *position;
}

ARM dss::Vector2<int>* getScreenPosition(int index)
{
    int buf = getDisplayFrame() & 1;
    return &s_screenPosition[index][buf];
}

ARM void updateScreenPositions()
{
    int buf = (getDisplayFrame() & 1) ? 0 : 1;
    for (int i = 0; i < 64; i++) {
        if (func_0206dfcc(&s_worldPosition[i], &s_screenPosition[i][buf].vx, &s_screenPosition[i][buf].vy) == -1) {
#ifdef DQIV_NATIVE
            // Large battles: places in the band around the screen stay (the
            // monsters show there, and their name plates).
            const dss::Vector2<int>& at = s_screenPosition[i][buf];
            if (pc::battlePlateSide() > 0 && at.vx >= -pc::battleBandSide && at.vx < 256 + pc::battleBandSide &&
                at.vy >= -pc::battleBandRows && at.vy < 192) {
                continue;
            }
#endif
            s_screenPosition[i][buf].vx = -1;
            s_screenPosition[i][buf].vy = -1;
        }
    }
}
