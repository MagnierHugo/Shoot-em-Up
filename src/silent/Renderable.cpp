#include "Renderable.hpp"
#include "resource_manager.hpp"
#include "time.hpp"

namespace sl
{
    Renderable::~Renderable()
    {
        if (rectangleShape != nullptr)
        {
            delete rectangleShape;
            rectangleShape = nullptr;
        }

        if (circleShape != nullptr)
        {
            delete circleShape;
            circleShape = nullptr;
        }

        if (text != nullptr)
        {
            delete text;
            text = nullptr;
        }

        if (sprite != nullptr)
        {
            delete sprite;
            sprite = nullptr;
        }
    }

    void Renderable::Update(const float)
    {
        UpdateBounds();
    }

    void Renderable::UpdateBounds()
    {
        //std::cout << Owner->name << ": Updated bounds\n";

        switch (ShapeType)
        {
            case sl::ShapeType::Rectangle:
                rectangleShape->setPosition(Transform()->Position);
                rectangleShape->setScale(Transform()->Scale);
                return;

            case sl::ShapeType::Circle:
                circleShape->setPosition(Transform()->Position);
                circleShape->setScale(Transform()->Scale);
                return;

            case sl::ShapeType::Text:
                text->setPosition(Transform()->Position);
                text->setScale(Transform()->Scale);
                return;

            case sl::ShapeType::Sprite:
                sprite->setPosition(Transform()->Position);
                sprite->setScale(Transform()->Scale);
                return;

            default:
                //std::cout << "Somebody probably messed up, do u feel targeted yet? ^^" << std::endl;
                //std::cout << Owner->name << std::endl;
                //throw new std::runtime_error("Not doing it ^^");
                break;
        }
    }

    sl::Vector2 Renderable::GetSize()
    {
        UpdateBounds();
        return GetBounds().getSize();
    }

    float Renderable::GetWidth()
    {
        UpdateBounds();
        return GetBounds().width;
    }

    float Renderable::GetHeight()
    {
        UpdateBounds();
        return GetBounds().height;
    }

    sf::Drawable* Renderable::GetDrawable() const
    {
        switch (ShapeType)
        {
            case sl::ShapeType::Rectangle: 
                return rectangleShape;

            case sl::ShapeType::Circle:
                return circleShape;
            

            case sl::ShapeType::Text:
                return text;


            case sl::ShapeType::Sprite:
                return sprite;

            default:
                //std::cout << "Somebody probably messed up, do u feel targeted yet? ^^" << std::endl;
                //throw new std::runtime_error("Not doing it ^^");
                break;
        }

        return nullptr;
    }

    sf::FloatRect Renderable::GetBounds() const
    {
        switch (ShapeType)
        {
        case sl::ShapeType::Rectangle:
            return rectangleShape->getGlobalBounds();

        case sl::ShapeType::Circle:
            return circleShape->getGlobalBounds();


        case sl::ShapeType::Text:
            return text->getGlobalBounds();


        case sl::ShapeType::Sprite:
            return sprite->getGlobalBounds();

        default:
            //std::cout << "Somebody messed up, do u feel targeted yet? ^^" << std::endl;
            //throw new std::runtime_error("Not doing it ^^");
            break;
        }

        return sf::FloatRect();
    }

    void Renderable::Flicker(float duration)
    {
        if (isFlickering) { return; }

        if (duration < flickerSpeed) { return; } // don t ever waste my time like this

        isFlickering = true;
        SetVisibility(false);

        bool valueToSetTo = true;
        for (int durationForThisIteration = flickerSpeed; durationForThisIteration < duration; durationForThisIteration += flickerSpeed, valueToSetTo = !valueToSetTo)
        {
            StartTimer(durationForThisIteration, [=]() { SetVisibility(valueToSetTo); });
        }

        StartTimer(duration, [=]() { SetVisibility(true); isFlickering = false;});
    }

    void Renderable::SetVisibility(bool setTo)
    {
        ShouldRender = setTo;
    }

    void Renderable::SetOrigin(bool center)
    {
        switch (ShapeType)
        {
            case sl::ShapeType::Rectangle:
                rectangleShape->setOrigin(center ? GetSize() / 2 : Vector2::Zero.Copy());
                return;

            case sl::ShapeType::Circle:
                circleShape->setOrigin(center ? GetSize() / 2 : Vector2::Zero.Copy());
                return;

            case sl::ShapeType::Text:
                text->setOrigin(center ? GetSize() / 2 : Vector2::Zero.Copy());
                return;

            case sl::ShapeType::Sprite:
                sprite->setOrigin(center ? GetSize() / 2 : Vector2::Zero.Copy());
                return;
        }
    }

    void Renderable::SetModeRect(Vector2& size, sf::Color color)
    {
        ShapeType = ShapeType::Rectangle;

        if (rectangleShape != nullptr) { delete rectangleShape; }

        rectangleShape = new sf::RectangleShape(size);
        rectangleShape->setFillColor(color);
        rectangleShape->setScale(Transform()->Scale);
        rectangleShape->setPosition(Transform()->Position);
    }

    void Renderable::SetModeCircle(float radius, sf::Color color)
    {
        ShapeType = ShapeType::Circle;

        if (circleShape != nullptr) { delete circleShape; }

        circleShape = new sf::CircleShape(radius);
        circleShape->setFillColor(color);
        circleShape->setScale(Transform()->Scale);
        circleShape->setPosition(Transform()->Position);
    }

    void Renderable::SetModeSprite(Ref<Resource> texture)
    {
        SetModeSprite(texture->texture);
    }

    void Renderable::SetModeSprite(sf::Texture& texture)
    {
        ShapeType = ShapeType::Sprite;

        if (sprite != nullptr) { delete sprite; }

        sprite = new sf::Sprite();
        texture_ = texture;
        sprite->setTexture(texture_);
        sprite->setScale(Transform()->Scale);
        sprite->setPosition(Transform()->Position);
    }

    void Renderable::SetModeText(const std::string& actualText, Ref<Resource> font, int textSize, sf::Color color)
    {
        ShapeType = ShapeType::Text;

        if (text != nullptr) { delete text; }

        text = new sf::Text();
        text->setString(actualText);
        text->setFont(font->font);
        text->setCharacterSize(textSize);
        text->setScale(Transform()->Scale);
        text->setPosition(Transform()->Position);
    }

    void Renderable::SetText(const std::string& actualText)
    {
        text->setString(actualText);
    }
}