#ifndef _NITRO_FS_ROM_H
#define _NITRO_FS_ROM_H

#include <nitro/reg.h>
#include <nitro/types.h>

extern CartridgeRegion fsi_ovt7;
extern CartridgeRegion fsi_ovt9;

extern FS_Record fsi_arc_rom;

/**
 * @brief Loads the ROM file allocation and name tables into memory if sufficient space is provided
 *
 * @param buf Pointer to memory where the tables will be loaded
 * @param size Size of the memory buffer. If insufficient, only returns required size
 *
 * @return The size of memory used by FAT and FNT
 */
u32 FS_TryLoadTable(void* buf, u32 size);

#endif // _NITRO_FS_ROM_H