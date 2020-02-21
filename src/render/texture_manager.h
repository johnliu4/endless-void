#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <glad/glad.h>
#include <list>
#include <string>

/**
 * A unique identifier for textures.
 * The name should be unique.
 */
struct TextureIdentifier {
	std::string name;
	GLuint texture_id;
};

/**
 * A managing system for textures to ensure that no texture is ever
 * loaded more than once and to manage textures in memory.
 */
class TextureManager {
private:
	static TextureManager* singleton;
	TextureManager();
	~TextureManager();

	std::list<TextureIdentifier> texture_list;

	GLuint load_png(std::string file_path);

public:
	// singleton methods
	static TextureManager* get_instance();
	// must be called before any call to get_instance()
	static void init();
	static void destroy();

	// returns whether or not a texture has been loaded already
	bool exists(std::string file_path) const;

	// adds a texture if it does not already exist
	void add_texture(std::string file_path);

	// gets the id of a given texture
	// if the texture does not already exist, it will be created
	GLuint get_texture(std::string file_path);
};

#endif
