#include "wf_obj_manager.h"
#include "render_loader.h"
#include "texture_manager.h"
#include <sstream>
#include <fstream>

// TODO move to utilities file
std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

WavefrontOBJEntity::WavefrontOBJEntity(WavefrontOBJData* obj_data) {
	this->obj_data = obj_data;
}

WavefrontOBJEntity::~WavefrontOBJEntity() {}

void WavefrontOBJEntity::render(Camera& camera) {
	load_model_matrix(camera);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, obj_data->vertex_buffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj_data->uv_buffer_id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj_data->texture_id);

	glDrawArrays(GL_TRIANGLES, 0, obj_data->num_vertices * 3);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

WavefrontOBJData::WavefrontOBJData(
    std::string name,
    GLuint vertex_buffer_id,
    GLuint uv_buffer_id,
    GLuint texture_id,
	GLuint num_vertices) {
    this->name = name;
    this->vertex_buffer_id = vertex_buffer_id;
    this->uv_buffer_id = uv_buffer_id;
    this->texture_id = texture_id;
	this->num_vertices = num_vertices;
    num_instances = 0;
}

WavefrontOBJManager::WavefrontOBJManager() {}
WavefrontOBJManager::~WavefrontOBJManager() {
	// deallocate all buffer data
	for (std::list<WavefrontOBJData*>::iterator it = obj_data_list.begin();
		it != obj_data_list.end(); it++) {
		WavefrontOBJData& data = **it;
		glDeleteBuffers(1, &data.vertex_buffer_id);
        glDeleteBuffers(1, &data.uv_buffer_id);
	}
}

WavefrontOBJData* WavefrontOBJManager::load_obj_data(std::string file_path) {
	std::ifstream obj_file;
	obj_file.open(file_path.c_str(), std::ios::in);

	if (obj_file.is_open()) {
		std::string line;

		std::vector<GLfloat> vertices;
		std::vector<GLfloat> uvs;
		std::vector<GLfloat> normals;

		std::vector<GLfloat> vertex_buffer;
		std::vector<GLfloat> uv_buffer;

		// keep track of the current texture being used and the number of vertices
		// that the texture is applied to
		bool using_textures = false;
		GLuint current_texture = 0;
		GLuint total_num_vertices = 0;
		GLuint current_num_vertices = 0;
		GLuint texture_id = 0;

		TextureManager* texture_manager = TextureManager::get_instance();

		while (std::getline(obj_file, line)) {
			// ignore blank lines and comments
			if (line.length() == 0 || line[0] == '#') {
				continue;
			}

			std::vector<std::string> tokens = split(line, ' ');
			if (tokens[0] == "v") {
				vertices.push_back(std::stof(tokens[1]));
				vertices.push_back(std::stof(tokens[2]));
				vertices.push_back(std::stof(tokens[3]));
			} else if (tokens[0] == "vt") {
				uvs.push_back(std::stof(tokens[1]));
				uvs.push_back(std::stof(tokens[2]));
			} else if (tokens[0] == "vn") {
				normals.push_back(std::stof(tokens[1]));
				normals.push_back(std::stof(tokens[2]));
				normals.push_back(std::stof(tokens[3]));
			} else if (tokens[0] == "usemtl") {
				std::string texture_name = "resources/" + tokens[1];
				texture_id = texture_manager->get_texture(texture_name);
			} else if (tokens[0] == "f") {
				current_num_vertices += 3;

				std::vector<std::string> v1 = split(tokens[1], '/');
				std::vector<std::string> v2 = split(tokens[2], '/');
				std::vector<std::string> v3 = split(tokens[3], '/');

				// get vertex position data
				unsigned int index1 = std::stoi(v1[0]) - 1;
				unsigned int index2 = std::stoi(v2[0]) - 1;
				unsigned int index3 = std::stoi(v3[0]) - 1;
				vertex_buffer.push_back(vertices[index1 * 3]);
				vertex_buffer.push_back(vertices[index1 * 3 + 1]);
				vertex_buffer.push_back(vertices[index1 * 3 + 2]);
				vertex_buffer.push_back(vertices[index2 * 3]);
				vertex_buffer.push_back(vertices[index2 * 3 + 1]);
				vertex_buffer.push_back(vertices[index2 * 3 + 2]);
				vertex_buffer.push_back(vertices[index3 * 3]);
				vertex_buffer.push_back(vertices[index3 * 3 + 1]);
				vertex_buffer.push_back(vertices[index3 * 3 + 2]);

				// get uv data
				index1 = std::stoi(v1[1]) - 1;
				index2 = std::stoi(v2[1]) - 1;
				index3 = std::stoi(v3[1]) - 1;
				uv_buffer.push_back(uvs[index1 * 2]);
				uv_buffer.push_back(uvs[index1 * 2 + 1]);
				uv_buffer.push_back(uvs[index2 * 2]);
				uv_buffer.push_back(uvs[index2 * 2 + 1]);
				uv_buffer.push_back(uvs[index3 * 2]);
				uv_buffer.push_back(uvs[index3 * 2 + 1]);
			}
		}

		GLuint vertex_buffer_id;
		GLuint uv_buffer_id;

		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_buffer.size(), &vertex_buffer[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uv_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uv_buffer.size(), &uv_buffer[0], GL_STATIC_DRAW);
		
		WavefrontOBJData* data = new WavefrontOBJData(
            file_path,
            vertex_buffer_id,
            uv_buffer_id,
            texture_id,
			current_num_vertices);
        obj_data_list.push_back(data);
        return data;
	} else {
		std::cout << "Load OBJ error " << file_path << std::endl;
		return nullptr;
	}
}

WavefrontOBJEntity* WavefrontOBJManager::create_instance(std::string file_path) {
    WavefrontOBJData* data = nullptr;
    // first check if the OBJ data already exists
    for (std::list<WavefrontOBJData*>::iterator it = obj_data_list.begin();
        it != obj_data_list.end(); it++) {
        if ((*it)->name == file_path) {
            data = *it;
            break;
        }
    }

    // otherwise load the OBJ data
    if (data == nullptr) {
        data = load_obj_data(file_path);
    }

    data->num_instances++;
    return new WavefrontOBJEntity(data);
}

void WavefrontOBJManager::remove_instance(WavefrontOBJEntity* entity) {
    // find the OBJ data
    for (std::list<WavefrontOBJData*>::iterator it = obj_data_list.begin();
        it != obj_data_list.end(); it++) {
        if ((*it)->name == entity->obj_data->name) {
            (*it)->num_instances--;
            break;
        }
    }

    delete entity;
}
