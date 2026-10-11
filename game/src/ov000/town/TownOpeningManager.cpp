#pragma ipa file
#include "ov000/town/TownOpeningManager.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/UnkLanguage.hpp"
#include "main/dss/Random.hpp"
#include "main/global/Global.hpp"
#include "main/sound/Sound.hpp"
#include "main/status/GameStatus.hpp"
#include <string.h>

ARM TownOpeningManager::TownOpeningManager()
{
    m_enable = 0;
}

ARM TownOpeningManager::~TownOpeningManager()
{
}

ARM TownOpeningManager* TownOpeningManager::getSingleton()
{
    static TownOpeningManager m_singleton;
    return &m_singleton;
}

ARM void TownOpeningManager::setup()
{
    char filename[0x80];
    func_0206ae58(0, 0x8b7, -0xc76, -0x4f7);
    func_0206ae94(0, 0x7bff);
    dss::Fix32Vector3 pos;
    char area[8];
    char lang[8];
    if (unkfunc_0208a104() == 2) {
        dss::strcpy(area, "na");
        switch (status::g_Game.language) {
            case French:
                strcpy(lang, "fr");
                break;
            case Spanish:
                strcpy(lang, "es");
                break;
            default:
                strcpy(lang, "en");
                break;
        }
    } else {
        dss::strcpy(area, "eu");
        switch (status::g_Game.language) {
            case French:
                strcpy(lang, "fr");
                break;
            case German:
                strcpy(lang, "de");
                break;
            case Italian:
                strcpy(lang, "it");
                break;
            case Spanish:
                strcpy(lang, "es");
                break;
            default:
                strcpy(lang, "en");
                break;
        }
    }
    dss::sprintf_s(filename, 0x80, "data/opening/%s/%s/logo.tex.lz", area, lang);
    m_tex_data.setup(filename, 0, 1);
    dss::sprintf_s(filename, 0x80, "data/opening/staff.tex.lz");
    m_staff_tex_data.setup(filename, 0, 1);
    void* tex = m_tex_data.getAddr();
    ((TextureObject*)tex)->unkfunc_02086798(1);
    dss::memcpy(&m_tex, tex, sizeof(TextureObject));
    m_tex_data.cleanup();
    tex = m_staff_tex_data.getAddr();
    ((TextureObject*)tex)->unkfunc_02086798(1);
    dss::memcpy(&m_staff_tex, tex, sizeof(TextureObject));
    m_staff_tex_data.cleanup();
    dss::sprintf_s(filename, 0x80, "data/opening/%s/%s/logo1.dssa", area, lang);
    m_dssa_data[0].setup(filename, 0, 0);
    dss::sprintf_s(filename, 0x80, "data/opening/%s/%s/logo2.dssa", area, lang);
    m_dssa_data[1].setup(filename, 0, 0);
    dss::sprintf_s(filename, 0x80, "data/opening/%s/%s/logo3.dssa", area, lang);
    m_dssa_data[2].setup(filename, 0, 0);
    dss::sprintf_s(filename, 0x80, "data/opening/%s/%s/logo4.dssa", area, lang);
    m_dssa_data[3].setup(filename, 0, 0);
    dss::sprintf_s(filename, 0x80, "data/opening/staff.dssa", area, lang);
    m_staff_dssa_data.setup(filename, 0, 0);
    m_dssa[0].setup(m_dssa_data[0].getAddr());
    m_dssa[0].setTexture(&m_tex);
    m_dssa[1].setup(m_dssa_data[1].getAddr());
    m_dssa[1].setTexture(&m_tex);
    m_dssa[2].setup(m_dssa_data[2].getAddr());
    m_dssa[2].setTexture(&m_tex);
    m_dssa[3].setup(m_dssa_data[3].getAddr());
    m_dssa[3].setTexture(&m_tex);
    m_dssa_camera.setup(m_dssa_data[3].getAddr());
    m_dssa_camera.setTexture(&m_tex);
    m_dssa_camera.type_ = DSSAObjectWithCamera::Near;
    m_staff_dssa.setup(m_staff_dssa_data.getAddr());
    m_staff_dssa.setTexture(&m_staff_tex);
    pos.vx.value = 0x400;
    pos.vy.value = -0x800;
    pos.vz.value = 0x1000;
    m_dssa_camera.setPosition(pos);
    pos.vx.value = 0;
    pos.vy.value = 0x10000;
    pos.vz.value = 0;
    m_staff_dssa.setPosition(pos);
    pos.vx.value = 0;
    pos.vy.value = 0x20000;
    pos.vz.value = 0;
    m_dssa[0].setPosition(pos);
    m_dssa[1].setPosition(pos);
    m_dssa[2].setPosition(pos);
    m_dssa[3].setPosition(pos);
    m_effect = 0;
    m_enable = 1;
    m_effectCounter = 0;
    m_counter = -45;
    unk_50c = 0;
}

