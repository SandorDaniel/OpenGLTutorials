#include "XVAO.hpp"



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