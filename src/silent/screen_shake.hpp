#pragma once

#include "vector.hpp"

namespace sl
{
	class ScreenShake
	{
	public:
		ScreenShake(sl::Vector2 intensity, float durationInMS);
		bool HasEnded() const;
		sl::Vector2 Intensity;
	private:
		void Expire();
		bool inEffect;
	};
}