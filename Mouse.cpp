//#define sd_debugger
#include "Debug.h"

#include "Mouse.h"


GLFWwindow* Cursor::p_win = nullptr;
double Cursor::xpos = 0;
double Cursor::ypos = 0;

std::set<Cursor::Observer*> Cursor::m_set_p_observer{};


std::set<ScrollBar::Observer*> ScrollBar::m_set_p_observer{};
