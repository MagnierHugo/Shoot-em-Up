#pragma once

#include <functional>
#include <array>

#include "active_shooter.hpp"
#include "Renderable.hpp"
#include "common.hpp"
#include "renderer.hpp"

namespace sl
{
	class SomewhatSmartEnemy : public ActiveShooter
	{
	public:
		Vector2 roadBounds;
		
		void Awake() override;
		void Update(const float deltaTime) override;
	private:
		Ref<Entity> player;
		Ref<Renderable> playerRenderable;
		Ref<Renderable> renderable;
		float speed = 0;
		float width = 0;
		float height = 0;
		int forceYDirection = 0;
		const int yOffsetToPlayerLenieny = 20;
		bool GetState() const;


		std::array<std::function<void(const float)>, 2> states;

		int ammo = 0;
		int maxAmmo = 10;
		void UpdateOffensive(const float deltaTime);

		int timeToReload = 1500;
		bool isReloading = false;
		void UpdateDefensive(const float deltaTime);

		void HandleBounds();
	};
};

