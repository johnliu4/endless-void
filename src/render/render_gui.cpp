#include "render_gui.h"
#include "render_engine.h"
#include "render_loader.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

RenderGui::RenderGui() :
    font_manager(RenderEngine::get_instance()->get_font_manager()
) {
    this->window_width = RenderEngine::get_instance()->get_window_width();
    this->window_height = RenderEngine::get_instance()->get_window_height();
    // initialize id counter
    text_line_id_counter = 0;
    // generate VAO and VBO
    glGenVertexArrays(1, &vertex_array_id);
    glGenBuffers(1, &vertex_buffer_id);
    glBindVertexArray(vertex_array_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);

    // load vertex and fragment shaders for text rendering
    GLuint vertex_shader_id = load_shader("resources/text_vert.glsl", GL_VERTEX_SHADER);
    GLuint fragment_shader_id = load_shader("resources/text_frag.glsl", GL_FRAGMENT_SHADER);
    text_program_id = glCreateProgram();
    glAttachShader(text_program_id, vertex_shader_id);
    glAttachShader(text_program_id, fragment_shader_id);
    glLinkProgram(text_program_id);
    glDetachShader(text_program_id, vertex_shader_id);
    glDetachShader(text_program_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    // get projection matrix location in GLSL
    // proj_mat_id = glGetUniformLocation(text_program_id, "proj_mat");
    proj_mat = glm::ortho(0.0f, (GLfloat) window_width, 0.0f, (GLfloat) window_height);

    font_manager = RenderEngine::get_instance()->get_font_manager();
}

RenderGui::~RenderGui() {

}

/**
 * Renders a TextLine; a single line of text.
 */
void RenderGui::render_text_line(TextLine& text_line) {
    // set the color of the text
    glUniform3f(glGetUniformLocation(text_program_id, "text_color"), 1.0f, 0.0f, 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(text_program_id, "proj_mat"), 1, GL_FALSE, &proj_mat[0][0]);
    glActiveTexture(GL_TEXTURE0);

    GLfloat x = text_line.left;
    GLfloat y = text_line.top;

    for (std::string::const_iterator c = text_line.text.begin();
        c != text_line.text.end(); c++) {
        CharInfo ci = font_manager.get_char_info(*c);

        GLfloat scale = 1.0f;

        GLfloat pos_x = x + ci.bearing.x * scale;
        GLfloat pos_y = y - (ci.size.y - ci.bearing.y) * scale;

        GLfloat width = ci.size.x * scale;
        GLfloat height = ci.size.y * scale;

        GLfloat vertices[6][4] = {
            {pos_x, pos_y + height, 0.0, 0.0},
            {pos_x, pos_y, 0.0, 1.0},
            {pos_x + width, pos_y, 1.0, 1.0},
            {pos_x, pos_y + height, 0.0, 0.0},
            {pos_x + width, pos_y, 1.0, 1.0},
            {pos_x + width, pos_y + height, 1.0, 0.0},
        };

        glBindTexture(GL_TEXTURE_2D, ci.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // note that advance is equal to 1/64 of a pixel
        x += (ci.advance >> 6) * scale;
    }
}

void RenderGui::render() {
    glBindVertexArray(vertex_array_id);
    glUseProgram(text_program_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    // render each TextLine
    for (std::list<TextLine>::iterator it = text_lines.begin(); it != text_lines.end(); it++) {
        render_text_line(*it);
    }

    glDisableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int RenderGui::add_text_line(GLuint font_size, GLint top, GLint left, std::string text) {
    text_line_id_counter++;
    TextLine text_line{text_line_id_counter, font_size, top, left, text};
    text_lines.push_back(text_line);
    return text_line_id_counter;
}

void RenderGui::remove_text_line(unsigned int text_id) {
    // find and removes the text line by text id
    for (std::list<TextLine>::iterator it = text_lines.begin(); it != text_lines.end(); it++) {
        if (text_id == it->text_id) {
            text_lines.erase(it);
            break;
        }
    }
}