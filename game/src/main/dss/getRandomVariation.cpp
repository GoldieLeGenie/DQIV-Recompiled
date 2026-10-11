#include "main/dss/DssUtils.hpp"
#include "main/dss/Random.hpp"

THUMB int dss::getRandomVariation(int value, int under, int over)
{
    return value * (100 - under + dssrand::rand(under + over + 1)) / 100;
}