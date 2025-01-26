#pragma once

#include "Component.hpp"
#include "Renderable.hpp"

namespace sl
{
	class LightSource : public Component
	{
	public:
		void Awake() override;
		sf::Glsl::Vec2 GetData();
	private:
		bool useRenderable = true;
		std::shared_ptr<Renderable> renderable = nullptr;
		float intensity = 0; // unused so far
	};
}
