#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>

#include "VBO.hpp"
#include "VAO.hpp"
#include "IBO.hpp"
#include "TEX.h"

#include "Camera.h"



class App
{
	GLFWwindow* window = nullptr; // TODO: rename it properly

	VAO m_vao;
	VBO<glm::tvec3, float, glm::highp, 3> m_vbo_pos;
	VBO<glm::tvec3, float, glm::highp, 3> m_vbo_nor;
	VBO<glm::tvec2, float, glm::highp, 2> m_vbo_tex;
	IBO<GLushort> m_ibo;
	
	TEX m_tex;

	GLuint m_programID;
	GLuint m_MID;
	GLuint m_VID;
	GLuint m_PID;
	GLuint m_cam_posID;
	GLuint m_textureID;

	glm::mat4 m_M = glm::mat4(); // glm::mat4() is the identity matrix
	glm::mat4 m_M2 = glm::mat4();

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
