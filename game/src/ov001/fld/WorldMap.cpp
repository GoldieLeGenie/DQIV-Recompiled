#include "ov001/fld/WorldMap.hpp"
#include "nitro/os.hpp"

ARM CWorldMap::CWorldMap()
{
}

ARM CWorldMap::~CWorldMap()
{
}

ARM void CWorldMap::setup(void* data)
{
    chunk* cnk = (chunk*)((header*)data + 1);

    while (cnk->id != 0) {
        switch (cnk->id) {
        case 0x4e595357: // "WSYN"
            m_size = (unsigned char*)(cnk + 1);
            break;
        case 0x41484357: // "WCHA"
            m_change_info = (CNK_CHANGE_INFO*)(cnk + 1);
            m_change_num = cnk->size / sizeof(CNK_CHANGE_INFO);
            break;
        case 0x50414d57: // "WMAP"
            m_map = (short*)(cnk + 1);
            break;
        case 0x414d4357: // "WCMA"
            m_change_map = (short*)(cnk + 1);
            break;
        case 0x58455457: // "WTEX"
            m_tex = (CNK_TEX*)(cnk + 1);
            m_tex_num = cnk->size / sizeof(CNK_TEX);
            break;
        case 0x54414453: // "SDAT"
            m_symbol_chunk = cnk;
            break;
        default:
            OS_Terminate();
            break;
        }
        cnk = (chunk*)((unsigned char*)cnk + cnk->size + sizeof(chunk));
    }
}

ARM int CWorldMap::getAttr(int x, int y)
{
    int i = m_map[y * (unsigned short)(m_size[2] * m_size[0]) + x];
    i = (unsigned short)((unsigned short)(i - 1) & 0x3ff);
    return m_tex[i].attr;
}

ARM int CWorldMap::getWorldNo(int x, int y)
{
    int i = m_map[y * (unsigned short)(m_size[2] * m_size[0]) + x];
    return (unsigned short)((unsigned short)(i - 1) & 0x3ff);
}

ARM void CWorldMap::worldChange(int no)
{
    if (no < m_change_num) {
        CNK_CHANGE_INFO* info = &m_change_info[no];
        int y;
        int x;
        unsigned short width = m_size[2] * m_size[0];
        int offset = info->offset;
        for (y = info->sy; y < info->ey; y++) {
            int n = y * width;
            for (x = info->sx; x < info->ex; x++) {
                m_map[n + x] = m_change_map[offset];
                offset++;
            }
        }
    } else {
        OS_Terminate();
    }
}

ARM void CWorldSymbol::setup_symbol(void* data)
{
    CWorldMap::chunk* cnk = (CWorldMap::chunk*)data;

    while (cnk->id != 0) {
        switch (cnk->id) {
        case 0x54414453: // "SDAT"
            m_symbol_data = (WorldSymbolData*)(cnk + 1);
            m_symbol_num = cnk->size / sizeof(WorldSymbolData);
            break;
        default:
            OS_Terminate();
            break;
        }
        cnk = (CWorldMap::chunk*)((unsigned char*)cnk + cnk->size + sizeof(CWorldMap::chunk));
    }
}
