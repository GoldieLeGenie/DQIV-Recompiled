#pragma once
#include "Assets.hpp"
namespace pc {
class NitroFilesystem {
public:
    explicit NitroFilesystem(const Assets&);
    ~NitroFilesystem();
    NitroFilesystem(const NitroFilesystem&) = delete;
    NitroFilesystem& operator=(const NitroFilesystem&) = delete;
};
}
