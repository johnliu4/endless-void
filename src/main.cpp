#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render_loader.h"

int main() {
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(640, 480, "Hello World!", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "GLEW failed to init" << std::endl;
		glfwTerminate();
		return -1;
	}

    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint default_program_id = glCreateProgram();
	GLuint vertex_id = load_shader("resources/default_vert.glsl", GL_VERTEX_SHADER);
	GLuint fragment_id = load_shader("resources/default_frag.glsl", GL_FRAGMENT_SHADER);
	glAttachShader(default_program_id, vertex_id);
	glAttachShader(default_program_id, fragment_id);
	glLinkProgram(default_program_id);
	glDetachShader(default_program_id, vertex_id);
	glDetachShader(default_program_id, fragment_id);
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

    glClearColor(0.1f, 0.3f, 1.0f, 1.0f);

	glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	glm::mat4 view_mat = glm::lookAt(
		glm::vec3(4.0f, 3.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model_mat, mvp;
	GLuint mat_id = glGetUniformLocation(default_program_id, "mvp");

	double before_time = glfwGetTime();
	double current_time, delta_time;

	double frames_timer = 0;
	int frames = 0;

	GLfloat rotation = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		current_time = glfwGetTime();
		delta_time = current_time - before_time;
		before_time = current_time;

		frames_timer += delta_time;
		frames += 1;
		if (frames_timer >= 1.0) {
			std::cout << frames << std::endl;
			frames = 0;
			frames_timer = 0;
		}

		rotation += glm::two_pi<GLfloat>() * delta_time;
		rotation = glm::mod(rotation, glm::two_pi<GLfloat>());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model_mat = glm::rotate(glm::mat4(1.0f), rotation, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
		mvp = proj_mat * view_mat * model_mat;

        glUseProgram(default_program_id);
		glUniformMatrix4fv(mat_id, 1, GL_FALSE, &mvp[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*) 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
