#pragma once

#include "game_manager.hpp"


namespace sl
{
	GameManager* GameManager::instance = nullptr;
	Vector2 GameManager::bounds = Vector2::Zero;

	GameManager::GameManager()
	{
		instance = this;
	}

	void GameManager::Awake()
	{
		Ref<Entity> scoreCounter = Owner->CreateChildEntity("ScoreCounter");
		scoreCounterRenderablePtr = scoreCounter->AddComponent<Renderable>();
		scoreCounterRenderablePtr->SetModeText("0", ResourceManager::Get("assets/fonts/Roboto-Black.ttf", ResourceType::Font), 25, sf::Color::White);
		scoreCounterRenderablePtr->SetOrigin(true);

		scoreCounter->transform->Position = Vector2(395, 15);

		StartTimer(1000, [&]() { SpawnDumbEnemy(); });
		StartTimer(3000, [&]() { SpawnHurdle(); });
	}

	void GameManager::AddScore(int scoreToAdd)
	{
		score += scoreToAdd;

		scoreCounterRenderablePtr->SetText(std::to_string(score));

		if (score < scoreToSpawnSmarterEnemy)
		{
			StartTimer(1000, [&]() { SpawnDumbEnemy(); });
			return;
		}

		if (score < scoreToSpawnBoss)
		{
			if (rand() % 3 != 2)
			{
				StartTimer(1000, [&]() { SpawnSmartEnemy(); });
			}
			else
			{
				StartTimer(1000, [&]() { SpawnDumbEnemy(); });
			}
			return;
		}

		StartTimer(1000, [&]() { SpawnBoss(); });
	}

	void GameManager::SpawnHurdle()
	{
		Ref<Entity> hurdle = Owner->scene->AddEntity("Hurdle");

		int whatHurdle = rand() % 2; // 1 == fire_hydrant, 0 == trash_can

		Ref<IDamageable> iDamageablePtr = hurdle->AddComponent<IDamageable>();
		iDamageablePtr->SetMaxHealth(whatHurdle ? 3 : 2);
		iDamageablePtr->onDeath += [=]() { Owner->scene->RemoveEntity(hurdle); };
		iDamageablePtr->onDeath += [=]() {

			std::array<std::pair<std::string, std::function<void(Ref<Entity>, Ref<Entity>)>>, 3> collectibles = {
				{
					{"assets/collectibles/collectible_bombe.png", [](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<PlayerShooter>()->BombBonus();
						receiver->scene->RemoveEntity(sender);
					}},
					{"assets/collectibles/collectible_bouclier.png", [=](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<IDamageable>()->HealDamage(3 - currentDifficulty);
						for (int i = 1; i < receiver->GetComponent<IDamageable>()->GetHealth(); i++) {
							receiver->scene->GetEntity("Life Sprite " + std::to_string(i))->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/rose_vie.png", ResourceType::Texture));
						}
						receiver->scene->RemoveEntity(sender);
					}},
					{"assets/collectibles/collectible_doublecoup.png", [](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<PlayerShooter>()->AttackSpeedBonus();
						receiver->scene->RemoveEntity(sender);
					}}
				}
			};

			std::pair<std::string, std::function<void(Ref<Entity>, Ref<Entity>)>> current_collectible = collectibles[RandomInt(0, 2)];
			Ref<Entity> collectible = Owner->scene->AddEntity("Collectible");
			Ref<Renderable> renderablePtr = collectible->AddComponent<Renderable>();
			renderablePtr->SetModeSprite(ResourceManager::Get(current_collectible.first, ResourceType::Texture));
			collectible->transform->Position = hurdle->transform->Position;
			collectible->transform->Scale = Vector2(.2f, .2f);
			collectible->has_collider = true;
			collectible->collider_callback = current_collectible.second;
			{ auto _ = collectible->AddComponent<Scrollable>(); }
		};

		Ref<Scrollable> scrollablePtr = hurdle->AddComponent<Scrollable>();
		scrollablePtr->DieWhenOut = true;

		Ref<Renderable> renderablePtr = hurdle->AddComponent<Renderable>();
		renderablePtr->SetModeSprite(ResourceManager::Get(whatHurdle ? "assets/sprites/fire_hydrant.png" : "assets/sprites/trash_can.png", ResourceType::Texture));

		hurdle->transform->Scale = Vector2::One * .075f; // tiny
		hurdle->transform->Position = Vector2(
			(500 + RenderSettings::ScreenWidth),
			((RenderSettings::ScreenHeight - renderablePtr->GetHeight()) / 2 + (rand() % 300) - 150)
		);
		hurdle->has_collider = true;
		hurdle->collider_callback = [](Ref<Entity> sender, Ref<Entity> receiver)
			{
				if (receiver->name == "Player")
				{
					sender->scene->RemoveEntity(sender);
					receiver->GetComponent<IDamageable>()->TakeDamage(100);
				}
			};

		StartTimer(2500 + rand() % 5000, [&]() { SpawnHurdle();});
	}

