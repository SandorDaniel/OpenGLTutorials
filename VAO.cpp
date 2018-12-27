#include "VAO.hpp"


unsigned int VAO::ind = 1;


VAO::VAO(VAO&& vao) : m_vertexArrayID(vao.m_vertexArrayID), m_ind(vao.m_ind)
{
	vao.m_vertexArrayID = 0;
	vao.m_ind = 0;
}


VAO& VAO::operator=(VAO&& vao)
{
	if (this == &vao)
		return *this;

	glDeleteVertexArrays(1, &m_vertexArrayID);

	m_vertexArrayID = vao.m_vertexArrayID;
	m_ind = vao.m_ind;

	vao.m_vertexArrayID = 0;
	vao.m_ind = 0;

	return *this;
}


void VAO::Clear()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	m_vertexArrayID = 0;
	m_ind = 0;
}
