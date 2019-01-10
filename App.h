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
	VBO<glm::tvec4, float, glm::highp, 4> m_vbo_col;
	VBO<glm::tvec2, float, glm::highp, 2> m_vbo_tex;

	GLuint m_programID;

	glm::mat4 m_MVP;
	GLuint m_MVPID;

	TEX tex1;
	TEX tex2;

	Camera camera;

public:

	App(GLFWwindow* window) : window(window) {  }
	void Init();
	void Update();
	void Render() const;
	void Clean();
};
