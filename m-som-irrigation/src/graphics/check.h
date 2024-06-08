#include <stdint.h>

#include "Image.h"

static const uint8_t image_data_check[18] = {
    // ∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙█
    // ∙∙∙∙∙∙∙█∙
    // ∙∙∙∙∙∙█∙∙
    // ∙█∙∙∙█∙∙∙
    // ∙∙█∙█∙∙∙∙
    // ∙∙∙█∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙
    0x00, 0x00, 
    0x00, 0x00, 
    0x00, 0x80, 
    0x01, 0x00, 
    0x02, 0x00, 
    0x44, 0x00, 
    0x28, 0x00, 
    0x10, 0x00, 
    0x00, 0x00
};

const tImage checkBmp = { image_data_check, 9, 9,
    8 };
