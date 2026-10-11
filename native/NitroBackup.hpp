#pragma once
#include "SaveStore.hpp"
namespace pc {
class NitroBackup {
public:
    explicit NitroBackup(SaveStore&);
    ~NitroBackup();
    NitroBackup(const NitroBackup&)=delete;
    NitroBackup& operator=(const NitroBackup&)=delete;
};
}
