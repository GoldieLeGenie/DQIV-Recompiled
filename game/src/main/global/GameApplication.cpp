#pragma ipa file
#include "main/global/GameApplication.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalGamePart.hpp"
#include "main/global/StageLink.hpp"
#include "main/status/BaseActionMessage.hpp"
#include "main/btl/BattleMapList.hpp"
#include "main/data/FileLoader.hpp"
#include "main/debug/DebugConsole.hpp"
#include "main/dss/DisplayPlugin.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/ArrayErrorLog.hpp"
#include "main/dss/BgText.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/Fog.hpp"
#include "main/dss/UnkLanguage.hpp"
#include "main/dss/UnkMemory.hpp"
#include "main/dss/SystemText.hpp"
#include "main/dss/TextConvert.hpp"
#include "main/dss/VramManager.hpp"
#include "main/encount/Encount.hpp"
#include "main/menu/DebugMenu.hpp"
#include "main/menu/MenuSystem.hpp"
#include "main/part/BattleMenuPart.hpp"
#include "main/part/BattlePart.hpp"
#include "main/part/BookPart.hpp"
#include "main/part/CardCheckPart.hpp"
#include "main/part/CasinoPart.hpp"
#include "main/part/FieldPart.hpp"
#include "main/part/GameEndPart.hpp"
#include "main/part/GameStartPart.hpp"
#include "main/part/IshikuroTestPart.hpp"
#include "main/part/LogoPart.hpp"
#include "main/part/MPExchangePart.hpp"
#include "main/part/MenuPart.hpp"
#include "main/part/MessageDebugPart.hpp"
#include "main/part/TitlePart.hpp"
#include "main/part/TownPart.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/sound/SoundPlayer.hpp"
#include "main/status/GameStatus.hpp"
#include "main/text/TextAPI.hpp"
#include "main/text/TextHook.hpp"
#include "nitro/g2.hpp"
#include "nitro/g3.hpp"
#include "nitro/gx.h"
#include <nitro/os/os_owner.h>

// 8-byte bss item at the start of the TU, only referenced by a function the linker strips (stand-in, not original)
int data_020c4f70[2];
GameApplication g_GameApplication;

AppBuildDate g_appBuildDate;
BattleMapList g_battleMapList;
// month names of vf08 as named strings: their creation order gives the ROM .data order of the original literals (stand-in, not original)
static char s_monthApr[] = "Apr";
static char s_monthMay[] = "May";
static char s_monthNov[] = "Nov";
static char s_monthAug[] = "Aug";
static char s_monthOct[] = "Oct";
static char s_monthDec[] = "Dec";
static char s_monthFeb[] = "Feb";
static char s_monthMar[] = "Mar";
static char s_monthJul[] = "Jul";
static char s_monthJan[] = "Jan";
static char s_monthJun[] = "Jun";
static char s_monthSep[] = "Sep";

inline void setGameLanguage(Language language)
{
    status::g_Game.language = language;
    TextAPI::setLanguage(language);
}

