#pragma once

// C++ subset of nitro/fs.h
struct FSFile {
    char unk_00[0x24];
    unsigned int startRomOffset;                // 0x24
    unsigned int endRomOffset;                  // 0x28
    char unk_2c[0x1c];
};

extern "C" {
    int FS_LoadOverlay(void* overlay, unsigned int overlayID);
    int FS_UnloadOverlay(void* overlay, unsigned int overlayID);
    unsigned int FS_TryLoadTable(void* buf, unsigned int size);
    void func_02060e04(FSFile* file);           // FS_InitFile
    int func_020610e4(FSFile* file, const char* path);  // FS_OpenFile
    int func_0206112c(FSFile* file);            // FS_CloseFile
    int func_02061270(FSFile* file, void* buf, int size);  // FS_ReadFile
    int func_02061280(FSFile* file, int pos, int mode);     // FS_SeekFile
}
