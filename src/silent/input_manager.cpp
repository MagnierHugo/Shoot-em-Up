// input_manager.cpp

#include <iostream>

#include "input_manager.hpp"

namespace sl
{
    InputManager* InputManager::instance = nullptr;

    void InputManager::ResetInputState()
    {
        instance->keysDown.clear();
        instance->keysUp.clear();
        instance->mouseButtonsDown.clear();
        instance->mouseButtonsUp.clear();
        instance->mouseDelta = sf::Vector2i(0, 0);
        instance->mouseWheelDelta = 0;
    }

    InputManager::InputManager(sf::RenderWindow* window)
    {
        instance = this;
        window_ptr = window;
    }

    void InputManager::Update(sf::Event& event)
    {
        sf::Vector2i previousMousePosition;
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                if (keysHeld.find(event.key.code) == keysHeld.end())
                {
                    keysDown.insert(event.key.code);
                    keysHeld.insert(event.key.code);
                }
                break;
            case sf::Event::KeyReleased:
                keysUp.insert(event.key.code);
                keysHeld.erase(event.key.code);
                break;
            case sf::Event::MouseButtonPressed:
                mouseButtonsDown.insert(event.mouseButton.button);
                break;
            case sf::Event::MouseButtonReleased:
                mouseButtonsUp.insert(event.mouseButton.button);
                break;
            case sf::Event::MouseMoved:
                previousMousePosition = mousePosition;
                mousePosition = sf::Mouse::getPosition(*window_ptr);
                mouseDelta = previousMousePosition - mousePosition;
                break;
            case sf::Event::MouseWheelScrolled:
                mouseWheelDelta = event.mouseWheelScroll.delta;
                break;
            default:
                break;
        }	
    }

    bool InputManager::GetWhetherAnyInput()
    {
        return
            !instance->keysDown.empty() ||
            !instance->mouseButtonsDown.empty() ||
            instance->mouseDelta != sf::Vector2i(0, 0) ||
            instance->mouseWheelDelta != 0;
    }

    Input InputManager::GetAnyInput()
    {
        if (GetWhetherAnyInput())
        {
            Input toReturn;

            sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
            if (GetWhichKeyDown(key))
            {
                toReturn.InputType = InputTypes::Keyboard;
                toReturn.Key = key;
                return toReturn;
            }

            sf::Mouse::Button button = (sf::Mouse::Button) -1; // invalid button
            if (GetWhichMouseButtonDown(button))
            {
                toReturn.InputType = InputTypes::MouseButton;
                toReturn.Button = button;
                return toReturn;
            }

            int mouseWheelDelta = GetMouseWheelDelta();
            if (mouseWheelDelta != 0)
            {
                toReturn.InputType = InputTypes::MouseWheel;
                toReturn.WheelDelta = mouseWheelDelta;
                return toReturn;
            }

        // JoystickButton,
        // JoystickAxis,
        }

        return Input::None;
    }

    bool InputManager::GetAnyKeyDown()
    {
        return !instance->keysDown.empty();
    }

    bool InputManager::GetWhichKeyDown(sf::Keyboard::Key& key)
    {
        if ((int)instance->keysDown.size() != 1) { return false; }

        key = *instance->keysDown.begin();

        return true;
    }

    bool InputManager::GetKeyDown(const sf::Keyboard::Key key)
    {
        return instance->keysDown.find(key) != instance->keysDown.end();
    }

    bool InputManager::GetKey(const sf::Keyboard::Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    bool InputManager::GetKeyUp(const sf::Keyboard::Key key)
    {
        return instance->keysUp.find(key) != instance->keysUp.end();
    }

    bool InputManager::GetAnyMouseButtonDown()
    {
        return !instance->mouseButtonsDown.empty();
    }

    bool InputManager::GetWhichMouseButtonDown(sf::Mouse::Button& button)
    {
        if ((int)instance->mouseButtonsDown.size() != 1) { return false; }

        button = *instance->mouseButtonsDown.begin();

        return true;
    }

    bool InputManager::GetMouseButtonDown(const sf::Mouse::Button button)
    {
        return instance->mouseButtonsDown.find(button) != instance->mouseButtonsDown.end();
    }

    bool InputManager::GetMouseButton(const sf::Mouse::Button button)
    {
        return sf::Mouse::isButtonPressed(button);
    }

    bool InputManager::GetMouseButtonUp(const sf::Mouse::Button button)
    {
        return instance->mouseButtonsUp.find(button) != instance->mouseButtonsUp.end();
    }

    sf::Vector2i InputManager::GetMousePosition()
    {
        return instance->mousePosition;
    }

    bool InputManager::GetWhetherAnyMouseMotion()
    {
        return  instance->mouseDelta.x != 0 && instance->mouseDelta.y != 0;
    }

    sf::Vector2i InputManager::GetMouseDelta()
    {
        return instance->mouseDelta;
    }

	int InputManager::GetMouseWheelDelta()
	{
		return instance->mouseWheelDelta;
	}
}