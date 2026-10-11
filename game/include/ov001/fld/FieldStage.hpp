#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "ov001/fld/FieldData.hpp"

namespace fld {
    struct FieldStage {
        FieldData fieldData;                                                        // 0x0000
        int map_;                                                                   // 0x21E4
        int change_;                                                                // 0x21E8

        FieldStage();
        ~FieldStage();
        static FieldStage* getSingleton();
        void setDispSymbol(int id, int disp) { fieldData.setDispSymbol(id, disp); }
        int searchKanban(int x, int y, dss::Vector2<dss::Fix32>* pos) { return fieldData.searchKanban(x, y, pos); }
        void initialize();
        void terminate();
        void execute();
        void draw();
        void drawPlayer();
        void setOffset(int val);
        void setPosition(dss::Fix32Vector3& pos);
        bool getBlockAttr(int bx, int by);
        int getBlockAttr2(int bx, int by);
        dss::Fix32Vector3 getSymbolPosition(int index);
        int getSearchSymbolAttach(dss::Fix32Vector3 pos);
        void ChangeTime(int apply);
        dss::Vector2<int> calcDrawPosition(dss::Fix32Vector3 pos);
        void setSymbolFlag(int uid);
        void eraseSymbol(int id);
    };
}
