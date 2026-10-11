#include "main/dss/DssUtils.hpp"

THUMB int dss::arrayToMinIndex(int* array, int count)
{
    int index = -1;
    int min = 10000;

    for (int i = 0; i < count; i++) {
        if (array[i] < min) {
            min = array[i];
            index = i;
        }
    }

    return index;
}

