// TODO: VAO-ban ellenõrizni, hogy minden bindolt vbo ugyanannyi elemmel bír.

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>

#include "VBO.hpp"

#include <exception>



class VAO final
{

	GLuint m_vertexArrayID = 0; // TODO: ellenõrizni, hogy a OpenGL-ben a 0 valóban azt jelenti e, hogy a vertex array object még nem jött létre.

	unsigned int m_channel_number = 0;

public:

	class VBOIsNotLoaded : public std::exception
	{
		const char* what() const noexcept { return "You should have loaded the VBO object befor binding it to the VAO object."; }
	};

	VAO() 
	{ 
		glGenVertexArrays(1, &m_vertexArrayID);
	} // Generate a name for a new array.
	~VAO() 
	{ 
		clear();
	}

	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;

	VAO(VAO&& vao);
	VAO& operator=(VAO&& vao);

	void clear();

	template<
		template<typename, glm::precision> class TVec,
		typename CoordType,
		glm::precision precision,
		const int COORD_COUNT>
	void bind(const VBO<TVec, CoordType, precision, COORD_COUNT>& VBO);

	void enAble() const;
	void disAble() const;

};


template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
void VAO::bind(const VBO<TVec, CoordType, precision, COORD_COUNT>& VBO)
{
	if (!VBO.isLoaded())
	{
		throw VBOIsNotLoaded();
	}

	glBindVertexArray(m_vertexArrayID); // Make the new array active, creating it if necessary.
	glEnableVertexAttribArray(m_channel_number);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(
		m_channel_number,				// attribute 0. No particular reason for 0, but must match the layout in the shader.
		COORD_COUNT,					// size
		VBO.getCoordGLType(),           // type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)0						// array buffer offset
	);

	glDisableVertexAttribArray(m_channel_number);
	glBindVertexArray(0); // !!!VAO (container object) has to get unbound before VBO (regular object)!!! (???WHY???)

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	++m_channel_number;
}
