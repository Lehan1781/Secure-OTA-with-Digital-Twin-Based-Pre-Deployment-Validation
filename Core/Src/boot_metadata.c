#include "boot_metadata.h"
#include "memory_map.h"

#define METADATA_PTR ((boot_metadata_t*)METADATA_ADDR)

boot_metadata_t boot_meta;

void read_metadata(void)
{
    if (METADATA_PTR->magic == METADATA_MAGIC)
    {
        boot_meta = *METADATA_PTR;
    }
    else
    {
        // First boot or erased flash
        boot_meta.active_slot = 0;
        boot_meta.last_good_slot = 0;
        boot_meta.slot_a_crc = 0;
        boot_meta.slot_b_crc = 0;
        boot_meta.update_pending = 0;
        boot_meta.boot_attempts = 0;
        boot_meta.magic = METADATA_MAGIC;
    }
}

