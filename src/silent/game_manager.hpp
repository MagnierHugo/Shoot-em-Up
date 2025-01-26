#pragma once

#include "Component.hpp"
#include "renderable.hpp"
#include "camera.hpp"
#include "scrollable.hpp"
#include "player.hpp"
#include "time.hpp"
#include "scene.hpp"
#include "somewhat_smart_enemy.hpp"
#include "sine_wave_enemy.hpp"
#include "boss.hpp"
#include "idamageable.hpp"

namespace sl
{
	class GameManager : public Component {
	public:
		GameManager();
		void Awake() override;

		void AddScore(int scoreToAdd);

		void SpawnHurdle();
		void SpawnDumbEnemy();
		void SpawnSmartEnemy();
		void SpawnBoss();

		static void SetDifficulty(int newDifficulty);
		static int GetScore();
		static Vector2 bounds;
		
	private:
		int score = 0;
		const int scoreToSpawnSmarterEnemy = 30;
		const int scoreToSpawnBoss = 100;
		Ref<Renderable> scoreCounterRenderablePtr = nullptr;
		int currentDifficulty = 1;
		static GameManager* instance;
	};
}