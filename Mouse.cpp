//#define sd_debugger
#include "Debug.h"

#include <set> // TODO: priority queue may be a beter choice, because different callbacks could have a cocncrete proper order of invocation.

#include "Mouse.h"



namespace InPut
{

	std::set<Cursor::Observer*> Cursor::m_set_p_observer{};


	std::set<MouseButtons::Observer*> MouseButtons::m_set_p_observer{};


	std::set<ScrollBar::Observer*> ScrollBar::m_set_p_observer{};

};
