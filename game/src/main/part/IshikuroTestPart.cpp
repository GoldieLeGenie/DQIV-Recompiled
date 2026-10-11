#pragma ipa file
#include "main/part/IshikuroTestPart.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/Pad.hpp"
#include "main/menu/MenuManager.hpp"

IshikuroTestPart g_IshikuroTestPart;
static int s_counter;

ARM IshikuroTestPart::IshikuroTestPart()
{
}

ARM void IshikuroTestPart::initialize()
{
}

ARM void IshikuroTestPart::terminate()
{
}

ARM void IshikuroTestPart::onExecutePart()
{
    if (dss::g_Pad.edge() & 1) {
        data_0210bb94.unkfunc_020580fc(TITLE_PART);
    }
    if (dss::g_Pad.edge() & 2) {
        unkfunc_0207f834(&data_0211a60c, 0x400, 0x20);
    }
}

ARM void IshikuroTestPart::onDrawPart()
{
}

ARM void IshikuroTestPart::onWindowPart()
{
}

ARM void IshikuroTestPart::onDebugPart()
{
    data_02116ce0.unkfunc_0207e88c(0, 0, "Ishikuro Test Part");
    data_02116ce0.unkfunc_0207e88c(30, 23, "%02d", s_counter);
    s_counter++;
    if (s_counter == 100) {
        s_counter = 0;
    }
}
