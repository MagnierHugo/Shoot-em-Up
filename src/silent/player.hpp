#pragma once


#include "Renderable.hpp"
#include "active_shooter.hpp"


namespace sl {

	class PlayerMovement : public Component {
	public:
		Ref<Renderable> renderable;
		Vector2 bounds;
		
		void Awake() override;
		void Update(const float deltaTime) override;
	private:
		float speed = .0f;
		float width = .0f;
		float height = .0f;
	};

	class PlayerShooter : public ActiveShooter {
	public:
		void Awake() override;
		void Update(const float deltaTime) override;

		void BombBonus();
		void AttackSpeedBonus();

	private:
		
	};

	void InitPlayer(Ref<Entity> player);

}