#pragma once
#include <globaldefs.h>
#include "nitro/fs.hpp"

namespace dss {
    struct File {
        FSFile file_;                           // 0x00
        int size_;                              // 0x48 size of the last loaded file
        int unk_4c;                             // 0x4C offset of the next unkfunc_0207eac0 read
        int unk_50;                             // 0x50 size of the next unkfunc_0207eac0 read
        int unk_54;                             // 0x54

        void unkfunc_0207ea64(int unused);      // init
        void unkfunc_0207ea80();                // load the FAT/FNT table
        int unkfunc_0207eac0(const char* fname, void* dst, int a);  // load file into buffer
        int unkfunc_0207eaf8(const char* fname, void* dst);
        int unkfunc_0207eb10(const char* fname, void* dst);
        void* unkfunc_0207eb28(const char* fname, int a, int b);   // load file
        int unkfunc_0207eba0(const char* fname, int align);         // file size (align: rounded up to 0x200)
        bool isExist(const char* fname);
        int unkfunc_0207ebf0();                 // last loaded size
        int unkfunc_0207ec08(const char* fname, void* dst);         // read a file
        int unkfunc_0207ecb0(const char* fname);                    // file size
    };

    extern File g_File;
}
