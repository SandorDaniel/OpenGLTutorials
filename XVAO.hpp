#pragma once

#include "VAO.hpp"
#include "Aspects.hpp"



class XVAO final
{

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
		return (m_attaching.turnOn(static_cast<std::function<void(VAO&, const XVBO<TVec, CoordType, precision, COORD_COUNT>&)> >(&VAO::attach<TVec, CoordType, precision, COORD_COUNT>)))(m_vao, VBO);
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