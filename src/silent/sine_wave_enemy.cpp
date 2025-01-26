#include "sine_wave_enemy.hpp"

void sl::SineWaveEnemy::Awake()
{
	ActiveShooter::Awake();
	basePositionY = Transform()->Position.y;

	time_between_shots = 800;
	sineWaveRange = 150;
	sineWaveCounter = 0;
	sineWaveSpeedModifier = 1.0f;
}


void sl::SineWaveEnemy::Update(const float deltaTime)
{
	if (can_shoot) { ShootBasicProjectile(sl::Vector2::Right * -500); }

	sineWaveCounter += sineWaveSpeedModifier * deltaTime;

	Transform()->Position.y = basePositionY + std::sin(sineWaveCounter) * sineWaveRange;
}