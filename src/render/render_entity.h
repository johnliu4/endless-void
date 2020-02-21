/**
 * A renderable entity.
 */

#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "camera.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

using glm::vec3;

class RenderEntity {
private:
    static unsigned long long int render_entity_id_counter;
    const unsigned long long int RENDER_ENTITY_ID;

    vec3 pos;
    vec3 rot_axis;
    vec3 scale;
    GLfloat rot_angle;

public:
    RenderEntity();

    // creates a model matrix from the position, rotation, and scale variables
    // and passes it to the shader
    virtual void load_model_matrix(Camera& camera);
    virtual void render(Camera& camera) = 0;

    vec3 get_pos();
    void set_pos(vec3 pos);
    void set_rot_axis(vec3 axis);
    void set_scale(vec3 scale);
    void set_rot_angle(GLfloat angle);

    unsigned long long int get_id();
};

#endif
