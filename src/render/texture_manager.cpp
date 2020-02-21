#include "texture_manager.h"
#include <lodepng.h>
#include <iostream>

TextureManager* TextureManager::singleton = nullptr;

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
	for (std::list<TextureIdentifier>::iterator it = texture_list.begin(); it != texture_list.end(); it++) {
		glDeleteTextures(1, &(it->texture_id));
	}

	texture_list.clear();
}

GLuint TextureManager::load_png(std::string file_path) {
	std::vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, file_path.c_str());

	if (error != 0) {
		std::cout << "Load PNG error " << error << ": " << lodepng_error_text(error) << std::endl;
		return 0;
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture_id;
}

TextureManager* TextureManager::get_instance() {
	return TextureManager::singleton;
}

void TextureManager::init() {
	singleton = new TextureManager();
}

void TextureManager::destroy() {
	delete singleton;
}

bool TextureManager::exists(std::string file_path) const {
	for (std::list<TextureIdentifier>::const_iterator it = texture_list.begin(); it != texture_list.end(); it++) {
		if (it->name == file_path) {
			return true;
		}
	}

	return false;
}

void TextureManager::add_texture(std::string file_path) {
	if (!this->exists(file_path)) {
		GLuint texture_id = load_png(file_path);
		texture_list.push_back({ file_path, texture_id });
	}
}

GLuint TextureManager::get_texture(std::string file_path) {
	for (std::list<TextureIdentifier>::iterator it = texture_list.begin(); it != texture_list.end(); it++) {
		if (it->name == file_path) {
			return it->texture_id;
		}
	}

	GLuint texture_id = load_png(file_path);
	texture_list.push_back({ file_path, texture_id });
	return texture_id;
}
