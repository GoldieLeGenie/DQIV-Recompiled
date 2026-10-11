#pragma ipa file
#include "main/dss/BuildDate.hpp"
#include <string.h>

// The original used __DATE__ / __TIME__ (build of Mar 19 2008, 15:55:11).
ARM BuildDate::BuildDate()
{
    date_ = "Mar 19 2008";
    strcpy(time_, "15:55:11");
    strcpy(dateString_, "Mar 19 2008");
}

ARM void BuildDate::unkfunc_02057000()
{
    const char* month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    for (int i = 0; i < 12; i++) {
        if (date_[0] == month[i][0] && date_[1] == month[i][1] && date_[2] == month[i][2]) {
            int m = i;
            m++;
            dateString_[0] = date_[9];
            dateString_[1] = date_[10];
            dateString_[2] = '/';
            dateString_[3] = '0' + m / 10;
            dateString_[4] = '0' + m % 10;
            dateString_[5] = '/';
            dateString_[6] = date_[4] == ' ' ? '0' : date_[4];
            dateString_[7] = date_[5];
            dateString_[8] = 0;
        }
    }
    time_[5] = 0;
}
