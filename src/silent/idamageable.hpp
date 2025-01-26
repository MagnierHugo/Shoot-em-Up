#pragma once

#include "Component.hpp"
#include "multicast_delegate.hpp"

namespace sl
{
	class IDamageable : public Component
	{
	public:
		IDamageable();
		void TakeDamage(const int damage);
		void HealDamage(const int heal);
		void SetMaxHealth(const int maxHealth);
		void Awake() override;

		int GetHealth() const;

		MulticastDelegate<> onDeath;
		MulticastDelegate<int> onHit;
	private:
		int health;
		int maxHealth;
	};
}