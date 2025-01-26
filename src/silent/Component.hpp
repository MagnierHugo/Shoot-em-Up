#pragma once

#include <memory>
#include <functional>
#include "common.hpp"
#include "entity.hpp"

namespace  sl
{
	class Component
	{ 
		public:
			sl::Entity* Owner = nullptr;
			bool Enabled = false;
			virtual void Enable();
			virtual void Disable();
			virtual void Awake() { }
			virtual void Update(const float deltaTime) { }
			virtual void OnDestroy() { }
			sl::Ref<Transform> Transform() const;
			void SetOwner(sl::Entity* entity);
			void StartTimer(const float durationInMS, std::function<void(void)> callback);
	};	
}