	void GameManager::SpawnDumbEnemy()
	{
		Ref<Entity> enemy = Owner->scene->AddEntity("Naughty Boy");

		{ auto _ = enemy->AddComponent<SineWaveEnemy>(); }

		Ref<IDamageable> iDamageablePtr = enemy->AddComponent<IDamageable>();
		iDamageablePtr->SetMaxHealth(currentDifficulty == 0 ? 3 : currentDifficulty == 1 ? 5 : 7);
		iDamageablePtr->onDeath += [=]() { Owner->scene->RemoveEntity(enemy); };
		iDamageablePtr->onDeath += [=]() { Camera::ApplyScreenShake(15, 15, 150); };
		iDamageablePtr->onDeath += [=]() { AddScore(15);};
		iDamageablePtr->onDeath += [=]() {

			std::array<std::pair<std::string, std::function<void(Ref<Entity>, Ref<Entity>)>>, 3> collectibles = {
				{
					{"assets/collectibles/collectible_bombe.png", [](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<PlayerShooter>()->BombBonus();
						receiver->scene->RemoveEntity(sender);
					}},
					{"assets/collectibles/collectible_bouclier.png", [=](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<IDamageable>()->HealDamage(3 - currentDifficulty);
						for (int i = 1; i < receiver->GetComponent<IDamageable>()->GetHealth(); i++) {
							receiver->scene->GetEntity("Life Sprite " + std::to_string(i))->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/rose_vie.png", ResourceType::Texture));
						}
						receiver->scene->RemoveEntity(sender);
					}},
					{"assets/collectibles/collectible_doublecoup.png", [](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<PlayerShooter>()->AttackSpeedBonus();
						receiver->scene->RemoveEntity(sender);
					}}
				}
			};

			std::pair<std::string, std::function<void(Ref<Entity>, Ref<Entity>)>> current_collectible = collectibles[RandomInt(0, 2)];
			Ref<Entity> collectible = Owner->scene->AddEntity("Collectible");
			Ref<Renderable> renderablePtr = collectible->AddComponent<Renderable>();
			renderablePtr->SetModeSprite(ResourceManager::Get(current_collectible.first, ResourceType::Texture));
			collectible->transform->Position = enemy->transform->Position;
			collectible->transform->Scale = Vector2(.2f, .2f);
			collectible->has_collider = true;
			collectible->collider_callback = current_collectible.second;
			{ auto _ = collectible->AddComponent<Scrollable>(); }
		};

		Ref<Renderable> renderablePtr = enemy->AddComponent<Renderable>();
		renderablePtr->SetModeSprite(ResourceManager::Get("assets/sprites/ennemi1.png", ResourceType::Texture));

		enemy->transform->Scale = Vector2::One * .075f; // tiny
		enemy->transform->Position = RenderSettings::ScreenSize / 2 - renderablePtr->GetSize() / 2 + Vector2::Right * 200;
		enemy->has_collider = true;
	}

