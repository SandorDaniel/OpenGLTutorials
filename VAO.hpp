#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>

#include "VBO.hpp"

#include <exception>


class VAO final
{

	GLuint m_vertexArrayID = 0; // TODO: ellenõrizni, hogy a OpenGL-ben a 0 valóban azt jelenti e, hogy a vertex array object még nem jött létre.

	static unsigned int ind;
	unsigned int m_ind = 0;

public:

	class VBOIsNotLoaded : public std::exception
	{
		const char* what() const noexcept { return "You should have loaded the VBO object befor binding it to the VAO object."; }
	};

	VAO() { glGenVertexArrays(1, &m_vertexArrayID); } // Generate a name for a new array.
	~VAO() { Clear(); }

	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;

	VAO(VAO&& vao);
	VAO& operator=(VAO&& vao);

	void Clear();

	template<
		template<typename, glm::precision> class TVec,
		typename CoordType,
		glm::precision precision,
		const int COORD_COUNT>
	void Bind(const VBO<TVec, CoordType, precision, COORD_COUNT>& VBO);

	void Enable() const { glEnableVertexAttribArray(m_ind); }

	void Disable() const { glDisableVertexAttribArray(m_ind); }

};


template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
void VAO::Bind(const VBO<TVec, CoordType, precision, COORD_COUNT>& VBO)
{
	if (!VBO.IsLoaded())
	{
		throw VBOIsNotLoaded();
	}

	glBindVertexArray(m_vertexArrayID); // Make the new array active, creating it if necessary.

	glEnableVertexAttribArray(ind);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		ind,							// attribute 0. No particular reason for 0, but must match the layout in the shader.
		COORD_COUNT,					// size
		VBO.GetCoordGLType(),           // type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)0						// array buffer offset
	);

	glDisableVertexAttribArray(ind);

	m_ind = ind;
	++ind;
}
