#ifndef RENDER_GUI_H
#define RENDER_GUI_H

#include "font_manager.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <list>

/**
 * A single line of text.
 */
struct TextLine {
    unsigned int text_id;
    // height of text in pixels
    GLuint font_size;
    // distance from top of window in pixels
    GLint top;
    // distance from left of window in pixels
    GLint left;
    // string of text to render
    std::string text;
};

/**
 * Gui renderer. Used to display text and images in 2D.
 */
class RenderGui {
private:
    // dimensions of gui in pixels
    unsigned int window_width;
    unsigned int window_height;
    // id counter for text lines
    unsigned int text_line_id_counter;
    // list of lines of text to render
    std::list<TextLine> text_lines;
    // id of VAO
    GLuint vertex_array_id;
    // id of VBO for quad representing letter
    GLuint vertex_buffer_id;
    // id of shader program for text rendering
    GLuint text_program_id;
    // projection matrix location in GLSL
    GLuint proj_mat_id;
    // projection matrix
    glm::mat4 proj_mat;

    FontManager& font_manager;

    void render_text_line(TextLine& text_line);

public:
    RenderGui();
    ~RenderGui();

    void render();

    // adds a line of text for rendering
    // returns an id to the text which can be used to remove it
    unsigned int add_text_line(GLuint font_size, GLint top, GLint left, std::string text);

    void remove_text_line(GLuint text_id);
};

#endif