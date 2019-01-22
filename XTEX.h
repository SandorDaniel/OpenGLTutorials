#pragma once

#include <queue>
#include <algorithm>
#include <functional>

#include <GL/glew.h>

#include "TEX.h"
#include "Aspects.hpp"



class X_TEX
{
	mutable StrictOGLObjectBindingManager m_soobm;
	TEX m_tex;

public:

	friend void swap(X_TEX& t1, X_TEX& t2);

	X_TEX() = default;
	~X_TEX() = default;

	X_TEX(const X_TEX&) = delete;
	X_TEX& operator=(const X_TEX&) = delete;

	X_TEX(X_TEX&& xtex) : m_soobm(xtex.m_soobm), m_tex(std::move(xtex.m_tex))
	{
	}
	X_TEX& operator=(X_TEX&& T);
	
	operator GLuint() const
	{
		return (m_soobm.checkConstReturningMethodForBinding(static_cast<std::function<GLuint(const TEX&)>>(&TEX::operator GLuint), m_tex))();
	}

	void loadBMP_custom(const char* const filepath) // TODO: a két függvényt regexpes estszétválasztással összevonni egybe 
	{
		return m_tex.loadBMP_custom(filepath);
	}
	void loadDDS(const char* const filepath)
	{
		return m_tex.loadDDS(filepath);
	}
	void unLoad()
	{
		m_tex.unLoad();
	}

	void bind() const
	{
		return (m_soobm.treatConstNonReturningMethodAsBinding(static_cast<std::function<void(const TEX&)>>(&TEX::bind), m_tex))();
	}
	void unBind() const
	{
		return (m_soobm.treatConstNonReturningMethodAsUnBinding(static_cast<std::function<void(const TEX&)>>(&TEX::unBind), m_tex))();
	}

	void Clean()
	{
		unBind();
		unLoad();
	}

};

