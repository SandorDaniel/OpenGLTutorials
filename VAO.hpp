// TODO: VAO-ban ellen�rizni, hogy minden bindolt vbo ugyanannyi elemmel b�r.

#pragma once

#include <algorithm>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>

#include "XVBO.hpp"



class VAO final
{

	GLuint m_vertexArrayID;
	unsigned int m_channel_number = 0;

public:

	friend void swap(VAO& v1, VAO& v2);

	VAO() 
	{ 
		glGenVertexArrays(1, &m_vertexArrayID); // Generates a name for a new array.
	}
	~VAO() 
	{
		glDeleteVertexArrays(1, &m_vertexArrayID);
	}

	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;

	VAO(VAO&& vao);
	VAO& operator=(VAO&& vao);

	operator GLuint() const
	{
		return m_vertexArrayID;
	}

	template<
		template<typename, glm::precision> class TVec,
		typename CoordType,
		glm::precision precision,
		const int COORD_COUNT>
	void attach(const XVBO<TVec, CoordType, precision, COORD_COUNT>& VBO);

	void bind() const;
	void unBind() const;

};


template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
void VAO::attach(const XVBO<TVec, CoordType, precision, COORD_COUNT>& VBO)
{
	glBindVertexArray(m_vertexArrayID); // Make the new array active, creating it if necessary.

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(m_channel_number);

	glVertexAttribPointer(
		m_channel_number,				// attribute 0. No particular reason for 0, but must match the layout in the shader.
		COORD_COUNT,					// size
		VBO.getCoordGLType(),           // type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)0						// array buffer offset
	);

	glDisableVertexAttribArray(m_channel_number);
	
	glBindVertexArray(0); // !!!VAO (container object) has to get unbound before VBO otherwise VBO would be detached of VAO

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	++m_channel_number;
}


