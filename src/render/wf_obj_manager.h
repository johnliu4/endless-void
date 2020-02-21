/**
 * Manager and loader for Wavefront OBJ files.
 */

#ifndef WF_OBJ_MANAGER_H
#define WF_OBJ_MANAGER_H

#include "render_entity.h"
#include <glad/glad.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>

/**
 * Contains information about a particular Wavefront OBJ.
 * WavefrontOBJData can be used to generate instances of WavefrontOBJ.
 */
class WavefrontOBJData {
public:
    std::string name;
    GLuint vertex_buffer_id;
    GLuint uv_buffer_id;
    GLuint texture_id;
    GLuint num_vertices;
    // number of active instances of this particular OBJ
    // instances are created and removed through the manager
    unsigned int num_instances;
    WavefrontOBJData(std::string name, GLuint vertex_buffer_id,
        GLuint uv_buffer_id, GLuint texture_id, GLuint num_vertices);
};

/* An instance of a Wavefront OBJ as a RenderEntity. */
class WavefrontOBJEntity : public RenderEntity {
public:
    const WavefrontOBJData* obj_data;
    WavefrontOBJEntity(WavefrontOBJData* obj_data);
    ~WavefrontOBJEntity();
    void render(Camera& camera);
};

/**
 * Manager for loading Wavefront OBJ files and generating instances
 * of Wavefront OBJ RenderEntity's.
 */
class WavefrontOBJManager {
private:
    std::list<WavefrontOBJData*> obj_data_list;

    /**
     * Loads a Wavefront OBJ file into memory.
     * A very simple implementation is used here.
     * usemtl should indicate the name of the image to be used.
     * mtl files and mtllib are not used.
     * Only triangular faces are supported.
     */
    WavefrontOBJData* load_obj_data(std::string name);
public:
    WavefrontOBJManager();
    ~WavefrontOBJManager();

    WavefrontOBJEntity* create_instance(std::string file_path);
    void remove_instance(WavefrontOBJEntity* entity);
};

#endif
