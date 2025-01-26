#include "LightSource.hpp"
#include "scene.hpp"


namespace sl
{
	void LightSource::Awake()
	{
		Owner->scene->AddLightSource(std::shared_ptr<LightSource>(this));
		if (useRenderable)
		{
			renderable = Owner->GetComponent<Renderable>();
		}
	}

	sf::Glsl::Vec2 LightSource::GetData() // so far only position but could get the intensity too
	{
		return Transform()->Position + (useRenderable ? renderable->GetSize() / 2 : sl::Vector2::Zero);
	}
}

