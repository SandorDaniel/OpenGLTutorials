//#define sd_debugger
#include "Debug.h"

#include "Mouse.h"


namespace InPut
{

	std::set<Cursor::Observer*> Cursor::m_set_p_observer{};


	std::set<ScrollBar::Observer*> ScrollBar::m_set_p_observer{};

};
