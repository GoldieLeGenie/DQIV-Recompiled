#include "main/data/DataObject.hpp"
#include "main/data/FileLoader.hpp"

static int s_dataCount;

ARM DataObject::DataObject()
{
    m_flag = 0;
    m_addr = 0;
}

ARM DataObject::~DataObject()
{
}

ARM void DataObject::setup(const char* filename, int a, int b)
{
    m_flag |= 1;
    m_addr = dss::g_File.unkfunc_0207eb28(filename, b, a);
    m_size = dss::g_File.unkfunc_0207ebf0();
    s_dataCount++;
}

ARM void DataObject::setup(void* addr)
{
    s_dataCount++;
    m_addr = addr;
}

ARM void DataObject::cleanup()
{
    s_dataCount--;
    if (m_flag & 1) {
        unkfunc_0207f840(&data_0211a60c, m_addr);
    }
    m_flag = 0;
    m_addr = 0;
}

ARM void* DataObject::getAddr()
{
    return m_addr;
}

ARM long DataObject::getSize()
{
    return m_size;
}

ARM void LZDataObject::setup(const char* filename, int a, int b)
{
    m_flag |= 1;
    m_addr = dss::g_File.unkfunc_0207eb28(filename, b, a);
    m_size = dss::g_File.unkfunc_0207ebf0();
    if (unkfunc_0207f548(m_addr)) {
#ifdef DQIV_NATIVE
        try {
#endif
        m_size = unkfunc_0207f52c(m_addr);
        void* data = unkfunc_0207f590(m_addr);
        unkfunc_0207f840(&data_0211a60c, m_addr);
        m_addr = data;
#ifdef DQIV_NATIVE
        } catch (...) {
            unkfunc_0207f840(&data_0211a60c, m_addr);
            m_addr = 0;
            m_flag = 0;
            throw;
        }
#endif
    }
    s_dataCount++;
}

ARM void LZDataObject::setup(void* addr)
{
    s_dataCount++;
    m_addr = addr;
}
