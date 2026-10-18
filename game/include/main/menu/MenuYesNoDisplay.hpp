#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"
#include "main/dss/BgBuffer.hpp"

// Yes/no window drawn on the sub screen, texts drawn in two 6x2 character buffers (g_MenuDisplays.yesNo_)
struct UnkMenuYesNoDisplay : UnkMenuDisplay {
    UnkCharBuffer yesChar_;                     // 0x030
    UnkCharBuffer noChar_;                      // 0x040
    char yesData_[0x180];                       // 0x050
    char noData_[0x180];                        // 0x1D0
    int transfer_;                              // 0x350  1: transfer the texts
    int cursor_;                                // 0x354  0: yes, 1: no

    UnkMenuYesNoDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void unkfunc_02052a24(UnkOamBuffer* main, UnkOamBuffer* sub);
    void setTexts(int yesMessageId, int noMessageId);
    void setCursor(int cursor);
    void transferRow(int row);                  // copy one row of both texts to VRAM
};
