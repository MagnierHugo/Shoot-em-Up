#pragma once

#include "Renderable.hpp"
#include "renderer.hpp"

namespace sl
{
	class Scrollable : public Component
	{
	public:
		float warp_x_pos = RenderSettings::ScreenWidth;
		float ScrollingSpeed = 500;
		Ref<Renderable> renderable = nullptr;
		bool DieWhenOut = false;
		void Awake() override;
		void Update(const float deltaTime) override;
	private:
	};
}