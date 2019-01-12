//#define sd_debugger
#include "Debug.h"

#include <map>
#include <set> // TODO: priority queue may be a beter choice, because different callbacks could have a cocncrete proper order of invocation.

#include <GLFW/glfw3.h>

#include "KeyBoard.h"



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