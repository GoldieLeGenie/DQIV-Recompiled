#pragma once
#include "GxGeometry.hpp"
namespace pc {
// One context per rendering thread; nested contexts restore the previous one.
class NitroGeometry {
public:
    explicit NitroGeometry(GxGeometry&);
    ~NitroGeometry();
    NitroGeometry(const NitroGeometry&)=delete;
    NitroGeometry& operator=(const NitroGeometry&)=delete;
private:
    GxGeometry* previous_;
};
GxGeometry& activeGeometry();
}