ARM void TownOpeningManager::cleanup()
{
    func_0206ae58(0, 0x93d, -0x93d, -0x93d);
    func_0206ae94(0, 0x7fff);
    if (!m_enable) {
        return;
    }
    m_staff_dssa.cleanup();
    m_dssa[0].cleanup();
    m_dssa[1].cleanup();
    m_dssa[2].cleanup();
    m_dssa[3].cleanup();
    m_dssa_camera.cleanup();
    m_staff_dssa_data.cleanup();
    m_dssa_data[0].cleanup();
    m_dssa_data[1].cleanup();
    m_dssa_data[2].cleanup();
    m_dssa_data[3].cleanup();
    m_tex.unkfunc_02086868();
    m_staff_tex.unkfunc_02086868();
    m_enable = 0;
}

ARM void TownOpeningManager::draw()
{
    if (!m_enable) {
        return;
    }
    if (!(unkfunc_02081254() & 1)) {
        return;
    }
    if (m_counter == -45) {
        Sound::unkfunc_02055980(0);
        dss::Vector3<short> rot;
        rot.vx = 0;
        rot.vy = -0x7880;
        rot.vz = 0;
        TownCharacterManager::getSingleton()->setRotate(0, rot);
    }
    if (m_counter >= 0) {
        if (m_counter < 0x21c) {
            m_staff_dssa.draw();
            m_staff_dssa.execute();
        } else if (m_counter >= 0x249) {
            if (m_counter < 0x294) {
                m_dssa[0].draw();
                m_dssa[0].execute();
            } else if (m_counter < 0x990) {
                if (m_effect) {
                    m_dssa[1].draw();
                    m_dssa[1].execute();
                    DSSAObject::calcType_ = 0;
                    m_dssa_camera.draw();
                    m_dssa_camera.execute();
                    DSSAObject::calcType_ = 1;
                    if (++m_effectCounter == 0x12) {
                        m_effect = 0;
                        m_effectCounter = 0;
                    }
                } else {
                    if (m_counter == 0x2c4) {
                        m_effect = 1;
                    }
                    if (m_counter < 0x97e && dssrand::rand(0x200) == 0) {
                        m_effect = 1;
                    }
                    m_dssa[1].draw();
                    m_dssa[1].execute();
                }
            } else if (m_counter < 0x9b7) {
                m_dssa[2].draw();
                m_dssa[2].execute();
            }
        }
    }
    if (m_counter == 0xcde) {
        g_Global.fadeOutBlack(0x78);
    }
    if (m_counter == 0xd56) {
        g_Global.startLogo();
    }
    dss::Fix32Vector3 rate = cmn::CommonEffectLocation::getSingleton()->getPaletteRate();
    TownCharacterManager::getSingleton()->setPaletteRate(0, rate.vx, rate.vy, rate.vz);
    m_counter++;
}

ARM void TownOpeningManager::execute()
{
    if (!m_enable) {
        return;
    }
}
