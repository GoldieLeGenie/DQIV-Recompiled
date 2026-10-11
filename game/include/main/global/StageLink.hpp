#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"

struct StageLink {
    static DataObject mapLinkData_;
    static int townExitIndex_;
    static int fieldSymbolIndex_;

    static void initialize();
    static char* getName(const char* name, int index);
    static int getSymbolIndex();
    static void setTownExitIndex(int index);
    static void resetTownExitIndex();
    static int getTownExitIndex();
    static void setFieldSymbolIndex(int index);
    static int getFieldSymbolIndex();
};
