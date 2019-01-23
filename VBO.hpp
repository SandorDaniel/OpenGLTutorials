#pragma once

#include <vector>
#include <typeinfo>
#include <exception>
#include <limits>
#include <exception>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>



template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
class VBO final
{

private:

	GLuint m_id = 0;
	GLsizei m_element_count_of_vertexbuffer = 0;

public:

	friend void swap(VBO& v1, VBO& v2)
	{
		using std::swap;

		swap(v1.m_id, v2.m_id);
		swap(v1.m_element_count_of_vertexbuffer, v2.m_element_count_of_vertexbuffer);
	}

	VBO() = default;
	~VBO() 
	{ 
		unLoad();
	}

	VBO(const VBO&) = delete;
	VBO& operator=(const VBO&) = delete;

	VBO(VBO&& vbo);
	VBO& operator=(VBO&& vbo);

	operator GLuint() const 
	{
		return m_id; 
	}

	void load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data);
	void unLoad();

	GLsizei getElementCount() const
	{
		return m_element_count_of_vertexbuffer;
	}
	GLenum getCoordGLType() const
	{
		if(typeid(CoordType) == typeid(float))
		{
			return GL_FLOAT;
		}
		else
		{
			// TODO: ...
		}
	}
};


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
VBO<TVec, CoordType, precision, COORD_COUNT>::VBO(VBO<TVec, CoordType, precision, COORD_COUNT>&& vbo) :
	m_id(vbo.m_id),
	m_element_count_of_vertexbuffer(vbo.m_element_count_of_vertexbuffer)
{
	vbo.m_id = 0;
	m_element_count_of_vertexbuffer = 0;
}


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
VBO<TVec, CoordType, precision, COORD_COUNT>& VBO<TVec, CoordType, precision, COORD_COUNT>::operator=(VBO<TVec, CoordType, precision, COORD_COUNT>&& vbo)
{
	VBO temp_vbo((std::move(vbo)));

	swap(*this, temp_vbo);

	return *this;
}


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
void VBO<TVec, CoordType, precision, COORD_COUNT>::load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data)
{
	m_element_count_of_vertexbuffer = static_cast<GLsizei>(g_vertex_buffer_data.size() <= 2147483647 ? 
		g_vertex_buffer_data.size() : 
		throw std::domain_error("VBO.hpp: buffer element count is to big to be represented as a GLsizei"));

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, m_element_count_of_vertexbuffer * sizeof(TVec<CoordType, precision>), &g_vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
void VBO<TVec, CoordType, precision, COORD_COUNT>::unLoad()
{
	glDeleteBuffers(1, &m_id);
	m_id = 0;
	m_element_count_of_vertexbuffer = 0;
}
