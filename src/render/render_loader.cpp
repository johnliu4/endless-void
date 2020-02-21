#include "render_loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

GLuint load_shader(const char* file_path, const GLuint shader_type) {
	GLuint shader_id = glCreateShader(shader_type);

	// parse data from shader file
	std::string shader_data;
	std::ifstream shader_stream(file_path, std::ios::in);
	if (shader_stream.is_open()) {
		std::stringstream str_stream;
		str_stream << shader_stream.rdbuf();
		shader_data = str_stream.str();
		shader_stream.close();
	} else {
		std::cout << "Unable to load shader file: " << file_path << std::endl;
		std::cin.get();
		return 0;
	}

	// send shader data to OpenGL
	const char* src_ptr = shader_data.c_str();
	glShaderSource(shader_id, 1, &src_ptr, nullptr);
	glCompileShader(shader_id);

	// check for shader compilation errors
	GLint result;
	GLint log_len;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_len);
	if (log_len > 0) {
		GLchar* msg = new GLchar[log_len + 1];
		glGetShaderInfoLog(shader_id, log_len, nullptr, &msg[0]);
		std::cout << "Shader compilation error: " << msg;
		delete[] msg;
	}

	return shader_id;
}
