// TODO: VAO-ban ellenõrizni, hogy minden bindolt vbo ugyanannyi elemmel bír.

#pragma once

#include <algorithm>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/detail/precision.hpp>

#include "VBO.hpp"
#include "Aspects.hpp"



class XVAO final
{

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

	friend void swap(VAO& v1, VAO& v2);

private:

	mutable TwoStatesManager m_attaching;
	mutable TwoStatesManager m_binding;

	VAO m_vao;

public:

	friend void swap(XVAO& v1, XVAO& v2);

	XVAO() = default;
	~XVAO() = default;

	XVAO(const XVAO&) = delete;
	XVAO& operator=(const XVAO&) = delete;

	XVAO(XVAO&& vao) :
		m_attaching(vao.m_attaching),
		m_binding(vao.m_binding),
		m_vao(std::move(vao.m_vao))
	{
	}
	XVAO& operator=(XVAO&& vao);

	operator GLuint() const
	{
		return (m_attaching.checkOn(static_cast<std::function<GLuint(const VAO&)>>(&VAO::operator GLuint)))(m_vao);
	}

	template<
		template<typename, glm::precision> class TVec,
		typename CoordType,
		glm::precision precision,
		const int COORD_COUNT>
		void attach(const XVBO<TVec, CoordType, precision, COORD_COUNT>& VBO)
	{
		return (m_attaching.turnOn(static_cast<std::function<void(VAO&, const XVBO<TVec, CoordType, precision, COORD_COUNT>&)>>(&VAO::attach<TVec, CoordType, precision, COORD_COUNT>)))(m_vao, VBO);
	}

	void bind() const// FEJL check if has attachment; treat as binding
	{
		std::function<void(const VAO&)> func = m_binding.turnOn(static_cast<std::function<void(const VAO&)>>(&VAO::bind));
		return (m_attaching.checkOn(func))(m_vao);
	}
	void unBind() const // FEJL check if it is bounded; treat as unbinding
	{
		std::function<void(const VAO&)> func = m_binding.turnOff(static_cast<std::function<void(const VAO&)>>(&VAO::unBind));
		return (m_binding.checkOn(func))(m_vao);
	}
};


template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
void XVAO::VAO::attach(const XVBO<TVec, CoordType, precision, COORD_COUNT>& VBO)
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


