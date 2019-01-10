#include "KeyBoard.h"

#include <map>

#include <GLFW/glfw3.h>



std::map<int, KeyBoard::Key> KeyBoard::active_keys{};


void KeyBoard::Key::press()
{
	m_time_last_pressed = glfwGetTime();

	//for (auto it : m_set_p_observer)
	//{
	//	it->pressCallBack();
	//}
}


void KeyBoard::Key::release()
{
	m_time_duration_pressed += (glfwGetTime() - m_time_last_pressed);
	m_time_last_pressed = 0;

	//for (auto it : m_set_p_observer)
	//{
	//	it->releaseCallBack();
	//}
}


void KeyBoard::press_or_release_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		active_keys[key].press();
	}

	if (action == GLFW_RELEASE)
	{
		active_keys[key].release();
	}
}


void KeyBoard::reSet() // Has to be called right before processing input events.
{
	for (auto it = active_keys.begin(); it != active_keys.end(); ++it)
	{
		it->second.m_time_duration_pressed = 0;

		if (it->second.m_time_last_pressed)
		{
			it->second.release();
			it->second.press();
		}
	}
}
