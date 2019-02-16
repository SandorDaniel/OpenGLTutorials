//#define sd_debugger
#include "Debug.h"

#include "VAO.hpp"



void swap(VAO::AspFreeVAO& v1, VAO::AspFreeVAO& v2)
{
	using std::swap;

	swap(v1.m_vertexArrayID, v2.m_vertexArrayID);
	swap(v1.m_channel_number, v2.m_channel_number);
}


VAO::AspFreeVAO::AspFreeVAO()
{
	glGenVertexArrays(1, &m_vertexArrayID); // Generates a name for a new array.

	// Binding is neccesarry because generated names do not initially correspond to an instance of an object,
	// objects with generated names are created by binding a generated name to the context.
	// We need an instance of an object in order to use its name with a DSA function.
	GLint bound_vao; // We want to live every state to be the same...
	glGetIntegerv(GL_VERTEX_ARRAY_POINTER, &bound_vao); // TODO make casting more safety
	glBindVertexArray(m_vertexArrayID);
	glBindVertexArray(bound_vao);

	//// Instead of the above we can use this, if our OpenGL version number is at least 4.5:
	//glCreateVertexArrays(1, &m_vertexArrayID);
}


VAO::AspFreeVAO::AspFreeVAO(VAO::AspFreeVAO&& vao) :
	m_vertexArrayID(vao.m_vertexArrayID),
	m_channel_number(vao.m_channel_number)
{
	vao.m_vertexArrayID = 0;
	vao.m_channel_number = 0;
}


VAO::AspFreeVAO& VAO::AspFreeVAO::operator=(VAO::AspFreeVAO&& vao)
{
	VAO::AspFreeVAO temp_vao((std::move(vao)));

	swap(*this, temp_vao);

	return *this;
}


void VAO::AspFreeVAO::bind() const
{
	glBindVertexArray(m_vertexArrayID);

	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glEnableVertexAttribArray(i);
	}
}


void VAO::AspFreeVAO::unBind() const
{
	for (unsigned int i = 0; i < m_channel_number; ++i)
	{
		glDisableVertexAttribArray(i);
	}

	glBindVertexArray(0);
}


void swap(VAO& v1, VAO& v2)
{
	using std::swap;

	swap(v1.m_attaching, v2.m_attaching);
	swap(v1.m_binding, v2.m_binding);
	swap(v1.m_vao, v2.m_vao);
}


VAO& VAO::operator=(VAO&& vao)
{
	VAO temp_vao((std::move(vao)));

	swap(*this, temp_vao);

	return *this;
}