ARM void GameApplication::vf00()
{
#ifndef DQIV_NATIVE
    initHeap(&g_appHeap, 0x18000, 0x1000, 0x280000, 0);
#endif
    dss::g_File.unkfunc_0207ea64(0);
    int region = getGameRegion();
    OSOwnerInfo info;
    func_02079e40(&info);
    if (region == 1) {
        setGameLanguage(Japanese);
    }
    if (region == 2) {
        switch (info.language) {
        case 1:
            setGameLanguage(English);
            break;
        case 2:
            setGameLanguage(French);
            break;
        case 5:
            setGameLanguage(Spanish);
            break;
        default:
            setGameLanguage(English);
            break;
        }
    }
    if (region == 3) {
        switch (info.language) {
#ifdef DQIV_NATIVE
        case 0:
            // The European data still has the Japanese texts and fonts.
            setGameLanguage(Japanese);
            break;
#endif
        case 1:
            setGameLanguage(English);
            break;
        case 2:
            setGameLanguage(French);
            break;
        case 3:
            setGameLanguage(German);
            break;
        case 4:
            setGameLanguage(Italian);
            break;
        case 5:
            setGameLanguage(Spanish);
            break;
        default:
            setGameLanguage(English);
            break;
        }
    }
    vf04();
    bgChar_.setup("data/G2D/SYSTEM/bg.bgc", 0, 0);
    bgChar2_.setup("data/G2D/SYSTEM/bg.bgc", 0, 0);
    bgPltt_.setup("data/G2D/SYSTEM/bg.bgp", 0, 0);
    objChar_.setup("data/G2D/SYSTEM/obj.bgc", 0, 0);
    objPltt_.setup("data/G2D/SYSTEM/obj.bgp", 0, 0);
    initSystemText(bgChar_.getAddr(), bgChar2_.getAddr(), bgPltt_.getAddr(), objChar_.getAddr(), objPltt_.getAddr());
    font12_.setup("data/G2D/FONT/zen12.nftr", 0, 0);
    font12Half_.setup("data/G2D/FONT/han12.nftr", 0, 0);
    font10_.setup("data/G2D/FONT/zen10.nftr", 0, 0);
    font10Half_.setup("data/G2D/FONT/han10.nftr", 0, 0);
    initBgText();
    loadBgFont12(font12_.getAddr(), font12Half_.getAddr());
    loadBgFont10(font10_.getAddr(), font10Half_.getAddr());
    table81_.setup("DATA/G2D/BIN/81_9F.bin", 0, 0);
    tableE0_.setup("DATA/G2D/BIN/E0_EF.bin", 0, 0);
    initSjisTables((unsigned short*)table81_.getAddr(), table81_.getSize(), (unsigned short*)tableE0_.getAddr(), tableE0_.getSize());
    g_DebugConsole.unkfunc_0207e804();
    loadMenuSystemFiles();
    TextAPI::unkfunc_02054690();
    g_text_env.m_text_hook = &gTextHook;
    g_SoundSystem.unkfunc_020865f4((void*)"data/sound/sound_data.sdat", 1);
    g_SoundPlayer.unkfunc_0205c710();
    g_SoundPlayer.unkfunc_0205c78c(0, 0x6e);
    g_SoundPlayer.unkfunc_0205c78c(0, 0x6f);
    vf0c();
    vf08();
    unkfunc_02008768();
    setArrayErrorEnabled(1);
    updateDebugConsole();
}

ARM void GameApplication::vf04()
{
    GX_SetGraphicsMode((GXDisplayMode)1, (GXBGMode)0, (GX2D3D)1);
    GX_SetBankForTex((GXVRam)3);
    GX_SetBankForTexPltt((GXVRam)0x40);
    GX_SetBankForBg((GXVRam)0x10);
    GX_SetBankForObj((GXVRam)0x20);
    GX_SetBGScrOffset(0);
    GX_SetBGCharOffset(0);
    G2_SetBG1Control(0, 0, 0xe, 0, 0);
    G2_SetBG2ControlText(0, 0, 0xf, 0);
    G2_SetBG3ControlText(0, 0, 7, 0);
    GXS_SetGraphicsMode(0);
    GX_SetBankForSubBg((GXVRam)0x80);
    GX_SetBankForSubObj((GXVRam)0x100);
    G2S_SetBG0Control(0, 0, 7, 0, 0);
    G2S_SetBG1Control(0, 0, 7, 0, 0);
    G2S_SetBG2ControlText(0, 0, 7, 0);
    G2S_SetBG3ControlText(0, 0, 7, 0);
    *VIDEO_REG(vu16,0x04000060) = (u16)((*VIDEO_REG(vu16,0x04000060) & ~0x3000) | 8);
    func_0206541c(0, 31, 0x7fff, 63, 1);
    initFog();
    GX_DispOn();
    GXS_DispOn();
    GX_SetDispSelect(0);
    g_VramTransfer.initTransferQueue();
    GX_SetDispSelect(0);
}

