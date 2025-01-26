#include <iostream>

#include "IDamageable.hpp"
#include "Renderable.hpp"

sl::IDamageable::IDamageable()
{
	onDeath = MulticastDelegate<>(true);

	health = 0;
	maxHealth = 0;
}

void sl::IDamageable::TakeDamage(const int damage)
{
	health = std::max(0, health - damage);

	Ref<Renderable> renderablePtr = nullptr;
	if (Owner->TryGetComponent<Renderable>(renderablePtr))
	{
		renderablePtr->Flicker(1000);
	}
	onHit(health);

	if (health <= 0)
	{
		onDeath();
	}
}

void sl::IDamageable::HealDamage(const int heal) {
	health = std::min(maxHealth, health + heal);
}

void sl::IDamageable::SetMaxHealth(const int maxHealth_)
{
	maxHealth = maxHealth_;
}

void sl::IDamageable::Awake()
{
	health = maxHealth;
}

int sl::IDamageable::GetHealth() const {
	return health;
}
