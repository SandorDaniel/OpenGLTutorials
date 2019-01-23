#pragma once

#include <functional>



class TwoStatesManager
{

	bool m_is_on = false;

public:

	template <typename MyClass, typename Ret, typename ...Args>
	std::function<Ret(MyClass&, Args...)> turnOn(std::function<Ret(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> Ret
		{
			Ret r = memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

			m_is_on = true;
			return static_cast<Ret&&>(r); // Reference collapsion rule again...
		};
	};
	template <typename MyClass, typename ...Args>
	std::function<void(MyClass&, Args...)> turnOn(std::function<void(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> void
		{
			memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

			m_is_on = true;
		};
	};

	template <typename MyClass, typename Ret, typename ...Args>
	std::function<Ret(MyClass&, Args...)> turnOff(std::function<Ret(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> Ret
		{
			if (m_is_on)
			{

				Ret r = memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule...

				m_is_on = false;
			}
			return static_cast<Ret&&>(r); // Reference collapsion rule again..
		};
	}; 
	template <typename MyClass, typename ...Args>
	std::function<void(MyClass&, Args...)> turnOff(std::function<void(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> void
		{
			if (m_is_on)
			{

				memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

				m_is_on = false;
			}
		};
	};

	template <typename MyClass, typename Ret, typename ...Args>
	std::function<Ret(MyClass&, Args...)> checkOn(std::function<Ret(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> Ret
		{
			if (!m_is_on)
			{
				throw;
			}

			Ret r = memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

			return static_cast<Ret&&>(r); // Reference collapsion rule again...
		};
	};
	template <typename MyClass, typename ...Args>
	std::function<void(MyClass&, Args...)> checkOn(std::function<void(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> void
		{
			if (!m_is_on)
			{
				throw;
			}

			memFunc(mc, std::forward<Args>(args)...);  // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule...

			m_is_on = true;
		};
	};

};
