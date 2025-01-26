#pragma once

#include <functional>
#include <vector>

namespace sl
{
	template <typename... ParamsTypes>
	class MulticastDelegate
	{
	public:
		MulticastDelegate();
		MulticastDelegate(bool doDiscardUponTrigger_);
		void Add(std::function<void(ParamsTypes...)> function);
		void operator+=(std::function<void(ParamsTypes...)> function);
		void operator()(ParamsTypes... args);
		void Invoke(ParamsTypes... args);
	private:
		std::vector<std::function<void(ParamsTypes...)>> functions;
		bool doDiscardUponTrigger = false;
	};
}

#include "multicast_delegate.inl"
// if u see this add a smiley :) // ^^