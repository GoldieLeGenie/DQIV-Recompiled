#pragma ipa file
#include "main/cmn/WorldLocation.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"


ARM cmn::WorldLocation::WorldLocation()
{
}

ARM cmn::WorldLocation::~WorldLocation()
{
}

ARM cmn::WorldLocation* cmn::WorldLocation::getSingleton()
{
    static WorldLocation WorldLocation;
    return &WorldLocation;
}

ARM void cmn::WorldLocation::initialize()
{
  this->pCLUTCode_ = status::excelParam.clutCode_;
  return;
}

ARM void cmn::WorldLocation::terminate()

{
  return;
}

ARM void cmn::WorldLocation::calcWorldPos(fx32* x, fx32* y)
{
    fx32 vx = *x;
    if (vx < 0) {
        *x = vx + 0x1000000;
    } else if (vx >= 0x1000000) {
        *x = vx - 0x1000000;
    }

    fx32 vy = *y;
    if (vy < 0) {
        *y = vy + 0x1000000;
    } else if (vy >= 0x1000000) {
        *y = vy - 0x1000000;
    }
}

ARM dss::Fix32Vector3 cmn::WorldLocation::calcPaletteRate(int prev, int next, dss::Fix32 ratio)
{
    dss::Fix32Vector3 c0;
    dss::Fix32Vector3 c1;
    dss::Fix32Vector3 ret;
    dss::Fix32 one;
    one.value = 0x1000;

    param::CLUTCode* t0 = pCLUTCode_;
    param::CLUTCode* e0 = &t0[prev];
    c0.vx.value = t0[prev].rPoint;
    c0.vy.value = e0->gPoint;
    c0.vz.value = e0->bPoint;

    param::CLUTCode* t1 = pCLUTCode_;
    param::CLUTCode* e1 = &t1[next];
    c1.vx.value = t1[next].rPoint;
    c1.vy.value = e1->gPoint;
    c1.vz.value = e1->bPoint;

    ret = c0 * (one - ratio) + c1 * ratio;
    return ret;
}

ARM dss::Fix32Vector3 cmn::WorldLocation::calcPaletteRate(int time)
{
    int next;
    int prev;
    dss::Fix32 rate;

    if (time < 0x80) {
        rate.value = time * 0x1000 / 0x80;
        prev = 0xD;
        next = 0;
    } else if (time < 0x380) {
        rate.value = 0x1000;
        prev = 0;
        next = 0;
    } else if (time < 0x400) {
        rate.value = (time - 0x380) * 0x1000 / 0x80;
        prev = 0;
        next = 0xA;
    } else if (time < 0x740) {
        rate.value = 0x1000;
        prev = 0;
        next = 0xA;
    } else if (time < 0x7C0) {
        rate.value = (time - 0x740) * 0x1000 / 0x80;
        prev = 0xA;
        next = 0xB;
    } else if (time < 0x840) {
        rate.value = 0x1000;
        prev = 0;
        next = 0xB;
    } else if (time < 0x8C0) {
        rate.value = (time - 0x840) * 0x1000 / 0x80;
        prev = 0xB;
        next = 0xC;
    } else if (time < 0xC40) {
        rate.value = 0x1000;
        prev = 0;
        next = 0xC;
    } else if (time < 0xCC0) {
        rate.value = (time - 0xC40) * 0x1000 / 0x80;
        prev = 0xC;
        next = 0xD;
    } else if (time < 0xD40) {
        rate.value = 0x1000;
        prev = 0;
        next = 0xD;
    }

    return calcPaletteRate(prev, next, rate);
}

ARM dss::Fix32Vector3 cmn::WorldLocation::calcYamiPaletteRate()
{
    return calcPaletteRate(0, 0xE, dss::Fix32(0x1000));
}

ARM void cmn::WorldLocation::setCurrentTimeZone()
{
    int time = g_Stage.getWorldTime();

    if (time < 0x80) {
        g_Stage.timeZone_ = TIME_ZONE_MORNING;
    } else if (time < 0x7C0) {
        g_Stage.timeZone_ = TIME_ZONE_DAYTIME;
    } else if (time < 0x8C0) {
        g_Stage.timeZone_ = TIME_ZONE_EVENING;
    } else if (time < 0xCC0) {
        g_Stage.timeZone_ = TIME_ZONE_NIGHT;
    } else if (time < 0xD40) {
        g_Stage.timeZone_ = TIME_ZONE_MORNING;
    }

    g_Stage.setWorldTime(time);
}

ARM TIME_ZONE cmn::WorldLocation::getCurrentTimeZone()
{
    int time = g_Stage.getWorldTime();

    if (time < 0x80) {
        return TIME_ZONE_MORNING;
    }
    if (time < 0x7C0) {
        return TIME_ZONE_DAYTIME;
    }
    if (time < 0x8C0) {
        return TIME_ZONE_EVENING;
    }
    if (time < 0xCC0) {
        return TIME_ZONE_NIGHT;
    }
    return TIME_ZONE_MORNING;
}