// TODO: multiple color attachments, renderbuffer object attachments (see the outcommented code below and http://www.songho.ca/opengl/gl_fbo.html)

//// create a renderbuffer object to store depth info
//GLuint rboId;
//glGenRenderbuffers(1, &rboId);
//glBindRenderbuffer(GL_RENDERBUFFER, rboId);
//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXT_WIDTH, TEXT_HEIGHT);
//glBindRenderbuffer(GL_RENDERBUFFER, 0);

//// attach the renderbuffer to depth attachment point
//glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
//	GL_DEPTH_ATTACHMENT, // 2. attachment point
//	GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
//	rboId);              // 4. rbo ID

// AFTER redering to texture:
//// trigger mipmaps generation explicitly
//// NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
//// triggers mipmap generation automatically. However, the texture attached
//// onto a FBO should generate mipmaps manually via glGenerateMipmap().
//glBindTexture(GL_TEXTURE_2D, textureId);
//glGenerateMipmap(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, 0);


// TODO: adding aspect to class FBO


#pragma once


#pragma once

#include <GL/glew.h>

#include "Aspects.hpp"
#include "TEX.h"



class FBO final
{

	GLuint m_id;

	GLint m_width;
	GLint m_height;

	mutable GLint m_prev_x = 0;
	mutable GLint m_prev_y = 0;
	mutable GLint m_prev_width = 0;
	mutable GLint m_prev_height = 0;

public:

	friend void swap(FBO& t1, FBO& t2);

	FBO();
	~FBO()
	{
		glDeleteFramebuffers(1, &m_id);
	}

	FBO(const FBO&) = delete;
	FBO& operator=(const FBO&) = delete;

	FBO(FBO&& fbo);
	FBO& operator=(FBO&& fbo);

	operator GLuint() const
	{
		return m_id;
	}

	void bind(GLenum target = GL_FRAMEBUFFER) const;
	void unBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(m_prev_x, m_prev_y, m_prev_width, m_prev_height);
	}

	template<class TexType>
	void attach(TEX<TexType>& tex)
	{
		GLint bound_fbo; // We want to live every state to be the same...
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bound_fbo);

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glFramebufferTexture2D(GL_FRAMEBUFFER, TexType::attachment_id, GL_TEXTURE_2D, tex, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, bound_fbo);

		m_width = tex.getWidth();
		m_height = tex.getHeight();
	}
	template<class TexType>
	void detach(TEX<TexType>& tex)
	{
		GLint bound_fbo; // We want to live every state to be the same...
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bound_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glFramebufferTexture2D(GL_FRAMEBUFFER, TexType::attachment_id, GL_TEXTURE_2D, 0, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, bound_fbo);
	}

};