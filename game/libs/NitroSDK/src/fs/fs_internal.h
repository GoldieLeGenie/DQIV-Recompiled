#ifndef FS_INTERNAL_H
#define FS_INTERNAL_H

#include <nitro/fs.h>
#include <nitro/os/cpustat.h>
#include <nitro/mi/cpumem.h>


/* Directory entry filled by the read-directory command (0x94 bytes). */
typedef struct UnkFsDirEntry {
    /* 0x00 */ union {
        FS_FileIdentifier unk_file; /* file entry: archive + file index */
        FS_FilePosition   unk_dir;  /* directory entry: archive + directory id */
    } unk_00;
    /* 0x0C */ u32  unk_0c; /* 1 = directory */
    /* 0x10 */ u32  unk_10; /* name length */
    /* 0x14 */ char unk_14[128];
} UnkFsDirEntry;

/* Table read cursor used by the archive commands. */
typedef struct UnkFsTableCursor {
    /* 0x00 */ FS_Record* unk_00;
    /* 0x04 */ u32        unk_04;
} UnkFsTableCursor;

/* FS_File as seen by the archive commands (properties at 0x20, command arguments at 0x30). */
typedef struct UnkFsFile {
    /* 0x00 */ FS_LinkedFile    link;
    /* 0x08 */ FS_Record*       record;
    /* 0x0C */ vu32             status;
    /* 0x10 */ u32              unk_10;
    /* 0x14 */ FS_CommandResult result;
    /* 0x18 */ OSThreadQueue    unk_18;
    /* 0x20 */ union {
        struct {
            u32 unk_20; /* file index */
            u32 unk_24; /* top */
            u32 unk_28; /* bottom */
            u32 unk_2c; /* current position */
        } file;
        struct {
            FS_FilePosition unk_20; /* current directory position */
            u32             unk_2c; /* parent directory id */
        } dir;
    } prop;
    /* 0x30 */ union {
        struct {
            void* unk_30;
            u32   unk_34;
            u32   unk_38;
        } cmd0; /* read */
        struct {
            const void* unk_30;
            u32         unk_34;
            u32         unk_38;
        } cmd1; /* write */
        struct {
            FS_FilePosition unk_30;
        } cmd2; /* seek directory */
        struct {
            UnkFsDirEntry* unk_30;
            BOOL           unk_34; /* skip name */
        } cmd3; /* read directory */
        struct {
            FS_FilePosition unk_30;
            const char*     unk_3c;
            BOOL            unk_40; /* look for a directory */
            void*           unk_44; /* FS_FileIdentifier* or FS_FilePosition* */
        } cmd4; /* find path */
        struct {
            u8* unk_30;
            u32 unk_34;
            u16 unk_38;
            u16 unk_3a;
        } cmd5; /* get path */
        struct {
            FS_FileIdentifier unk_30;
        } cmd6; /* open by id */
        struct {
            u32 unk_30;
            u32 unk_34;
            u32 unk_38;
        } cmd7; /* open by range */
    } arg;
} UnkFsFile;

typedef FS_CommandResult (*UnkFsCommandFunc)(FS_File* file);
extern const UnkFsCommandFunc data_020b6328[9]; /* default archive commands */

static inline BOOL FSi_IsRecordSync(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_SYNC) != 0;
}

static inline BOOL FSi_IsPathChar(u32 c) {
    BOOL ret = FALSE;
    if (c != '\0' && c != '/' && c != '\\') {
        ret = TRUE;
    }
    return ret;
}

static inline BOOL FSi_IsFileDir(volatile UnkFsFile* file) {
    return (file->status & FS_FILE_STATUS_IS_DIR) != 0;
}

static inline u32 FSi_GetNameLength(u32 name) {
    if (name <= 0xFF) {
        return 1;
    } else if (name <= 0xFF00) {
        return 2;
    } else {
        return 3;
    }
}

/* fs_archive.c */
void             func_0205f9bc(FS_File* file, FS_CommandResult result);
FS_CommandResult func_0205fa18(FS_File* file, u32 command);
u32              func_02060548(const char* name, s32 nameLen);
FS_File*         func_020605f4(FS_Record* record);
void             func_02060808(FS_File* file);
BOOL             func_0206089c(FS_File* file);
BOOL             func_020608e0(FS_File* file, u32 command);
FS_Record*       func_02060ab0(const char* name, s32 nameLen);

/* fs_file.c */
void FSi_InitRom(u32 dmaNo);
void func_02060e04(FS_File* file);
BOOL func_02060e2c(FS_File* file, const char* path, FS_FileIdentifier* iden, FS_FilePosition* dirPosition);
s32  func_02060f70(FS_File* file, void* dest, s32 size, BOOL async);
BOOL func_02060fec(FS_FileIdentifier* iden, const char* path);
BOOL func_0206102c(FS_File* file, FS_Record* record, u32 startPos, u32 endPos, u32 fileIdx);
BOOL func_02061074(FS_File* file, FS_FileIdentifier iden);
BOOL func_020610e4(FS_File* file, const char* path);
BOOL func_0206112c(FS_File* file);
BOOL func_02061168(FS_File* file);
void func_02061228(FS_File* file);
s32  func_02061270(FS_File* file, void* dest, s32 size);
BOOL func_02061280(FS_File* file, s32 offset, s32 mode);

extern FS_FilePosition data_0210cdd4; /* current directory */

#endif
