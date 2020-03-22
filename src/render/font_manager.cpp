#include "font_manager.h"
#include <ft2build.h>
#include <glm/glm.hpp>
#include <iostream>
#include FT_FREETYPE_H

FontManager::FontManager() {
    load_font("resources/Helvetica.ttc");
}

FontManager::~FontManager() {
    // deallocate font here
}

bool FontManager::load_font(std::string font_name) {
    // loads the FreeType library
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "Error loading FreeType" << std::endl;
        return false;
    }

    // loads a new face (font)
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "Error loading " << font_name << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face,
        0, // set width to 0 to allow dynamic width based on height
        48); // set height to 48 pixels

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters from ASCII
    for (GLubyte c = 0; c < 128; c++) {
        // load glyph (character)
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "Error loading FreeType glyph" << std::endl;
            return false;
        }

        // generate an individual texture for the glyph
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        CharInfo char_info{texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint) face->glyph->advance.x};
        char_infos.insert(std::pair<GLchar, CharInfo>(c, char_info));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
}


CharInfo& FontManager::get_char_info(GLubyte c) {
    return char_infos[c];
}