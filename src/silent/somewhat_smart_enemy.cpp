#pragma once

#include "somewhat_smart_enemy.hpp"

namespace sl
{
	void SomewhatSmartEnemy::Awake()
	{
		ActiveShooter::Awake();
		renderable = Owner->GetComponent<Renderable>();
		width = renderable->GetWidth();
		height = renderable->GetHeight();
		// roadBounds = { (float)RenderSettings::ScreenHeight / 5, 3 * (float)RenderSettings::ScreenHeight / 4 }; // trusting Hugo ^^ // well not anymore :D
		states[0] = std::bind(&SomewhatSmartEnemy::UpdateDefensive, this, std::placeholders::_1);
		states[1] = std::bind(&SomewhatSmartEnemy::UpdateOffensive, this, std::placeholders::_1);

		ammo = maxAmmo;
		time_between_shots = 300;

		player = Owner->scene->GetEntity("Player");
		playerRenderable = player->GetComponent<Renderable>();
		speed = 3;
	}

	void SomewhatSmartEnemy::Update(const float deltaTime)
	{
		states[(int)GetState()](deltaTime);

		HandleBounds();
	}

	bool SomewhatSmartEnemy::GetState() const
	{
		return ammo > 0; // true being offensive
	}

	void SomewhatSmartEnemy::UpdateOffensive(const float deltaTime)
	{
		int playerY = player->transform->Position.y + playerRenderable->GetHeight() / 2;
		int y = Transform()->Position.y + renderable->GetHeight() / 2;
		if (playerY > y + yOffsetToPlayerLenieny)
		{
			Transform()->Position.y += speed * 144 * deltaTime;
		}
		else if (playerY < y - yOffsetToPlayerLenieny)
		{
			Transform()->Position.y -= speed * 144 * deltaTime;
		}
		else
		{
			if (can_shoot && ammo-- > 0)
			{
				ShootBasicProjectile(sl::Vector2::Right * -800);
			}
		}
	}

	void SomewhatSmartEnemy::UpdateDefensive(const float deltaTime)
	{
		if (!isReloading)
		{
			isReloading = true;
			StartTimer(timeToReload,
				[&]() {
					ammo = maxAmmo;
					isReloading = false;
				}
			);
		}

		int playerY = player->transform->Position.y + playerRenderable->GetHeight() / 2;
		int y = Transform()->Position.y + renderable->GetHeight() / 2;
		if (forceYDirection != 0)
		{
			Transform()->Position.y += speed * forceYDirection * 144 * deltaTime;
		}
		else if (playerY > y)
		{
			Transform()->Position.y -= speed * 144 * deltaTime;

			if (Transform()->Position.y < roadBounds.x + 50)
			{
				forceYDirection = 1;
				StartTimer(400, [&]() { forceYDirection = 0;});
			}
		}
		else if (playerY < y)
		{
			Transform()->Position.y += speed * 144 * deltaTime;

			if (Transform()->Position.y + height > roadBounds.y - 50)
			{
				forceYDirection = -1;
				StartTimer(400, [&]() { forceYDirection = 0;});
			}
		}
	}

	void SomewhatSmartEnemy::HandleBounds()
	{
		float currentX = Transform()->Position.x;
		Transform()->Position.x = currentX < .0f ? .0f : currentX + width > 1280.0f ? 1280.0f - width : currentX;
	
	
		float currentY = Transform()->Position.y;
		Transform()->Position.y = currentY < roadBounds.x ? roadBounds.x : currentY + height > roadBounds.y ? roadBounds.y - height : currentY;
	}
};

