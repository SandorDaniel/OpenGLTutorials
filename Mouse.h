#pragma once

#include <set> // TODO: priority queue may be a beter choice, because different callbacks could have a cocncrete proper order of invocation.

#include <GLFW/glfw3.h>


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

	//class Observer
	//{

	//public:

	//	virtual ~Observer() {  }

	//	virtual void pressCallBack() = 0;
	//	virtual void releaseCallBack() = 0;

	//};

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


class ScrollBar
{

public:

	class Observer
	{

	public:

		virtual ~Observer() {  }
		virtual void scrollCallBack(double yoffset) = 0;

	};

private:
	
	static std::set<Observer*> m_set_p_observer;

public:

	static void regist(Observer& observer) { m_set_p_observer.insert(&observer); }
	static void unRegist(Observer& observer)
	{
		auto it = m_set_p_observer.find(&observer);
		m_set_p_observer.erase(it);
	}

	static void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) // time independent method
	{
		for (auto it : m_set_p_observer)
		{
			it->scrollCallBack(yoffset);
		}
	}

};
