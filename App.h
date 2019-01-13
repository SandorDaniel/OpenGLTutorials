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
	GLFWwindow* window = nullptr;

	VAO m_vao;
	VBO<glm::tvec4, float, glm::highp, 4> m_vbo_pos;
	VBO<glm::tvec4, float, glm::highp, 4> m_vbo_nor;
	VBO<glm::tvec2, float, glm::highp, 2> m_vbo_tex;
	TEX m_tex;

	GLuint m_programID;

	glm::mat4 m_MVP;
	GLuint m_MVPID;

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
