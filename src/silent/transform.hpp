#pragma once

#include "Component.hpp"
#include "Vector.hpp"
namespace sl
{
	class Transform : public Component
	{
	public:
		Transform() = default;
		Vector2 Position = Vector2::Zero;
		Vector2 Scale = Vector2::One;
		float Rotation = .0f;
	};
}