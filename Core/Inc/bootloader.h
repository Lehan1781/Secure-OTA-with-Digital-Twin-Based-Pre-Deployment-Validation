#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>

/* Boot slots */
#define SLOT_A   0
#define SLOT_B   1

/* Bootloader public API */
void bootloader_run(void);

#endif /* BOOTLOADER_H */
