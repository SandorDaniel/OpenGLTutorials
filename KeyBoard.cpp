//#define sd_debugger
#include "Debug.h"

#include "KeyBoard.h"

#include <map>

#include <GLFW/glfw3.h>


namespace InPut
{

	std::map<int, KeyBoard::Key> KeyBoard::active_keys{};


	void KeyBoard::Key::press()
	{
		for (auto it : m_set_p_observers)
		{
			it->pressCallBack();
		}
	}
	void KeyBoard::Key::release()
	{
		for (auto it : m_set_p_observers)
		{
			it->releaseCallBack();
		}
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

};