#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>

#include "VBO.hpp"

#include <exception>


class VAO final
{

	static unsigned int ind;
	unsigned int m_ind;

public:

	class VBOIsNotLoaded : public std::exception
	{
		const char* what() const noexcept { return "You should have loaded the VBO object befor binding it to the VAO object."; }
	};

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

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(ind);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(
		ind,							// attribute 0. No particular reason for 0, but must match the layout in the shader.
		COORD_COUNT,								// size
		VBO.GetCoordGLType(),           // type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)0						// array buffer offset
	);

	glDisableVertexAttribArray(ind);

	m_ind = ind;
	++ind;
}