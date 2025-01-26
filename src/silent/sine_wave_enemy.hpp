#pragma once

#include "active_shooter.hpp"

namespace sl
{
	class SineWaveEnemy : public ActiveShooter
	{
	public:
		void Awake() override;
		void Update(const float deltaTime) override;
	private:
		float sineWaveRange = 0;
		float sineWaveCounter = 0;
		float sineWaveSpeedModifier = 0;
		int basePositionY = 0;

	};
}