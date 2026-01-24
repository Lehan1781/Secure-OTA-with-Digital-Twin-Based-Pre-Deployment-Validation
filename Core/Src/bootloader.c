#include "bootloader.h"

#include "boot_metadata.h"
#include "memory_map.h"
#include "image_validate.h"
#include "jump.h"

#include <stdio.h>

/*
 * Bootloader main decision logic
 * Called once from main()
 */
void bootloader_run(void)
{
    uint32_t target_addr;

    /* Load metadata from flash */
    read_metadata();

    printf("\r\n[BOOT] Bootloader running\r\n");

    /* -------------------------------------------------
     * Decide which slot to boot
     * ------------------------------------------------- */
    if (boot_meta.update_pending)
    {
        printf("[BOOT] Update pending\r\n");

        /* Boot the inactive slot */
        target_addr = (boot_meta.active_slot == SLOT_A) ?
                        SLOT_B_START : SLOT_A_START;

        boot_meta.boot_attempts++;
    }
    else
    {
        /* Normal boot: active slot */
        target_addr = (boot_meta.active_slot == SLOT_A) ?
                        SLOT_A_START : SLOT_B_START;
    }

    /* -------------------------------------------------
     * Validate selected image
     * ------------------------------------------------- */
    if (!is_valid_app(target_addr))
    {
        printf("[BOOT] Image invalid, rolling back\r\n");

        /* Roll back to last known good slot */
        target_addr = (boot_meta.last_good_slot == SLOT_A) ?
                        SLOT_A_START : SLOT_B_START;
    }
    else
    {
        printf("[BOOT] Image valid\r\n");
    }

    printf("[BOOT] Jumping to 0x%08lX\r\n", target_addr);

    /* Small delay to flush UART */
    for (volatile uint32_t i = 0; i < 1000000; i++);

    /* Jump to application (never returns) */
    jump_to_app(target_addr);

    /* Safety trap */
    while (1);
}
