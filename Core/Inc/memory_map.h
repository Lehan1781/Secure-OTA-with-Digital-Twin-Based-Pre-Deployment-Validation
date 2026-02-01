#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define FLASH_BASE_ADDR     0x08000000UL

#define BOOTLOADER_START    0x08000000UL
#define SLOT_A_START        0x08010000UL
#define SLOT_B_START        0x08040000UL
#define METADATA_ADDR       0x08078000UL

#define SLOT_SIZE           (224 * 1024)   // adjust if needed

#endif
