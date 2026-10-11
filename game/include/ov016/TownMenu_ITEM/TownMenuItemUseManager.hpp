#pragma once
#include "globaldefs.h"

// town item menu: event item used from the item menu (checked by the town scripts)
struct TownMenuItemUseManager
{
    unsigned char eventItem_;               /* 0x0 */
    int eventItemFlag_;                     /* 0x4 */
    int closeMenuFlag_;                     /* 0x8 */

    static TownMenuItemUseManager* getSingleton();
    void setEventItem(unsigned char item) { eventItem_ = item; }
    bool getDefaultCloseMenuItem(int item);
};
