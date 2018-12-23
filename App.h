#pragma once

#include <GL/glew.h>

#include "VBO.hpp"
#include "VAO.hpp"

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>

class App
{
	VBO<glm::tvec4, float, glm::highp, 4> vbo;
	VAO vao;
	GLuint programID;
	glm::mat4 MVP;
	GLuint MVPID;

public:

	void Init();
	void Render() const;
};