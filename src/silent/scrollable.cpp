#include "scrollable.hpp"
#include "common.hpp"
#include "renderer.hpp"

void sl::Scrollable::Awake()
{
	renderable = Owner->GetComponent<Renderable>();
}

void sl::Scrollable::Update(const float deltaTime)
{
	Transform()->Position -= sl::Vector2::Right * ScrollingSpeed * deltaTime;
	float sprite_pos = Transform()->Position.x + renderable->GetWidth();

	if (sprite_pos <= 0)
	{
		if (DieWhenOut)
		{
			Owner->scene->RemoveEntity(Owner->name);
		}
		else
		{
			Transform()->Position.x = warp_x_pos + sprite_pos;
		}
	}
}
