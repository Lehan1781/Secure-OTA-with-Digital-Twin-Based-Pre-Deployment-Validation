#include "image_validate.h"

int is_valid_app(uint32_t app_addr)
{
    uint32_t sp = *(uint32_t*)app_addr;
    uint32_t pc = *(uint32_t*)(app_addr + 4);

    // Stack pointer must be in SRAM
    if ((sp & 0x2FFE0000) != 0x20000000)
        return 0;

    // Reset handler must be in Flash
    if ((pc & 0x08000000) != 0x08000000)
        return 0;

    return 1;
}
