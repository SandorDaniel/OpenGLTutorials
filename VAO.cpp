//#define sd_debugger
#include "Debug.h"

#include "VAO.hpp"



void swap(XVAO::VAO& v1, XVAO::VAO& v2)
{
	using std::swap;

	swap(v1.m_vertexArrayID, v2.m_vertexArrayID);
	swap(v1.m_channel_number, v2.m_channel_number);
}


XVAO::VAO::VAO(XVAO::VAO&& vao) :
	m_vertexArrayID(vao.m_vertexArrayID),
	m_channel_number(vao.m_channel_number)
{
	vao.m_vertexArrayID = 0;
	vao.m_channel_number = 0;
}


XVAO::VAO& XVAO::VAO::operator=(XVAO::VAO&& vao)
{
	XVAO::VAO temp_vao((std::move(vao)));

	swap(*this, temp_vao);

	return *this;
}


void XVAO::VAO::bind() const
{
	glBindVertexArray(m_vertexArrayID);

	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glEnableVertexAttribArray(i);
	}
}


void XVAO::VAO::unBind() const
{
	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glDisableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}


void swap(XVAO& v1, XVAO& v2)
{
	using std::swap;

	swap(v1.m_attaching, v2.m_attaching);
	swap(v1.m_binding, v2.m_binding);
	swap(v1.m_vao, v2.m_vao);
}


XVAO& XVAO::operator=(XVAO&& vao)
{
	XVAO temp_vao((std::move(vao)));

	swap(*this, temp_vao);

	return *this;
}
