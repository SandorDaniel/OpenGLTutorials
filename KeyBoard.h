#pragma once

#include <map>
#include <set> // TODO: priority queue may be a beter choice, because different callbacks could have a cocncrete proper order of invocation.

#include <GLFW/glfw3.h>


class KeyBoard
{

public:

	class Key
	{

		friend class KeyBoard;

	public:

		class Observer
		{
			
		public:

			virtual ~Observer() {  }

			virtual void pressCallBack() = 0;
			virtual void releaseCallBack() = 0;

		};

	private:

		std::set<Observer*> m_set_p_observers;

		double m_time_duration_pressed = 0;
		double m_time_last_pressed = 0; // m_time_last_pressed == 0 <-> key is not being pressed

	public:

		void regist(Observer& observer) { m_set_p_observers.insert(&observer); }
		void unRegist(Observer& observer)
		{
			auto it = m_set_p_observers.find(&observer);
			m_set_p_observers.erase(it);
		}

		void press();
		void release();

		double getTimePressed() const{ return m_time_duration_pressed; }

	};

private:

	static std::map<int, Key> active_keys;

public:

	static void press_or_release_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static Key& getKey(const int GLFW_KEY_ID) { return active_keys[GLFW_KEY_ID]; }

	static void reSet(); // Has to be called right before processing input events.

};
