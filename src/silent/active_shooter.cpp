#pragma once

#include "active_shooter.hpp"

namespace sl
{
	int ActiveShooter::projectileCounter = 0;
	bool ActiveShooter::wasInit = false;

	void ActiveShooter::Awake()
	{
		if (wasInit) { return; }

		shootingSoundResource = AudioManager::CreateAudioResource("assets/music/shot.ogg", AudioType::Sound);

		for (int i = 0; i < audioSourcePoolSize; i++) // better safe than sorry
		{
		    AudioManager::CreateAudioSource("Projectile" + std::to_string(i))->SetAudioResource(shootingSoundResource);
		}

		wasInit = true;
	}

	void ActiveShooter::ShootBasicProjectile(Vector2& direction, b8 skip_timer)
	{
		Ref<Entity> new_projectile = Owner->scene->AddEntity("Projectile" + std::to_string(projectileCounter++));
		InitBasicProjectile(new_projectile, Owner, direction, Damage);

		PlayShootingSound();

		if (skip_timer) { return; }
		StartShootingCooldown();
		
	}

	void ActiveShooter::ShootHomingProjectile(Vector2& direction, b8 skip_timer)
	{
		Ref<Entity> new_projectile = Owner->scene->AddEntity("Projectile" + std::to_string(projectileCounter++));
		InitHomingProjectile(new_projectile, Owner, direction, Damage);

		PlayShootingSound();

		if (skip_timer) { return; }
		StartShootingCooldown();
	}

	void ActiveShooter::PlayShootingSound()
	{
		for (int iteration = 0; iteration < 100; iteration++) // this loop ensures that we only do <audioSourcePoolSize> loops
		{

		    currentAudioSourceIndex = ++currentAudioSourceIndex % audioSourcePoolSize; // while this index ensures that we continue where we left of and remain within the boundaries of the pool
		    Ref<AudioSource> currentAudioSource = AudioManager::GetAudioSource("Projectile" + std::to_string(currentAudioSourceIndex));
		    if (!currentAudioSource->IsPlaying())
		    {
		        currentAudioSource->Play(false, 20);
		        return;
		    }
		}

		// if reached this point there wasn t enough channels to play all the sounds concurrently so we allocate another one
		std::cout << "Added an audio source" << std::endl;
		AudioManager::CreateAudioSource("Projectile" + std::to_string(audioSourcePoolSize++))->SetAudioResource(shootingSoundResource)->Play(false, 15);
	}


	void ActiveShooter::InitBasicProjectile(Ref<Entity> projectile, Entity* shooter, Vector2& direction, int damage)
	{
		Ref<Renderable> renderable = projectile->AddComponent<Renderable>();
		// renderable->ShapeType = ShapeType::Sprite;
		// renderable->Texture = sl::ResourceManager::Get("Assets/...", ResourceType::Texture);

		renderable->SetModeRect(Vector2(10.0f, 10.0f), sf::Color::Green);

		Ref<Renderable> shooter_size = shooter->GetComponent<Renderable>();
		projectile->transform->Position = { shooter->transform->Position.x + (direction.x > 0 ? shooter_size->GetWidth() : -renderable->GetWidth()), shooter->transform->Position.y + shooter_size->GetHeight() / 2 };
		projectile->has_collider = true;

		Ref<BasicProjectile> projectile_behavior = projectile->AddComponent<BasicProjectile>();
		projectile_behavior->shooter = shooter;
		projectile_behavior->velocity = direction;
		projectile_behavior->renderable = renderable;
		projectile_behavior->Damage = damage;
	}

	void ActiveShooter::InitHomingProjectile(Ref<Entity> projectile, Entity* shooter, Vector2& direction, int damage)
	{
		Ref<Renderable> renderable = projectile->AddComponent<Renderable>();
		// renderable->ShapeType = ShapeType::Sprite;
		// renderable->Texture = sl::ResourceManager::Get("Assets/...", ResourceType::Texture);

		renderable->SetModeRect(Vector2(10.0f, 10.0f), sf::Color::Red);

		Ref<Renderable> shooter_size = shooter->GetComponent<Renderable>();
		projectile->transform->Position = { shooter->transform->Position.x + (direction.x > 0 ? shooter_size->GetWidth() : -renderable->GetWidth()), shooter->transform->Position.y + shooter_size->GetHeight() / 2 };
		projectile->has_collider = true;

		Ref<HomingProjectile> proj = projectile->AddComponent<HomingProjectile>();
		proj->shooter = shooter;
		proj->renderable = renderable;
		proj->Damage = damage;


	}
}