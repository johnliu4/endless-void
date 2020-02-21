/**
 * Utilities for loading external resources for rendering.
 * Public interfacing for the render engine.
 */

#ifndef RENDER_LOADER_H
#define RENDER_LOADER_H

#include <glad/glad.h>
#include <string>

// loads a GLSL shader and returns the id of the shader
GLuint load_shader(const char* file_path, const GLuint shader_type);

#endif
