#pragma once

#include <functional>



class StrictOGLObjectBindingManager
{

	bool m_is_bound = false;

public:

	template <typename MyClass, typename Ret, typename ...Args>
	std::function<Ret(MyClass&, Args...)> treatMethodAsBinding(std::function<Ret(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> Ret
		{
			Ret r = memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

			m_is_bound = true;
			return static_cast<Ret&&>(r); // Reference collapsion rule again...
		};
	};
	template <typename MyClass, typename ...Args>
	std::function<void(MyClass&, Args...)> treatMethodAsBinding(std::function<void(MyClass&, Args...)> memFunc)
	{
		return [memFunc, this](MyClass& mc, Args... args) -> void
		{
			memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

			m_is_bound = true;
		};
	};

	template <typename MyClass, typename Ret, typename ...Args>
	std::function<Ret(MyClass&, Args...)> treatMethodAsUnBinding(std::function<Ret(MyClass&, Args...)> memFunc, MyClass& mc)
	{
		return [&mc, memFunc, this](MyClass& mc, Args... args) -> Ret
		{
			if (m_is_bound)
			{

				Ret r = memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule...

				m_is_bound = false;
			}
			return static_cast<Ret&&>(r); // Reference collapsion rule again..
		};
	}; 
	template <typename MyClass, typename ...Args>
	std::function<void(MyClass&, Args...)> treatMethodAsUnBinding(std::function<void(MyClass&, Args...)> memFunc, MyClass& mc)
	{
		return [&mc, memFunc, this](MyClass& mc, Args... args) -> void
		{
			if (m_is_bound)
			{

				memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

				m_is_bound = false;
			}
		};
	};

	template <typename MyClass, typename Ret, typename ...Args>
	std::function<Ret(MyClass&, Args...)> checkMethodForBinding(std::function<Ret(MyClass&, Args...)> memFunc, MyClass& mc)
	{
		return [&mc, memFunc, this](MyClass& mc, Args... args) -> Ret
		{
			if (!m_is_bound)
			{
				throw;
			}

			Ret r = memFunc(mc, std::forward<Args>(args)...); // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule... 

			return static_cast<Ret&&>(r); // Reference collapsion rule again...
		};
	};
	template <typename MyClass, typename ...Args>
	std::function<void(MyClass&, Args...)> checkMethodForBinding(std::function<void(MyClass&, Args...)> memFunc, MyClass& mc)
	{
		return [&mc, memFunc, this](MyClass& mc, Args... args) -> void
		{
			if (!m_is_bound)
			{
				throw;
			}

			memFunc(mc, std::forward<Args>(args)...);  // std::forward(t) is something like static_cast<T&&>(t) ie std::move(t). We use his for perfect forwarding. See reference-collapsion rule...

			m_is_bound = true;
		};
	};

};
