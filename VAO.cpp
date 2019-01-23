//#define sd_debugger
#include "Debug.h"

#include "VAO.hpp"



void swap(VAO& v1, VAO& v2)
{
	using std::swap;

	swap(v1.m_vertexArrayID, v2.m_vertexArrayID);
	swap(v1.m_channel_number, v2.m_channel_number);
}


VAO::VAO(VAO&& vao) : 
	m_vertexArrayID(vao.m_vertexArrayID),
	m_channel_number(vao.m_channel_number)
{
	vao.m_vertexArrayID = 0;
	vao.m_channel_number = 0;
}


VAO& VAO::operator=(VAO&& vao)
{
	VAO temp_vao((std::move(vao)));

	swap(*this, temp_vao);

	return *this;
}


void VAO::clear()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	m_vertexArrayID = 0;
	m_channel_number = 0;
}


void VAO::bind() const
{
	glBindVertexArray(m_vertexArrayID);

	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glEnableVertexAttribArray(i);
	}
}


void VAO::unBind() const
{
	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glDisableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}
