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



template<typename ElementType>
class IBO final
{

	class AspFreeIBO final
	{

	private:

		GLuint m_ibo_id = 0;
		GLsizei m_element_count_of_indexbuffer = 0;

	public:

		friend void swap(AspFreeIBO& v1, AspFreeIBO& v2)
		{
			using std::swap;

			swap(v1.m_ibo_id, v2.m_ibo_id);
			swap(v1.m_element_count_of_indexbuffer, v2.m_element_count_of_indexbuffer);
		}

		AspFreeIBO();
		~AspFreeIBO()
		{
			glDeleteBuffers(1, &m_ibo_id);
		}

		AspFreeIBO(const AspFreeIBO&) = delete;
		AspFreeIBO& operator=(const AspFreeIBO&) = delete;

		AspFreeIBO(AspFreeIBO&& vbo);
		AspFreeIBO& operator=(AspFreeIBO&& vbo)
		{
			AspFreeIBO temp_vbo((std::move(vbo)));

			swap(*this, temp_vbo);

			return *this;
		}

		operator GLuint() const
		{
			return m_ibo_id;
		}

		void load(const std::vector<ElementType>& g_vertex_buffer_data);

		GLsizei getElementCount() const
		{
			return m_element_count_of_indexbuffer;
		}

	};

public:

	class IBOIsNotLoaded final : public std::exception
	{
		const char* what() const noexcept { return "You should have loaded the IBO object befor binding it to the VAO object."; }
	};

private:

	mutable TwoStatesManager m_loading;

	AspFreeIBO m_ibo;

public:

	friend void swap(IBO<ElementType>& t1, IBO<ElementType>& t2)
	{
		using std::swap;

		swap(t1.m_loading, t2.m_loading);
		swap(t1.m_ibo, t2.m_ibo);
	}

	IBO() = default;
	~IBO() = default;

	IBO(const IBO&) = delete;
	IBO& operator=(const IBO&) = delete;

	IBO(IBO&& vbo) :
		m_loading(vbo.m_loading),
		m_ibo(std::move(vbo.m_ibo))
	{
	}
	IBO& operator=(IBO&& vbo);

	operator GLuint() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLuint(const IBO<ElementType>::AspFreeIBO&)>>(&IBO<ElementType>::AspFreeIBO::operator GLuint)))(m_ibo);
	}

	void load(const std::vector<ElementType>& g_vertex_buffer_data)
	{
		return (m_loading.turnOn(static_cast<std::function<void(IBO<ElementType>::AspFreeIBO&, const std::vector<ElementType>&)>>(&IBO<ElementType>::AspFreeIBO::load)))(m_ibo, g_vertex_buffer_data);
	}

	GLsizei getElementCount() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLsizei(const IBO<ElementType>::AspFreeIBO&)>>(&IBO<ElementType>::AspFreeIBO::getElementCount)))(m_ibo);
	}

};


template<typename ElementType>
IBO<ElementType>::AspFreeIBO::AspFreeIBO()
{
	glGenBuffers(1, &m_ibo_id);

	// Binding is neccesarry because generated names do not initially correspond to an instance of an object,
	// objects with generated names are created by binding a generated name to the context.
	// We need an instance of an object in order to use its name with a DSA function.
	GLint bound_ibo; // We want to live every state to be the same...
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound_ibo); // TODO make casting more safety
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bound_ibo);

	//// Instead of the above we can use this, if our OpenGL version number is at least 4.5:
	//glCreateBuffers(1, &m_ibo_id);
}


template<typename ElementType>
IBO<ElementType>::AspFreeIBO::AspFreeIBO(IBO<ElementType>::AspFreeIBO&& vbo) :
	m_ibo_id(vbo.m_ibo_id),
	m_element_count_of_indexbuffer(vbo.m_element_count_of_indexbuffer)
{
	vbo.m_ibo_id = 0;
	m_element_count_of_indexbuffer = 0;
}


template<typename ElementType>
void IBO<ElementType>::AspFreeIBO::load(const std::vector<ElementType>& g_index_buffer_data)
{
	m_element_count_of_indexbuffer = static_cast<GLsizei>(g_index_buffer_data.size() <= 2147483647 ?
		g_index_buffer_data.size() :
		throw std::domain_error("IBO.hpp: buffer element count is to big to be represented as a GLsizei"));

	glNamedBufferData(m_ibo_id, m_element_count_of_indexbuffer * sizeof(ElementType), &g_index_buffer_data[0], GL_STATIC_DRAW);
}


template<typename ElementType>
IBO<ElementType>& IBO<ElementType>::operator=(IBO<ElementType>&& vbo)
{
	IBO<ElementType> temp_vbo((std::move(vbo)));

	swap(*this, temp_vbo);

	return *this;
}