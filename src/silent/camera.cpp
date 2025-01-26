#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include "Camera.hpp"
#include "screen_shake.hpp"
#include "input_manager.hpp"
#include "transform.hpp"
#include "renderer.hpp"

namespace sl
{
	Camera* Camera::instance = nullptr;

	Camera::Camera()
	{
		instance = this;
	}

	Camera::~Camera()
	{
		ResetScreenShake();
	}


	void Camera::Awake()
	{
		Transform()->Position = RenderSettings::ScreenSize / 2;

		ViewSize = RenderSettings::ScreenSize;
		ZoomFactor = 1.0f;
		ViewLookAt = Transform()->Position;
	}

	void Camera::Update(const float)
	{
#ifdef TEST_SHAKE
		ImGui::Begin("Screen Shake Settings");
		ImGui::SliderInt("Screen Shake Intensity X", &shakeIntensityX, 0, 50, "%d px", ImGuiSliderFlags_::ImGuiSliderFlags_None);
		//shakeIntensityX = (shakeIntensityX / 5) * 5; // round to the nearest 5
		ImGui::SliderInt("Screen Shake Intensity Y", &shakeIntensityY, 0, 50, "%d px", ImGuiSliderFlags_::ImGuiSliderFlags_None);
		//shakeIntensityY = (shakeIntensityY / 5) * 5; // round to the nearest 5
		ImGui::SliderInt("Screen Shake Duration", &shakeDuration, 5, 3000, "%d ms", ImGuiSliderFlags_::ImGuiSliderFlags_None);
		//shakeDuration = (shakeDuration / 5) * 5; // round to the nearest 5
		ImGui::End();

		if (sl::InputManager::GetKeyDown(sf::Keyboard::Key::A))
		{
			ResetScreenShake();
			ApplyScreenShake(shakeIntensityX, shakeIntensityY, shakeDuration);
		}
#endif // TEST_SHAKE


		ViewLookAt = Transform()->Position;

		if (screenShakeValues.empty()) { return; }

		sl::Vector2 maxIntensity;

		for (int i = screenShakeValues.size() - 1; i > -1; i--)
		{
			if (screenShakeValues[i]->HasEnded())
			{
				delete screenShakeValues[i];
				screenShakeValues.erase(screenShakeValues.begin() + i);
				continue;
			}

			maxIntensity.x = screenShakeValues[i]->Intensity.x > maxIntensity.x ? screenShakeValues[i]->Intensity.x : maxIntensity.x;
			maxIntensity.y = screenShakeValues[i]->Intensity.y > maxIntensity.y ? screenShakeValues[i]->Intensity.y : maxIntensity.y;
		}

		ViewLookAt += sf::Vector2f(
			rand() % (int)(2.0f * maxIntensity.x + 1) - maxIntensity.x,
			rand() % (int)(2.0f * maxIntensity.y + 1) - maxIntensity.y
		); // Note(Antoine): either do some sort of max delta so it doesn t jitter too much or just don t be a retard giving an input of (maxInt, maxInt) (imma do it tho ^^)
	}


	void Camera::ApplyScreenShake(const sl::Vector2& intensity, float durationInMS)
	{
		instance->screenShakeValues.push_back(
			new ScreenShake(intensity, durationInMS)
		);
	}
	void Camera::ApplyScreenShake(const float xIntensity, const float yIntensity, float durationInMS)
	{
		ApplyScreenShake(sl::Vector2(xIntensity, yIntensity), durationInMS);
	}
	void Camera::ApplyScreenShake(const float xIntensity, const float yIntensity, ScreenShakeDuration duration)
	{
		ApplyScreenShake(sl::Vector2(xIntensity, yIntensity), (int)duration);
	}

	void Camera::ResetScreenShake()
	{
		for (int i = 0; i < instance->screenShakeValues.size(); i++)
		{
			delete instance->screenShakeValues[i];
		}

		instance->screenShakeValues.clear();
	}


	void Camera::Zoom(const float factor)
	{
		instance->ZoomFactor *= factor;
	}

	void Camera::SetZoom(const float newZoomFactor)
	{
		instance->ZoomFactor = newZoomFactor;
	}

	void Camera::ResetZoom()
	{
		SetZoom(1.0f);
	}
}