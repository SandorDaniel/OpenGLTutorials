#pragma once

#include <vector>
#include <typeinfo>
#include <exception>
#include <limits>
#include <exception>
#include <algorithm>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>


#include "Aspects.hpp"



template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
class XVBO final
{

	class VBO final
	{

	private:

		GLuint m_vbo_id = 0;
		GLsizei m_element_count_of_vertexbuffer = 0;

	public:

		friend void swap(VBO& v1, VBO& v2)
		{
			using std::swap;

			swap(v1.m_vbo_id, v2.m_vbo_id);
			swap(v1.m_element_count_of_vertexbuffer, v2.m_element_count_of_vertexbuffer);
		}

		VBO()
		{
			glGenBuffers(1, &m_vbo_id);
		}
		~VBO()
		{
			glDeleteBuffers(1, &m_vbo_id);
		}

		VBO(const VBO&) = delete;
		VBO& operator=(const VBO&) = delete;

		VBO(VBO&& vbo);
		VBO& operator=(VBO&& vbo)
		{
			VBO temp_vbo((std::move(vbo)));

			swap(*this, temp_vbo);

			return *this;
		}

		operator GLuint() const
		{
			return m_vbo_id;
		}

		void load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data);

		GLsizei getElementCount() const
		{
			return m_element_count_of_vertexbuffer;
		}
		GLenum getCoordGLType() const
		{
			if (typeid(CoordType) == typeid(float))
			{
				return GL_FLOAT;
			}
			else
			{
				// TODO: ...
			}
		}

	};

public:

	class VBOIsNotLoaded : public std::exception
	{
		const char* what() const noexcept { return "You should have loaded the VBO object befor binding it to the VAO object."; }
	};

private:

	mutable TwoStatesManager m_loading;

	VBO m_vbo;

public:
	
	friend void swap(XVBO<TVec, CoordType, precision, COORD_COUNT>& t1, XVBO<TVec, CoordType, precision, COORD_COUNT>& t2)
	{
		using std::swap;

		swap(t1.m_loading, t2.m_loading);
		swap(t1.m_vbo, t2.m_vbo);
	}

	XVBO() = default;
	~XVBO() = default;

	XVBO(const XVBO&) = delete;
	XVBO& operator=(const XVBO&) = delete;

	XVBO(XVBO&& vbo) :
		m_loading(vbo.m_loading),
		m_vbo(std::move(vbo.m_vbo))
	{
	}
	XVBO& operator=(XVBO&& vbo);

	operator GLuint() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLuint(const XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO&)>>(&XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO::operator GLuint)))(m_vbo);
	}

	void load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data)
	{
		return (m_loading.turnOn(static_cast<std::function<void(XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO&, const std::vector<TVec<CoordType, precision>>&)>>(&XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO::load)))(m_vbo, g_vertex_buffer_data);
	}

	GLsizei getElementCount() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLsizei(const XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO&)>>(&XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO::getElementCount)))(m_vbo);
	}
	GLenum getCoordGLType() const
	{
		return m_vbo.getCoordGLType();
	}
};


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO::VBO(XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO&& vbo) :
	m_vbo_id(vbo.m_vbo_id),
	m_element_count_of_vertexbuffer(vbo.m_element_count_of_vertexbuffer)
{
	vbo.m_vbo_id = 0;
	m_element_count_of_vertexbuffer = 0;
}


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
void XVBO<TVec, CoordType, precision, COORD_COUNT>::VBO::load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data)
{
	m_element_count_of_vertexbuffer = static_cast<GLsizei>(g_vertex_buffer_data.size() <= 2147483647 ? 
		g_vertex_buffer_data.size() : 
		throw std::domain_error("VBO.hpp: buffer element count is to big to be represented as a GLsizei"));

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
	glBufferData(GL_ARRAY_BUFFER, m_element_count_of_vertexbuffer * sizeof(TVec<CoordType, precision>), &g_vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
XVBO<TVec, CoordType, precision, COORD_COUNT>& XVBO<TVec, CoordType, precision, COORD_COUNT>::operator=(XVBO<TVec, CoordType, precision, COORD_COUNT>&& vbo)
{
	XVBO<TVec, CoordType, precision, COORD_COUNT> temp_vbo((std::move(vbo)));

	swap(*this, temp_vbo);

	return *this;
}