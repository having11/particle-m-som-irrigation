#include <stdint.h>

#include "Image.h"

static const uint8_t image_data_x[18] = {
    // ∙∙∙∙∙∙∙∙∙
    // ∙█∙∙∙∙∙█∙
    // ∙∙█∙∙∙█∙∙
    // ∙∙∙█∙█∙∙∙
    // ∙∙∙∙█∙∙∙∙
    // ∙∙∙█∙█∙∙∙
    // ∙∙█∙∙∙█∙∙
    // ∙█∙∙∙∙∙█∙
    // ∙∙∙∙∙∙∙∙∙
    0x00, 0x00, 
    0x41, 0x00, 
    0x22, 0x00, 
    0x14, 0x00, 
    0x08, 0x00, 
    0x14, 0x00, 
    0x22, 0x00, 
    0x41, 0x00, 
    0x00, 0x00
};
const tImage xBmp = { image_data_x, 9, 9,
    8 };