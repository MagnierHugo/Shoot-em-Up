#pragma once

#include <functional>
#include "Component.hpp"
#include "vector.hpp"
#include "scene.hpp"
#include "projectile.hpp"
#include "time.hpp"
#include "audio_manager.hpp"

namespace sl
{
	class ActiveShooter : public Component
	{
	public:
		void Awake() override;
		void ShootBasicProjectile(Vector2& direction, b8 skip_timer = false);
		void ShootHomingProjectile(Vector2& direction, b8 skip_timer = false);
		void PlayShootingSound();
		int Damage = 1;
	protected:
		float time_between_shots = 500; // NOTE(hugo): In milliseconds
		bool can_shoot = true;

		void StartShootingCooldown()
		{
			can_shoot = false;
			StartTimer(time_between_shots, [this]() { this->can_shoot = true; });
		}
		static void InitBasicProjectile(Ref<Entity> projectile, Entity* shooter, Vector2& direction, int damage);
		static void InitHomingProjectile(Ref<Entity> projectile, Entity* shooter, Vector2& direction, int damage);
		static int projectileCounter;

		static bool wasInit;
		int audioSourcePoolSize = 100; // better safe than sorry
		int currentAudioSourceIndex = -1;
		Ref<AudioResource> shootingSoundResource = nullptr;
	};
}