#pragma once
#include "globaldefs.h"
#include "main/data/DataObject.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/TextureObject.hpp"
#include "main/dss/UnkSprite2D.hpp"
#include "ov001/fld/WorldMap.hpp"

namespace fld {
    struct FieldData {
        enum {
            SYMBOL_NUM_MAX = 114,
            SYMBOL_TEXTURE_NUM = 21,
            ANIM_PATTERN8 = 8,
            ANIM_PATTERN24 = 24
        };

        DataObject unk_0000;                                            // 0x0000 pattern8 pack
        DataObject unk_0010;                                            // 0x0010 pattern24 pack
        DataObject unk_0020;                                            // 0x0020 symbol pack
        TextureObject* unk_0030[SYMBOL_TEXTURE_NUM];                    // 0x0030 symbol textures
        UnkSprite2D m_symbol[SYMBOL_NUM_MAX];                           // 0x0084
        unsigned short unk_1974[SYMBOL_TEXTURE_NUM][16];                // 0x1974 symbol palettes
        DataObject dataObject_;                                         // 0x1C14 map file
        DataObject unk_1c24;                                            // 0x1C24 block texture file
        TextureObject* unk_1c34;                                        // 0x1C34
        TextureObject unk_1c38;                                         // 0x1C38
        CWorldMap m_cell_map;                                           // 0x1CA8
        CWorldSymbol unk_1cc4;                                          // 0x1CC4
        int unk_1ccc;                                                   // 0x1CCC pattern8 texture block
        int unk_1cd0;                                                   // 0x1CD0 pattern24 texture block
        unsigned short unk_1cd4[256];                                   // 0x1CD4 block palette
        unsigned short unk_1ed4[256];                                   // 0x1ED4 block palette (original colors)
        int unk_20d4;                                                   // 0x20D4 pattern8 frame
        int unk_20d8;                                                   // 0x20D8 pattern24 frame
        int pause_;                                                     // 0x20DC
        dss::Fix32Vector3 position_;                                    // 0x20E0
        dss::Vector2<int> baseBlock_;                                   // 0x20EC
        dss::Vector2<dss::Fix32> ofsBlock_;                             // 0x20F4
        int offset_;                                                    // 0x20FC
        int frame_;                                                     // 0x2100
        int unk_2104;                                                   // 0x2104 search area left
        int unk_2108;                                                   // 0x2108 search area top
        int unk_210c;                                                   // 0x210C search area right
        int unk_2110;                                                   // 0x2110 search area bottom
        int unk_2114;                                                   // 0x2114
        dss::Vector2<int> kanbanPos_[20];                               // 0x2118
        unsigned short kanbanId_[20];                                   // 0x21B8
        int kanbanCount_;                                               // 0x21E0

        FieldData();
        ~FieldData();
        void setup(int map, int ch);
        void setupBlock(int map);
        void setupSymbol();
        void cleanup();
        void setPlayerCamera();
        void setFieldCamera();
        void drawBlock();
        void draw2DField(int x, int y, int u, int v);
        void drawSymbol();
        void nextAnimation();
        void draw();
        void setPosition(dss::Fix32Vector3& pos);
        bool isEnable(int bx, int by);
        int getAttr(int bx, int by);
        dss::Fix32Vector3 getSymbolPosition(int index);
        int getSearchSymbolAttach(dss::Fix32Vector3& pos);
        void setKekaiSymbol(int index, int x, int y, int w, int h);
        void setPaletteRate(dss::Fix32Vector3& rate);
        void setKanban();
        int searchKanban(int x, int y, dss::Vector2<dss::Fix32>* pos);
        void setDispSymbol(int id, int disp);
    };
}
