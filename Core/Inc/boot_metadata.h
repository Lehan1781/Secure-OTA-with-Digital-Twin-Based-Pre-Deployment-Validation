#ifndef BOOT_METADATA_H
#define BOOT_METADATA_H

#include <stdint.h>

typedef struct {
    uint32_t active_slot;      // 0 = A, 1 = B
    uint32_t last_good_slot;
    uint32_t slot_a_crc;
    uint32_t slot_b_crc;
    uint32_t update_pending;
    uint32_t boot_attempts;
    uint32_t magic;
} boot_metadata_t;

#define METADATA_MAGIC 0xB007B007

extern boot_metadata_t boot_meta;
void read_metadata(void);

#endif
