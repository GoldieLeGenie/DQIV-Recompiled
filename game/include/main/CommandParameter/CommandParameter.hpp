#include "globaldefs.h"
#include "GameInfo.hpp"


struct CommandParameter {
    unsigned short command_;
    unsigned char flag_;
    unsigned char reserved_;
    int param_[1];
};