ARM void GameApplication::vf08()
{
    func_0202d014();
    char text[0x21];
    const char* month[12] = { s_monthJan, s_monthFeb, s_monthMar, s_monthApr, s_monthMay, s_monthJun, s_monthJul, s_monthAug, s_monthSep, s_monthOct, s_monthNov, s_monthDec };
    for (int i = 0; i < 12; i++) {
        if (g_appBuildDate.date_[0] == month[i][0] && g_appBuildDate.date_[1] == month[i][1] && g_appBuildDate.date_[2] == month[i][2]) {
            int m = i;
            m++;
            g_appBuildDate.dateString_[0] = g_appBuildDate.date_[9];
            g_appBuildDate.dateString_[1] = g_appBuildDate.date_[10];
            g_appBuildDate.dateString_[2] = '/';
            g_appBuildDate.dateString_[3] = '0' + m / 10;
            g_appBuildDate.dateString_[4] = '0' + m % 10;
            g_appBuildDate.dateString_[5] = '/';
            g_appBuildDate.dateString_[6] = g_appBuildDate.date_[4] == ' ' ? '0' : g_appBuildDate.date_[4];
            g_appBuildDate.dateString_[7] = g_appBuildDate.date_[5];
            g_appBuildDate.dateString_[8] = 0;
        }
    }
    g_appBuildDate.time_[5] = 0;
    dss::sprintf_s(text, sizeof(text), "%s %s", g_appBuildDate.dateString_, g_appBuildDate.time_);
    DebugMenu::setBuildDate(text);
}

ARM void GameApplication::vf0c()
{
    data_020c1b7c = 0;
    g_GlobalDQ4.unkfunc_020580bc(TITLE_PART, &g_TitlePart);
    g_GlobalDQ4.unkfunc_020580bc(MENU_PART, &g_MenuPart);
    g_GlobalDQ4.unkfunc_020580bc(LOGO_PART, &g_LogoPart);
    g_GlobalDQ4.unkfunc_020580bc(GAME_START_PART, &g_GameStartPart);
    g_GlobalDQ4.unkfunc_020580bc(CARDCHECK_PART, &g_CardCheckPart);
    g_GlobalDQ4.unkfunc_020580bc(TOWN_PART, &g_TownPart);
    g_GlobalDQ4.unkfunc_020580bc(BATTLE_PART, &g_BattlePart);
    g_GlobalDQ4.unkfunc_020580bc(FIELD_PART, &g_FieldPart);
    g_GlobalDQ4.unkfunc_020580bc(CASINO_PART, &g_CasinoPart);
    g_GlobalDQ4.unkfunc_020580bc(BOOK_PART, &g_BookPart);
    g_GlobalDQ4.unkfunc_020580bc(BATTLE_MENU_PART, &g_BattleMenuPart);
    g_GlobalDQ4.unkfunc_020580bc(MPEXCHANGE_PART, &g_MPExchangePart);
    g_GlobalDQ4.unkfunc_020580bc(MESSAGEDEBUG_PART, &g_MessageDebugPart);
    g_GlobalDQ4.unkfunc_020580bc(GAME_END_PART, &g_GameEndPart);
    g_GlobalDQ4.unkfunc_020580bc(ISHIKUROTEST_PART, &g_IshikuroTestPart);
    GlobalDQ4::unkfunc_02058128(unkfunc_020087b4);
    GlobalDQ4::unkfunc_02058138(unkfunc_020087cc);
    setPartFrameCallback(unkfunc_020087e0);
    g_GlobalDQ4.unk_74 = 0;
}

ARM void GameApplication::unkfunc_02008768()
{
    g_battleMapList.load("data/btlmap/_list.txt");
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_SINGLE3D);
    g_Global.initialize();
    SoundManager::initialize();
    StageLink::initialize();
    encount::Encount::getSingleton()->enable_ = 1;
}

ARM void GameApplication::vf14()
{
}

ARM void GameApplication::unkfunc_020087b4()
{
    status::g_Game.addPlayTime(1);
}

ARM void GameApplication::unkfunc_020087cc()
{
    g_Global.partChangeFlag_ = 0;
}

ARM void GameApplication::unkfunc_020087e0()
{
    SoundManager::update();
}

ARM int GameApplication::vf10()
{
    return CARDCHECK_PART;
}

ARM GameApplication::~GameApplication()
{
}

ARM void DssApplication::vf14()
{
}

ARM int DssApplication::vf10()
{
    return 0;
}

ARM void DssApplication::vf0c()
{
}

ARM void DssApplication::vf08()
{
}

ARM void DssApplication::vf04()
{
}

ARM void DssApplication::vf00()
{
}

// stand-in for a stripped function using data_020c4f70 (keeps the bss layout), not original
ARM void unkfunc_unused_14()
{
    data_020c4f70[0] = 0;
}
