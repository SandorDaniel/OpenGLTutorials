#pragma once

#include <map>
#include <set> // TODO: priority queue may be a beter choice, because different callbacks could have a cocncrete proper order of invocation.

#include <GLFW/glfw3.h>



namespace InPut
{

	class KeyBoard final
	{

	public:

		class Key final
		{

		public:

			class Observer
			{

			public:

				virtual ~Observer() 
				{  
				}

				virtual void pressCallBack() = 0;
				virtual void releaseCallBack() = 0;

			};

		private:

			std::set<Observer*> m_set_of_p_observers{};

		public:

			void regist(Observer& observer) 
			{ 
				m_set_of_p_observers.insert(&observer);
			}
			void unRegist(Observer& observer)
			{
				auto it = m_set_of_p_observers.find(&observer);
				m_set_of_p_observers.erase(it);
			}

			void press();
			void release();

		};

	private:

		static std::map<int, Key> active_keys;

	public:

		static void press_or_release_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static Key& getKey(const int GLFW_KEY_ID)
		{ 
			return active_keys[GLFW_KEY_ID];
		}
		static const std::map<int, KeyBoard::Key>& getActiveKeys()
		{
			return active_keys;
		}

	};

};