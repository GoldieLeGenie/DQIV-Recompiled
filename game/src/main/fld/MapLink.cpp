#include "main/fld/MapLink.hpp"
#include "main/dss/DssUtils.hpp"
#include "nitro/os.hpp"

ARM void CMapLink::setup(void* data)
{
    chunk* cnk = (chunk*)data;

    while (cnk->id != 0) {
        switch (cnk->id) {
        case 0x454d414e: // "NAME"
            m_name = (char*)cnk;
            break;
        case 0x41544144: // "DATA"
            m_data = cnk + 1;
            break;
        case 0x48534148: { // "HASH"
            unsigned short* p = (unsigned short*)(cnk + 1);
            m_data_num = *p;
            m_hash = p + 1;
            break;
        }
        default:
            OS_Terminate();
            break;
        }
        cnk = (chunk*)((unsigned char*)cnk + cnk->size + sizeof(chunk));
    }
}

ARM char* CMapLink::search(const char* map_name, int exit_id)
{
    int h;
    MAP_LINK_IDX* idx;
    char* name_top;
    int old;
    MAP_LINK_TBL* tbl;

    name_top = m_name;
    m_exit_id = exit_id;
    old = hash(map_name);
    h = old;
    do {
        idx = (MAP_LINK_IDX*)((unsigned char*)m_data + m_hash[h]);
        if (dss::strcmp(map_name, name_top + idx->name) == 0) {
            tbl = (MAP_LINK_TBL*)(idx + 1);
            for (int i = 0; i < idx->n_exit; i++) {
                if (tbl[i].exit_id == exit_id && tbl[i].name != 0) {
                    m_exit_id = tbl[i].target_id;
                    return name_top + tbl[i].name;
                }
            }
            break;
        }
        h = (h + 1) % m_data_num;
    } while (h != old);
    return NULL;
}

ARM int CMapLink::hash(const char* name)
{
    unsigned int h = 0;

    while (*name) {
        h = h * 37 + *name++;
    }
    return h % m_data_num;
}
