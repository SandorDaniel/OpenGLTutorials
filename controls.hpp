#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(GLFWwindow*);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();


#include <map>
#include <iostream>

class KeyBoard
{

public:

	class Key
	{
		friend class KeyBoard;

		double m_time_duration_pressed = 0;
		double m_time_last_pressed = 0; // m_time_last_pressed == 0 <-> key is not being pressed

	public:

		void press() 
		{ 
			m_time_last_pressed = glfwGetTime(); 
		}

		void release()
		{ 
			m_time_duration_pressed += (glfwGetTime() - m_time_last_pressed);
			m_time_last_pressed = 0; 
		}

		double getTimePressed() const 
		{ 
			return m_time_duration_pressed + (m_time_last_pressed ? glfwGetTime() - m_time_last_pressed : 0);
		}

	};

private:

	static std::map<int, Key> active_keys;

public:

	static void event_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		active_keys[key];

		if (action == GLFW_PRESS)
		{
			active_keys[key].press();
		}

		if (action == GLFW_RELEASE)
		{
			active_keys[key].release();
		}
	}

	static const Key& getKey(const int GLFW_KEY_ID) { return active_keys[GLFW_KEY_ID]; }

	static void reSet() // Has to be called right before processing input events.
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

};


class Cursor
{
	static GLFWwindow* p_win;

	static double xpos;
	static double ypos;

	static void initCheck()
	{
		if (!p_win)
		{
			throw; // TODO
		}
	}

public:

	static void motion_callback(GLFWwindow* p_win, double xpos, double ypos)
	{
		Cursor::p_win = p_win;
		Cursor::xpos = xpos;
		Cursor::ypos = ypos;
	}

	static void reSetPosition()
	{
		initCheck();

		int width, height;
		glfwGetWindowSize(p_win, &width, &height);

		glfwSetCursorPos(p_win, width / 2, height / 2);

		xpos = width / 2;
		ypos = height / 2;
	}

	static double getXPos()
	{
		initCheck();

		return xpos;
	}

	static double getYPos()
	{
		initCheck();

		return ypos;
	}

};

#endif
