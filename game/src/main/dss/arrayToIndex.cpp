#include "main/dss/DssUtils.hpp"

THUMB int dss::arrayToIndex(int* array, int value, int max)
{
    int sums[32];
    int lo;
    int hi;
    int result;

    sums[0] = array[0];

    for (int i = 1; i < max; i++)
        sums[i] = array[i] + sums[i - 1];

    result = 0;

    for (int i = 0; i < max; i++) {
        if (i == 0)
            lo = 0;
        else
            lo = sums[i - 1];

        hi = sums[i];

        if (lo != hi && lo <= value && value < hi) {
            result = i;
            break;
        }
    }

    return result;
}