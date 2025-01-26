#include "Component.hpp"

namespace sl
{
	void Component::Enable()
	{
		Enabled = true;
	}

	void Component::Disable()
	{
		Enabled = false;
	}

	sl::Ref<Transform> Component::Transform() const
	{
		return Owner->transform;
	}

	void Component::SetOwner(sl::Entity* entity)
	{
		Owner = entity;
	}

	void Component::StartTimer(const float durationInMS, std::function<void(void)> callback)
	{
		Owner->StartTimer(durationInMS, callback);
	}
}
