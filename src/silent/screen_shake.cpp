#include <functional>

#include "screen_shake.hpp"
#include "time.hpp"

sl::ScreenShake::ScreenShake(sl::Vector2 intensity, float durationInMS)
{
	Intensity = intensity;
	inEffect = true;
	Time::AddTimer(durationInMS, std::bind(&ScreenShake::Expire, this));
}

bool sl::ScreenShake::HasEnded() const
{
	return !inEffect;
}

void sl::ScreenShake::Expire()
{
	inEffect = false;
}
