#pragma once

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
