#pragma once

#include "VBO.hpp"
#include "Aspects.hpp"



template<
	template<typename, glm::precision> class TVec,
	typename CoordType,
	glm::precision precision,
	const int COORD_COUNT>
class XVBO final
{

public:

	class VBOIsNotLoaded : public std::exception
	{
		const char* what() const noexcept { return "You should have loaded the VBO object befor binding it to the VAO object."; }
	};

private:

	mutable TwoStatesManager m_loading;

	VBO<TVec, CoordType, precision, COORD_COUNT> m_vbo;

public:
	
	friend void swap(XVBO<TVec, CoordType, precision, COORD_COUNT>& t1, XVBO<TVec, CoordType, precision, COORD_COUNT>& t2)
	{
		using std::swap;

		swap(t1.m_loading, t2.m_loading);
		swap(t1.m_vbo, t2.m_vbo);
	}

	XVBO() = default;
	~XVBO() = default;

	XVBO(const XVBO&) = delete;
	XVBO& operator=(const XVBO&) = delete;

	XVBO(XVBO&& vbo) :
		m_loading(vbo.m_loading),
		m_vbo(std::move(vbo.m_vbo))
	{
	}
	XVBO& operator=(XVBO&& vbo);

	operator GLuint() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLuint(const VBO<TVec, CoordType, precision, COORD_COUNT>&)>>(&VBO<TVec, CoordType, precision, COORD_COUNT>::operator GLuint)))(m_vbo);
	}

	void load(const std::vector<TVec<CoordType, precision>>& g_vertex_buffer_data)
	{
		return (m_loading.turnOn(static_cast<std::function<void(VBO<TVec, CoordType, precision, COORD_COUNT>&, const std::vector<TVec<CoordType, precision>>&)>>(&VBO<TVec, CoordType, precision, COORD_COUNT>::load)))(m_vbo, g_vertex_buffer_data);
	}
	void unLoad()
	{
		return (m_loading.turnOff(static_cast<std::function<void(VBO<TVec, CoordType, precision, COORD_COUNT>&)>>(&VBO<TVec, CoordType, precision, COORD_COUNT>::unLoad)))(m_vbo);
	}

	GLsizei getElementCount() const
	{
		return (m_loading.checkOn(static_cast<std::function<GLsizei(const VBO<TVec, CoordType, precision, COORD_COUNT>&)>>(&VBO<TVec, CoordType, precision, COORD_COUNT>::getElementCount)))(m_vbo);
	}
	GLenum getCoordGLType() const
	{
		return m_vbo.getCoordGLType();
	}
};


template<template<typename, glm::precision> class TVec, typename CoordType, glm::precision precision, const int COORD_COUNT>
XVBO<TVec, CoordType, precision, COORD_COUNT>& XVBO<TVec, CoordType, precision, COORD_COUNT>::operator=(XVBO<TVec, CoordType, precision, COORD_COUNT>&& vbo)
{
	XVBO<TVec, CoordType, precision, COORD_COUNT> temp_vbo((std::move(vbo)));

	swap(*this, temp_vbo);

	return *this;
}