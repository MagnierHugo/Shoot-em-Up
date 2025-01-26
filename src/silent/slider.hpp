// slider.hpp

#pragma once

#include "Component.hpp"
#include "Renderable.hpp"
#include "vector.hpp"



namespace sl {
    
    class Slider : public Component {

    friend class BallSliderBehaviour;

    private:

        struct SliderBounds {
            Vector2 min;
            Vector2 max;
        };

    public:
        void Awake() override;

        void SetSliderBounds(SliderBounds slider_bounds);
        void SetSliderBounds(Vector2 min, Vector2 max);
        void SetMinMaxValues(float min_, float max_);
        void UpdateValue();
        void SetCallback(std::function<void(float)> callback_);

    private:
        Ref<Renderable> renderable;
        SliderBounds bounds;
        Vector2 axis;
        float value_percentage; // between 0 and 1
        float ballRadius = 0;
        float min = .0f;
        float max = .0f;
        float value = .0f;
        std::function<void(float)> callback = [](float _) { };
    };

    class BallSliderBehaviour : public Component {
    public:
        void Awake() override;
        void Update(const float deltaTime) override;

    private:
        Ref<Renderable> renderable;
        Ref<Slider> slider;
        b8 is_moving;
        Vector2 center_to_topleft;
    };

    void InitSlider(Ref<Entity> slider, Vector2 slider_scale, Vector2 slider_pos, const std::string& slider_path, const std::string& ball_path);

}
