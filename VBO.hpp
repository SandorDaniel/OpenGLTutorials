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
class VBO final
{

	class AspFreeVBO final
	{

	private:

		GLuint m_vbo_id = 0;
		GLsizei m_element_count_of_vertexbuffer = 0;

	public:

		friend void swap(AspFreeVBO& v1, AspFreeVBO& v2)
		{
			using std::swap;

			swap(v1.m_vbo_id, v2.m_vbo_id);
			swap(v1.m_element_count_of_vertexbuffer, v2.m_element_count_of_vertexbuffer);
		}

		AspFreeVBO()
		{
			glGenBuffers(1, &m_vbo_id);

			// Binding is neccesarry because generated names do not initially correspond to an instance of an object,
			// objects with generated names are created by binding a generated name to the context.
			// We need an instance of an object in order to use its name with a DSA function.
			GLuint bound_vbo; // We want to live every state to be the same...
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, reinterpret_cast<GLint*>(&bound_vbo)); // TODO make casting more safety
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
			glBindBuffer(GL_ARRAY_BUFFER, bound_vbo);

			//// Instead of the above we can use this, if our OpenGL version number is at least 4.5:
			//glCreateBuffers(1, &m_vbo_id);
		}
		~AspFreeVBO()
		{
			glDeleteBuffers(1, &m_vbo_id);
		}

		AspFreeVBO(const AspFreeVBO&) = delete;
		AspFreeVBO& operator=(const AspFreeVBO&) = delete;

		AspFreeVBO(AspFreeVBO&& vbo);
		AspFreeVBO& operator=(AspFreeVBO&& vbo)
		{
			AspFreeVBO temp_vbo((std::move(vbo)));

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

	};

public:

	class VBOIsNotLoaded : public std::exception
	{
		const char* what() const noexcept { return "You should have loaded the VBO object befor binding it to the VAO object."; }
	};

private:

	mutable TwoStatesManager m_loading;

	AspFreeVBO m_vbo;

public:
	
	friend void swap(VBO<TVec, CoordType, precision, COORD_COUNT>& t1, VBO<TVec, CoordType, precision, COORD_COUNT>& t2)
	{
		using std::swap;

		swap(t1.m_loading, t2.m_loading);
		swap(t1.m_vbo, t2.m_vbo);
	}

	VBO() = default;
	~VBO() = default;

	VBO(const VBO&) = delete;
	VBO& operator=(const VBO&) = delete;

	VBO(VBO&& vbo) :
		m_loading(vbo.m_loading),
		m_vbo(std::move(vbo.m_vbo))
	{
	}
	VBO& operator=(VBO&& vbo);

	operator GLuint() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLuint(const VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO&)>>(&VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO::operator GLuint)))(m_vbo);
	}

	void load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data)
	{
		return (m_loading.turnOn(static_cast<std::function<void(VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO&, const std::vector<TVec<CoordType, precision>>&)>>(&VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO::load)))(m_vbo, g_vertex_buffer_data);
	}

	GLsizei getElementCount() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLsizei(const VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO&)>>(&VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO::getElementCount)))(m_vbo);
	}

};


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO::AspFreeVBO(VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO&& vbo) :
	m_vbo_id(vbo.m_vbo_id),
	m_element_count_of_vertexbuffer(vbo.m_element_count_of_vertexbuffer)
{
	vbo.m_vbo_id = 0;
	m_element_count_of_vertexbuffer = 0;
}


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
void VBO<TVec, CoordType, precision, COORD_COUNT>::AspFreeVBO::load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data)
{
	m_element_count_of_vertexbuffer = static_cast<GLsizei>(g_vertex_buffer_data.size() <= 2147483647 ? 
		g_vertex_buffer_data.size() : 
		throw std::domain_error("VBO.hpp: buffer element count is to big to be represented as a GLsizei"));

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
	glBufferData(GL_ARRAY_BUFFER, m_element_count_of_vertexbuffer * sizeof(TVec<CoordType, precision>), &g_vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
VBO<TVec, CoordType, precision, COORD_COUNT>& VBO<TVec, CoordType, precision, COORD_COUNT>::operator=(VBO<TVec, CoordType, precision, COORD_COUNT>&& vbo)
{
	VBO<TVec, CoordType, precision, COORD_COUNT> temp_vbo((std::move(vbo)));

	swap(*this, temp_vbo);

	return *this;
}