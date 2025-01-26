// slider.cpp

#include "slider.hpp"
#include "input_manager.hpp"
#include "Renderable.hpp"


namespace sl {

    void Slider::Awake() {
        renderable = Owner->GetComponent<Renderable>();
    }

    void Slider::SetSliderBounds(SliderBounds slider_bounds) {
        bounds = slider_bounds;
        axis = slider_bounds.max - slider_bounds.min;
    }

    void Slider::SetSliderBounds(Vector2 min, Vector2 max) {
        bounds = { min, max };
        axis = max - min;
    }

    void Slider::SetMinMaxValues(float min_, float max_)
    {
        min = min_;
        max = max_;
    }


    void Slider::UpdateValue()
    {
        float valueBefore = value;
        value = min + ((max - min) * value_percentage);

        if (valueBefore == value) { return; }

        callback(value);
    }

    void Slider::SetCallback(std::function<void(float)> callback_)
    {
        callback = callback_;
    }

    void BallSliderBehaviour::Awake() {
        renderable = Owner->GetComponent<Renderable>();
        slider = Owner->parent->GetComponent<Slider>();
        is_moving = false;

        center_to_topleft = Vector2(-renderable->GetWidth(), -renderable->GetHeight()) / 2;
        Transform()->Position = slider->bounds.min + (slider->axis / 2) + center_to_topleft;
    }

    void BallSliderBehaviour::Update(const float deltaTime) {

        if (!InputManager::GetMouseButton(sf::Mouse::Left)) {
            is_moving = false;
            return; 
        }

        Vector2 mouse_pos = InputManager::GetMousePosition();
        Vector2 ball_center = Transform()->Position - center_to_topleft;

        float squared_distance = pow(mouse_pos.x - ball_center.x, 2) + pow(mouse_pos.y - ball_center.y, 2);
        float squared_radius = pow(renderable->GetWidth() / 2, 2);

        if ((squared_distance > squared_radius) && !is_moving) { return; }
        is_moving = true;

        Vector2 mouse_delta = InputManager::GetMouseDelta();
        Vector2 projection = slider->axis * (mouse_delta.Dot(slider->axis) / slider->axis.SquaredLength());
        ball_center -= projection;

        // Objectively superior method :)
        ball_center = std::max(slider->bounds.min, std::min(slider->bounds.max, ball_center));

        // Objectively worst method
        // if (*ball_pos < slider->bounds.min) {
        //     *ball_pos = slider->bounds.min;
        // }
        // else if (*ball_pos > slider->bounds.max) {
        //     *ball_pos = slider->bounds.max;
        // }

        slider->value_percentage = (ball_center - slider->bounds.min).Length() / slider->axis.Length();
        slider->UpdateValue();
        Transform()->Position = ball_center + center_to_topleft;
    }

    void InitSlider(Ref<Entity> slider, Vector2 slider_scale, Vector2 slider_pos, const std::string& slider_path, const std::string& ball_path) {
        slider->transform->Scale = slider_scale;
        slider->transform->Position = slider_pos;

        Ref<Renderable> renderable = slider->AddComponent<Renderable>();
        renderable->SetModeSprite(ResourceManager::Get(slider_path, ResourceType::Texture));

        Ref<Slider> slider_component = slider->AddComponent<Slider>();
        Vector2 slider_bounds_min = {slider_pos.x, slider_pos.y + renderable->GetHeight() / 2};
        Vector2 slider_bounds_max = {slider_pos.x + renderable->GetWidth(), slider_pos.y + renderable->GetHeight() / 2};
        slider_component->SetSliderBounds(slider_bounds_min, slider_bounds_max);

        Ref<Entity> ball = slider->CreateChildEntity("SliderBall");
        ball->transform->Scale = slider_scale;

        Ref<Renderable> ball_renderable = ball->AddComponent<Renderable>();
        ball_renderable->SetModeSprite(sl::ResourceManager::Get(ball_path, sl::ResourceType::Texture));

        { auto _ = ball->AddComponent<BallSliderBehaviour>(); }

    }

}