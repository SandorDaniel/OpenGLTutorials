#pragma once

#include <GL/glew.h>

#include "VBO.hpp"
#include "VAO.hpp"

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/precision.hpp>

class App
{
	VAO m_vao;
	VBO<glm::tvec4, float, glm::highp, 4> m_vbo_pos;
	VBO<glm::tvec4, float, glm::highp, 4> m_vbo_col;
	GLuint m_programID;
	glm::mat4 m_MVP;
	GLuint m_MVPID;

public:

	void Init();
	void Update();
	void Render() const;
	void Clean();
};
