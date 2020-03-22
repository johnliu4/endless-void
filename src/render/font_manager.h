#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <map>

struct CharInfo {
    GLuint texture_id; // OpenGL texture id of glyph
    glm::ivec2 size; // dimensions of glyph in pixels
    glm::ivec2 bearing; // offset from baseline to top-left corner of glyph in pixels
    GLuint advance; // offset to next glyph in pixels
};

class FontManager {
private:
    std::map<GLchar, CharInfo> char_infos;

public:
    FontManager();
    ~FontManager();
    

    bool load_font(std::string font_path);

    CharInfo& get_char_info(GLubyte c);
};

#endif