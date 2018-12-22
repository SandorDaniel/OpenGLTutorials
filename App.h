#pragma once

#include <GL/glew.h>

#include "VBO.hpp"
#include "VAO.hpp"

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>

class App
{
	VBO<glm::tvec3, float, glm::highp, 3> vbo;
	VAO vao;
	GLuint programID;

public:

	void Init();
	void Render() const;
};