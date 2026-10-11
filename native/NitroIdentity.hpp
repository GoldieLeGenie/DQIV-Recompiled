#pragma once
#include <array>
#include <cstdint>
#include <filesystem>

namespace pc {
// A local unicast device identity belongs to the save profile. It survives
// restarts and does not depend on the PC's physical network adapters.
class NitroIdentity {
public:
    explicit NitroIdentity(const std::filesystem::path& path);
    ~NitroIdentity();
    NitroIdentity(const NitroIdentity&)=delete;
    NitroIdentity& operator=(const NitroIdentity&)=delete;
    const std::array<std::uint8_t,6>& address() const {return address_;}
private:
    std::array<std::uint8_t,6> address_{};
};
}
