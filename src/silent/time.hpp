// time.h

#pragma once
#include <unordered_map>
#include <functional>
#include <iostream>
#include <memory>
#include <SFML/System/Clock.hpp>


namespace sl {

	struct Timer {
		sf::Clock Clock;
		float Duration; // in Milliseconds
		std::function<void(void)> TimerCallback;
	};


	class Time
	{
	public:
		static void Tick();
		static float GetDeltaTime();
		static sf::Time GetRawDeltaTime();
		static int AddTimer(const float durationInMS, std::function<void(void)> timerCallback);
		static void RemoveTimer(const int timerID);
	private:
		static sf::Time deltaTime;
		static std::unordered_map<int, Timer> timers;
		static sf::Clock deltaClock;
		static int smallestFreeTimerID;
	};

}