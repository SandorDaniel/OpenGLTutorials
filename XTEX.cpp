// #define sd_debugger
#include "Debug.h"

#include <utility>

#include "XTEX.h"



void swap(X_TEX& t1, X_TEX& t2)
{
	using std::swap;

	swap(t1.m_soobm, t2.m_soobm);
	swap(t1.m_tex, t2.m_tex);
}


X_TEX& X_TEX::operator=(X_TEX&& xtex)
{
	X_TEX temp_xtex((std::move(xtex)));

	swap(*this, temp_xtex);

	return *this;
}
