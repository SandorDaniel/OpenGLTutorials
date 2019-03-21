#pragma once

#include <set> // TODO: priority queue may be a beter choice, because different callbacks could have a cocncrete proper order of invocation.

#include <GLFW/glfw3.h>



namespace InPut
{

	class Cursor final
	{

	public:

		class Observer
		{

		public:

			virtual ~Observer() {  }
			virtual void motionCallBack(GLFWwindow* p_win, double xpos, double ypos) = 0;

		};

	private:

		static std::set<Observer*> m_set_p_observer;

	public:

		static void regist(Observer& observer) 
		{ 
			m_set_p_observer.insert(&observer);
		}
		static void unRegist(Observer& observer)
		{
			auto it = m_set_p_observer.find(&observer);
			m_set_p_observer.erase(it);
		}

		static void motionCallback(GLFWwindow* p_win, double xpos, double ypos)
		{
			for (auto it : m_set_p_observer)
			{
				it->motionCallBack(p_win, xpos, ypos);
			}
		}

	};


	class MouseButtons final
	{

	public:

		class Observer
		{

		public:

			virtual ~Observer() {  }
			virtual void pressCallBack(GLFWwindow* p_win) = 0;
			virtual void releaseCallBack(GLFWwindow* p_win) = 0;

		};

	private:

		static std::set<Observer*> m_set_p_observer;

	public:

		static void regist(Observer& observer)
		{
			m_set_p_observer.insert(&observer);
		}
		static void unRegist(Observer& observer)
		{
			auto it = m_set_p_observer.find(&observer);
			m_set_p_observer.erase(it);
		}

		static void actionCallback(GLFWwindow* p_win, int button, int action, int mods)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				for (auto it : m_set_p_observer)
				{
					switch (action)
					{
					case GLFW_PRESS:
						it->pressCallBack(p_win);
						break;
					case GLFW_RELEASE:
						it->releaseCallBack(p_win);
						break;
					}
				}
			}
		}

	};


	class ScrollBar final
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

};
