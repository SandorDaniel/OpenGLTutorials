#include "VAO.hpp"


VAO::VAO(VAO&& vao) : m_vertexArrayID(vao.m_vertexArrayID), m_channel_number(vao.m_channel_number)
{
	vao.m_vertexArrayID = 0;
	vao.m_channel_number = 0;
}


VAO& VAO::operator=(VAO&& vao)
{
	if (this == &vao)
		return *this;

	glDeleteVertexArrays(1, &m_vertexArrayID);

	m_vertexArrayID = vao.m_vertexArrayID;
	m_channel_number = vao.m_channel_number;

	vao.m_vertexArrayID = 0;
	vao.m_channel_number = 0;

	return *this;
}


void VAO::Clear()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	m_vertexArrayID = 0;
	m_channel_number = 0;
}


void VAO::Enable() const
{
	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glEnableVertexAttribArray(i);
	}
}


void VAO::Disable() const
{
	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glDisableVertexAttribArray(i);
	}
}
