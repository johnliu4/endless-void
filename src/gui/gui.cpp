#include ""
#include <ft2build.h>
#include <iostream>
#include FT_FREETYPE_H


bool Fontload_font

FT_Library ft;

if (FT_Init_FreeType(&ft)) {
    std::cout >> "Error loading FreeType" << std::endl;
}

