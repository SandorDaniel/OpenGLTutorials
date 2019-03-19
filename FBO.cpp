#include "FBO.h"



void swap(FBO& t1, FBO& t2)
{
	using std::swap;

	swap(t1.m_id, t2.m_id);
	swap(t1.m_width, t2.m_width);
	swap(t1.m_height, t2.m_height);
	swap(t1.m_prev_x, t2.m_prev_x);
	swap(t1.m_prev_y, t2.m_prev_y);
	swap(t1.m_prev_width, t2.m_prev_width);
	swap(t1.m_prev_height, t2.m_prev_height);
}


FBO::FBO()
{
	glGenFramebuffers(1, &m_id);

	// Binding is neccesarry because generated names do not initially correspond to an instance of an object,
	// objects with generated names are created by binding a generated name to the context.
	// We need an instance of an object in order to use its name with a DSA function.
	GLint bound_fbo; // We want to live every state to be the same...
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bound_fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	glBindFramebuffer(GL_FRAMEBUFFER, bound_fbo);

	//// Instead of the above we can use this, if our OpenGL version number is at least 4.5:
	//void glCreateFramebuffers(1, &m_id);
}


FBO::FBO(FBO&& fbo) : m_id(fbo.m_id)
{
	fbo.m_id = 0;
	fbo.m_width = 0; // TODO: is this neccesarry?
	fbo.m_height = 0; // TODO: is this neccesarry?
	fbo.m_prev_x = 0; // TODO: is this neccesarry?
	fbo.m_prev_y = 0; // TODO: is this neccesarry?
	fbo.m_prev_width = 0; // TODO: is this neccesarry?
	fbo.m_prev_height = 0; // TODO: is this neccesarry?
}


FBO& FBO::operator=(FBO&& fbo)
{
	FBO temp_fbo((std::move(fbo)));

	swap(*this, temp_fbo);

	return *this;
}


void FBO::bind(GLenum target = GL_DRAW_FRAMEBUFFER) const
{
	if (target != GL_FRAMEBUFFER && target != GL_DRAW_FRAMEBUFFER && target != GL_READ_FRAMEBUFFER)
	{
		throw; // TODO
	}

	GLint p[4];
	glGetIntegerv(GL_VIEWPORT, p);
	m_prev_x = p[0];
	m_prev_y = p[1];
	m_prev_width = p[2];
	m_prev_height = p[3];
	glViewport(0, 0, m_width, m_height);

	glBindFramebuffer(target, m_id);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw; // TODO
	}

	if (target == GL_DRAW_FRAMEBUFFER || target == GL_FRAMEBUFFER)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}