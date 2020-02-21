#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;

class Camera {
private:

    // shader variables
    GLuint program_id;
    GLuint proj_mat_id;
    GLuint view_mat_id;
    GLuint model_mat_id;
    mat4 proj_mat;
    mat4 view_mat;

    // camera position and orientation
    vec3 prev_pos;
    vec3 pos;
    GLfloat yaw_dir;
    GLfloat pitch_dir;
public:
    Camera();

    // uses the camera shader
    void use_program(GLfloat delta_time);

    // get the model matrix shader id
    GLuint get_model_matrix_id();

    void set_pos(vec3 pos);
    void set_yaw_dir(GLfloat yaw);
    void set_pitch_dir(GLfloat pitch);
};

#endif
