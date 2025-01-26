// time.cpp

#include "time.hpp"


namespace sl {

	sf::Clock Time::deltaClock;
	std::unordered_map<int, Timer> Time::timers;
	sf::Time Time::deltaTime;
	int Time::smallestFreeTimerID = 0;


	void Time::Tick() 
	{
		deltaTime = deltaClock.restart();

		for (auto& [timerID, timer] : timers)
		{
			if (timer.Clock.getElapsedTime().asMilliseconds() < timer.Duration) { continue; }

			timer.TimerCallback();
			timers.erase(timerID);
		}
	}

	float Time::GetDeltaTime()
	{
		return deltaTime.asSeconds();
	}

	sf::Time Time::GetRawDeltaTime()
	{
		return deltaTime;
	}

	int Time::AddTimer(const float durationInMS, std::function<void(void)> timer_callback) {
		timers[smallestFreeTimerID] = { sf::Clock(), durationInMS, timer_callback };

		return smallestFreeTimerID++;
	}

	void Time::RemoveTimer(const int timerID)
	{
		timers.erase(timerID);
	}
}