	void GameManager::SpawnSmartEnemy()
	{
		Ref<Entity> enemy = Owner->scene->AddEntity("Naughty Boy");

		auto enemy_movement = enemy->AddComponent<SomewhatSmartEnemy>();
		enemy_movement->roadBounds = bounds;

		Ref<IDamageable> iDamageablePtr = enemy->AddComponent<IDamageable>();
		iDamageablePtr->SetMaxHealth(currentDifficulty == 0 ? 5 : currentDifficulty == 1 ? 7 : 9);
		iDamageablePtr->onDeath += [=]() { Owner->scene->RemoveEntity(enemy); };
		iDamageablePtr->onDeath += [=]() { Camera::ApplyScreenShake(15, 15, 150); };
		iDamageablePtr->onDeath += [=]() { AddScore(15);};
		iDamageablePtr->onDeath += [=]() {

			std::array<std::pair<std::string, std::function<void(Ref<Entity>, Ref<Entity>)>>, 3> collectibles = {
				{
					{"assets/collectibles/collectible_bombe.png", [](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<PlayerShooter>()->BombBonus();
						receiver->scene->RemoveEntity(sender);
					}},
					{"assets/collectibles/collectible_bouclier.png", [=](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<IDamageable>()->HealDamage(3 - currentDifficulty);
						for (int i = 1; i < receiver->GetComponent<IDamageable>()->GetHealth(); i++) {
							receiver->scene->GetEntity("Life Sprite " + std::to_string(i))->GetComponent<Renderable>()->SetModeSprite(ResourceManager::Get("assets/UI-UX/UI/rose_vie.png", ResourceType::Texture));
						}
						receiver->scene->RemoveEntity(sender);
					}},
					{"assets/collectibles/collectible_doublecoup.png", [](Ref<Entity> sender, Ref<Entity> receiver) {
						if (receiver->name != "Player") { return; }
						receiver->GetComponent<PlayerShooter>()->AttackSpeedBonus();
						receiver->scene->RemoveEntity(sender);
					}}
				}
			};

			std::pair<std::string, std::function<void(Ref<Entity>, Ref<Entity>)>> current_collectible = collectibles[RandomInt(0, 2)];
			Ref<Entity> collectible = Owner->scene->AddEntity("Collectible");
			Ref<Renderable> renderablePtr = collectible->AddComponent<Renderable>();
			renderablePtr->SetModeSprite(ResourceManager::Get(current_collectible.first, ResourceType::Texture));
			collectible->transform->Position = enemy->transform->Position;
			collectible->transform->Scale = Vector2(.2f, .2f);
			collectible->has_collider = true;
			collectible->collider_callback = current_collectible.second;
			{ auto _ = collectible->AddComponent<Scrollable>(); }
			};

		Ref<Renderable> renderablePtr = enemy->AddComponent<Renderable>();
		renderablePtr->SetModeSprite(ResourceManager::Get("assets/sprites/ennemi2.png", ResourceType::Texture));

		enemy->transform->Scale = Vector2::One * .075f; // tiny
		enemy->transform->Position =
			RenderSettings::ScreenSize / 2 - renderablePtr->GetSize() / 2 // center
			+ Vector2::Right * 200;
		enemy->has_collider = true;
	}

	void GameManager::SpawnBoss()
	{
		Ref<Entity> boss = Owner->scene->AddEntity("Naughty Boy");
		InitBoss(boss, currentDifficulty);
		boss->GetComponent<BossMovement>()->bounds = bounds;
	}

	void GameManager::SetDifficulty(int newDifficulty)
	{
		instance->currentDifficulty = newDifficulty;
	}

	int GameManager::GetScore()
	{
		return instance->score;
	}
}