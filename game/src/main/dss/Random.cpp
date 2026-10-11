#include <globaldefs.h>
#include "main/dss/Random.hpp"

dssrand::Random dssrand::generator;


ARM void dssrand::unkfunc_02080cec()
{
    unsigned int buffer[8];
    unsigned int seed = 0;
    OS_GetLowEntropyData(buffer);
    for (int i = 0; i < 8; i++) {
        seed += buffer[i];
    }
    generator.state = seed;
    generator.multiplier = 0x5d588b65;
    generator.increment = 0x269ec3;
}

ARM int dssrand::rand(int n) {
    unsigned int masked_n;
    
    generator.state = generator.multiplier * generator.state + generator.increment;

    masked_n = (unsigned int)n & 0xFFFF;

    if (masked_n != 0) {
        return ((((generator.state >> 16) * masked_n) >> 16) & 0xFFFF);
    }
    return ((generator.state >> 16) & 0xFFFF);
}
