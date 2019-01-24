// TODO: VAO-ban ellenõrizni, hogy minden bindolt vbo ugyanannyi elemmel bír.

#pragma once

#include <algorithm>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>

#include "VBO.hpp"
#include "IBO.hpp"
#include "Aspects.hpp"



class VAO final
{

	class AspFreeVAO final
	{

		GLuint m_vertexArrayID;
		unsigned int m_channel_number = 0;

	public:

		friend void swap(AspFreeVAO& v1, AspFreeVAO& v2);

		AspFreeVAO()
		{
			glGenVertexArrays(1, &m_vertexArrayID); // Generates a name for a new array.
		}
		~AspFreeVAO()
		{
			glDeleteVertexArrays(1, &m_vertexArrayID);
		}

		AspFreeVAO(const AspFreeVAO&) = delete;
		AspFreeVAO& operator=(const AspFreeVAO&) = delete;

		AspFreeVAO(AspFreeVAO&& vao);
		AspFreeVAO& operator=(AspFreeVAO&& vao);

		operator GLuint() const
		{
			return m_vertexArrayID;
		}

		template<
			template<typename, glm::precision> class TVec,
			typename CoordType,
			glm::precision precision,
			const int COORD_COUNT>
		void attach(const VBO<TVec, CoordType, precision, COORD_COUNT>& AspFreeVBO);
		template<
			typename ElementType>
		void attach(const IBO<ElementType>& AspFreeIBO);

		void bind() const;
		void unBind() const;

	};

	friend void swap(AspFreeVAO& v1, AspFreeVAO& v2);

private:

	mutable TwoStatesManager m_attaching;
	mutable TwoStatesManager m_binding;

	AspFreeVAO m_vao;

public:

	friend void swap(VAO& v1, VAO& v2);

	VAO() = default;
	~VAO() = default;

	VAO(const VAO&) = delete;
	VAO& operator=(const VAO&) = delete;

	VAO(VAO&& vao) :
		m_attaching(vao.m_attaching),
		m_binding(vao.m_binding),
		m_vao(std::move(vao.m_vao))
	{
	}
	VAO& operator=(VAO&& vao);

	operator GLuint() const
	{
		return (m_attaching.checkOn(static_cast<std::function<GLuint(const AspFreeVAO&)>>(&AspFreeVAO::operator GLuint)))(m_vao);
	}

	template<
		template<typename, glm::precision> class TVec,
		typename CoordType,
		glm::precision precision,
		const int COORD_COUNT>
	void attach(const VBO<TVec, CoordType, precision, COORD_COUNT>& AspFreeVBO)
	{
		return (m_attaching.turnOn(static_cast<std::function<void(AspFreeVAO&, const VBO<TVec, CoordType, precision, COORD_COUNT>&)>>(&AspFreeVAO::attach<TVec, CoordType, precision, COORD_COUNT>)))(m_vao, AspFreeVBO);
	}
	template<
		typename ElementType>
	void attach(const IBO<ElementType>& AspFreeIBO)
	{
		return m_vao.attach(AspFreeIBO);
	}

	void bind() const// FEJL check if has attachment; treat as binding
	{
		std::function<void(const AspFreeVAO&)> func = m_binding.turnOn(static_cast<std::function<void(const AspFreeVAO&)>>(&AspFreeVAO::bind));
		return (m_attaching.checkOn(func))(m_vao);
	}
	void unBind() const // FEJL check if it is bounded; treat as unbinding
	{
		std::function<void(const AspFreeVAO&)> func = m_binding.turnOff(static_cast<std::function<void(const AspFreeVAO&)>>(&AspFreeVAO::unBind));
		return (m_binding.checkOn(func))(m_vao);
	}
};


template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
void VAO::AspFreeVAO::attach(const VBO<TVec, CoordType, precision, COORD_COUNT>& AspFreeVBO)
{
	glBindVertexArray(m_vertexArrayID); // Make the new array active, creating it if necessary.

	glBindBuffer(GL_ARRAY_BUFFER, AspFreeVBO);

	glEnableVertexAttribArray(m_channel_number);

	glVertexAttribPointer(
		m_channel_number,				// attribute 0. No particular reason for 0, but must match the layout in the shader.
		COORD_COUNT,					// size
		AspFreeVBO.getCoordGLType(),           // type
		GL_FALSE,						// normalized?
		0,								// stride
		(void*)0						// array buffer offset
	);

	glDisableVertexAttribArray(m_channel_number);
	
	glBindVertexArray(0); // !!!VAO (container object) has to get unbound before VBO otherwise VBO would be detached of VAO

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	++m_channel_number;
}


template<
	typename ElementType>
void VAO::AspFreeVAO::attach(const IBO<ElementType>& AspFreeIBO)
{
	glBindVertexArray(m_vertexArrayID); // Make the new array active, creating it if necessary.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AspFreeIBO);

	glBindVertexArray(0); // !!!VAO (container object) has to get unbound before VBO otherwise VBO would be detached of VAO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
