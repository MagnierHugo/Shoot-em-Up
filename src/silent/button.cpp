// button.cpp

#include <cmath>

#include "button.hpp"
#include "Renderable.hpp"
#include "input_manager.hpp"
#include "time.hpp"


namespace sl
{    
    void OnClick::Awake() {
        renderable = Owner->GetComponent<Renderable>();
    }

    void OnClick::Update(const float deltaTime) {
        if (!InputManager::GetMouseButtonDown(sf::Mouse::Left) || !clickable) { return; }

        sf::Vector2i mouse_pos = InputManager::GetMousePosition();
        Vector2 button_pos = Transform()->Position;
        

        sf::IntRect rect;
        float squared_distance, squared_radius;

        switch (renderable->ShapeType)
        {
            case ShapeType::Rectangle:
                rect = { button_pos, renderable->GetSize()};

                if (rect.contains(mouse_pos)) {
                    Clicked();
                }

                break;

            case ShapeType::Circle:
                squared_distance = pow(mouse_pos.x - button_pos.x, 2) + pow(mouse_pos.y - button_pos.y, 2);
                squared_radius = pow(renderable->circleShape->getRadius(), 2);

                if (squared_distance <= squared_radius) {
                    Clicked();
                }

                break;

            case ShapeType::Sprite:
                rect = { button_pos, renderable->GetSize() };

                if (rect.contains(mouse_pos)) {
                    Clicked();
                }

                break;
        
            default:
                std::cout << "ShapeType: None" << std::endl;
                break;
        }
    }
    
    void OnClick::ManualCall() const {
        on_click();
    }

    void OnClick::Clicked() {
        on_click();
        clickable = false;
        StartTimer(100, [&]() { this->clickable = true; });

    }

    void OnClick::SetCallback(std::function<void(void)> callback) {
        on_click = callback;
    }

    void InitButton(Ref<Entity> button, Vector2 topleft, std::string path, std::function<void(void)> callback) {

        Ref<Renderable> renderable = button->AddComponent<Renderable>();
        renderable->SetModeSprite(ResourceManager::Get(path, ResourceType::Texture));

        button->transform->Scale = sl::Vector2(0.667f, 0.667f); // only works as long as the button stays a rect ^^
        button->transform->Position = topleft;

        Ref<OnClick> on_click = button->AddComponent<OnClick>();
        on_click->SetCallback(callback);
    }

    void InitButton(Ref<Renderable> renderable, Ref<Entity> button, Vector2 topleft, std::string path, std::function<void(void)> callback) {

        renderable = button->AddComponent<Renderable>();
        renderable->SetModeSprite(ResourceManager::Get(path, ResourceType::Texture));

        button->transform->Scale = sl::Vector2(0.667f, 0.667f); // only works as long as the button stays a rect ^^
        button->transform->Position = topleft;

        Ref<OnClick> on_click = button->AddComponent<OnClick>();
        on_click->SetCallback(callback);
    }

}