#pragma once

#include "Component.hpp"
#include "common.hpp"
#include "shape_type.hpp"
#include "resource_manager.hpp"
#include "vector.hpp"
#include "transform.hpp"

namespace sl
{
    class Renderable : public Component
    {
    public:
        Renderable() = default;
        ~Renderable();
        void Update(const float) override;
        void UpdateBounds();
        ShapeType ShapeType = ShapeType::None;

        sf::Text* text = nullptr;
        sf::CircleShape* circleShape = nullptr;
        sf::RectangleShape* rectangleShape = nullptr;
        sf::Sprite* sprite = nullptr;

        bool ShouldRender = true;

        Vector2 GetSize();
        float GetWidth();
        float GetHeight();

        void SetModeRect(Vector2& size, sf::Color color);
        void SetModeCircle(float radius, sf::Color color);
        void SetModeSprite(Ref<Resource> texture);
        void SetModeSprite(sf::Texture& texture);
        void SetModeText(const std::string& actualText, Ref<Resource> font, int textSize, sf::Color color);
        void SetText(const std::string& actualText);

        sf::Drawable* GetDrawable() const;
        sf::FloatRect GetBounds() const;

        void SetOrigin(bool center); // sets to topleft otherwise 

        void Flicker(float duration); // blink if u prefer ^^
        void SetVisibility(bool setTo);
    private:
        sf::Texture texture_;
        float flickerSpeed = 75.0f;
        bool isFlickering = false;
    };
}



