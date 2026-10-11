#include "main/text/TextAPI.hpp"
#include "main/data/FileLoader.hpp"
#include "main/dss/DssUtils.hpp"

THUMB MsgData::MsgData()
{
    m_addr = 0;
    unk_10 = -1;
    unk_18 = -1;
}

THUMB void MsgData::msg_setup(int msg_base_id, int arg1, int size, int count)
{
    m_addr = (char*)unkfunc_0207f77c(&data_0211a60c, size, -4);
    m_size = size;
    m_msg_base_id = msg_base_id;
    if (arg1 == 0) {
        unk_0c = msg_base_id + count - 1;
    } else {
        unk_0c = arg1;
    }
    unk_10 = -1;
    unk_18 = -1;
    unk_14 = count;
}

THUMB void MsgData::unkfunc_020554ec()
{
    unkfunc_0207f840(&data_0211a60c, m_addr);
    m_addr = 0;
    m_size = 0;
    m_msg_base_id = -1;
    unk_0c = -1;
    unk_10 = -1;
    unk_18 = -1;
}

THUMB int MsgData::unkfunc_02055510(unsigned int msg_id)
{
    if (m_msg_base_id <= msg_id && msg_id <= unk_0c) {
        return 1;
    }
    return 0;
}

THUMB int MsgData::unkfunc_02055524(unsigned int msg_id, int lang)
{
    char path[0x80];
    if (unk_18 != lang) {
        unk_10 = -1;
    }
    int block = -1;
    if (msg_id != -1) {
        block = msg_id / unk_14 * unk_14;
    }
    int cur = -1;
    if (unk_10 != -1) {
        cur = (unsigned int)unk_10 / unk_14 * unk_14;
    }
    if (block != cur) {
        const char* dir = 0;
        if (lang == 0) {
            dir = "JA";
        }
        if (lang == 1) {
            dir = "EN";
        }
        if (lang == 2) {
            dir = "FR";
        }
        if (lang == 3) {
            dir = "DE";
        }
        if (lang == 4) {
            dir = "IT";
        }
        if (lang == 5) {
            dir = "ES";
        }
        if (unk_14 == 10000) {
            dss::sprintf(path, "data/MESS/%s/a%07d.mpt", dir, block);
        } else {
            dss::sprintf(path, "data/MESS/%s/b%07d.mpt", dir, block);
        }
        if (dss::g_File.isExist(path) == 0) {
            return 0;
        }
        dss::g_File.unkfunc_0207eac0(path, m_addr, 1);
    }
    unk_10 = msg_id;
    unk_18 = lang;
    return 1;
}

THUMB int MsgMeta::unkfunc_02055604(const unsigned char* src, int msg_id, int lang)
{
    unsigned char* p = 0;
    m_meta[0][0] = 0;
    m_meta[1][0] = 0;
    m_meta[2][0] = 0;
    m_meta[3][0] = 0;
    m_meta[4][0] = 0;
    m_meta[5][0] = 0;
    m_meta[6][0] = 0;
    m_meta[7][0] = 0;
    m_meta[8][0] = 0;
    m_meta[9][0] = 0;
    m_head[0] = 0;
    m_body[0] = 0;
    m_sound[0] = 0;
    while (*src != 0) {
        unsigned char c = *(const unsigned char*)src;
        if (c == '@') {
            if (p != 0) {
                *p = 0;
                p = 0;
            }
        } else if (p == 0) {
            switch (c) {
                case '0':
                    p = m_meta[0];
                    break;
                case '1':
                    p = m_meta[1];
                    break;
                case '2':
                    p = m_meta[2];
                    break;
                case '3':
                    p = m_meta[3];
                    break;
                case '4':
                    p = m_meta[4];
                    break;
                case '5':
                    p = m_meta[5];
                    break;
                case '6':
                    p = m_meta[6];
                    break;
                case '7':
                    p = m_meta[7];
                    break;
                case '8':
                    p = m_meta[8];
                    break;
                case '9':
                    p = m_meta[9];
                    break;
                case 'a':
                    p = m_head;
                    break;
                case 'b':
                    p = m_body;
                    break;
                case 'c':
                    p = m_sound;
                    break;
                case 0:
                    p = 0;
                    break;
                default:
                    return 0;
            }
        } else {
            *p++ = c;
        }
        src++;
    }
    if (lang == 0) {
        m_head[0] = 0;
    }
    return 1;
}

THUMB unsigned char* MsgMeta::unkfunc_02055740(int index)
{
    switch (index) {
        case 0:
            return m_meta[0];
        case 1:
            return m_meta[1];
        case 2:
            return m_meta[2];
        case 3:
            return m_meta[3];
        case 4:
            return m_meta[4];
        case 5:
            return m_meta[5];
        case 6:
            return m_meta[6];
        case 7:
            return m_meta[7];
        case 8:
            return m_meta[8];
        case 9:
            return m_meta[9];
        case 10:
            return m_head;
        case 11:
            return m_body;
        case 12:
            return m_sound;
    }
    return 0;
}

THUMB void MsgFile::unkfunc_020557cc()
{
    m_addr = (unsigned char*)unkfunc_0207f77c(&data_0211a60c, 0x404, -4);
    m_size = 0x400;
    unk_80c = 0;
}

THUMB void MsgFile::unkfunc_020557f8()
{
    unkfunc_0207f840(&data_0211a60c, m_addr);
    m_addr = 0;
    m_size = 0;
}

THUMB int MsgFile::unkfunc_02055810(unsigned int msg_id, int lang)
{
    MsgData* data = unkfunc_0205488c(msg_id);
    if (data == 0) {
        return 0;
    }
    if (data->unkfunc_02055524(msg_id, lang) == 0) {
        return 0;
    }
    if (unkfunc_02088484(data->m_addr, msg_id) != 0) {
        return 1;
    }
    return 0;
}

THUMB int MsgFile::unkfunc_02055848(unsigned int msg_id, int lang)
{
    if (unkfunc_02055810(msg_id, lang) == 0) {
        return 0;
    }
    unsigned char* src = data_02120544;
    int len = data_020c4978;
    if (*src != '@') {
        return 0;
    }
    dss::memcpy(m_addr, src, len);
    m_addr[len] = 0;
    m_addr[len + 1] = 0;
    m_addr[len + 2] = 0xff;
    return 1;
}

THUMB int MsgFile::unkfunc_02055894(unsigned int msg_id, int lang)
{
    m_msg_id = msg_id;
    unk_80c = unkfunc_02055848(msg_id, lang);
    if (unk_80c == 0) {
        return unk_80c;
    }
    unk_80c = m_meta.unkfunc_02055604(m_addr, msg_id, lang);
    return unk_80c;
}

THUMB unsigned char* MsgFile::unkfunc_020558cc(int index)
{
    if (unk_80c != 0) {
        return m_meta.unkfunc_02055740(index);
    }
    if (index == 10) {
        return (unsigned char*)"";
    }
    dss::sprintf_s((char*)m_addr, 0x400, "ERROR:MID <%d>", m_msg_id);
    return m_addr;
}
