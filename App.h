#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "VBO.hpp"
#include "VAO.hpp"

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>

#include "TEX.h"
#include "Camera.h"



class App
{
	GLFWwindow* window = nullptr; // TODO: rename it properly

	VAO m_vao;
	VBO<glm::tvec4, float, glm::highp, 4> m_vbo_pos;
	VBO<glm::tvec4, float, glm::highp, 4> m_vbo_nor;
	VBO<glm::tvec2, float, glm::highp, 2> m_vbo_tex;
	TEX m_tex;

	GLuint m_programID;
	GLuint m_MVPID;

	glm::mat4 m_M = glm::mat4(); // glm::mat4() is the identity matrix

	Camera m_camera;

public:

	App(GLFWwindow* window) : window(window)
	{  
	}

	void init();
	void upDate();
	void render() const;
	void clean();
};
