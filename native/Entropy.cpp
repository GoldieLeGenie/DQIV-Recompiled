#include <random>
#include <limits>

static_assert(std::numeric_limits<unsigned int>::digits == 32);
// Host replacement for the SDK entropy source. The game's RNG is unchanged.
extern "C" void OS_GetLowEntropyData(unsigned int buffer[8]) {
    std::random_device source;
    for (int i = 0; i < 8; ++i) buffer[i] = source();
}
