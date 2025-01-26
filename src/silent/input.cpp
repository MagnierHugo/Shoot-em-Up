// input.cpp

#include "Input.hpp"

namespace sl
{
	Input::Input()
	{
		InitEmpty();
	}

	Input::Input(sf::Keyboard::Key key)
	{
		InitEmpty(InputTypes::Keyboard);
		Key = key;
	}
	
	Input::Input(sf::Mouse::Button button)
	{
		InitEmpty(InputTypes::MouseButton);
		Button = button;
	}
	
	Input::Input(sf::Mouse::Wheel wheel)
	{
		InitEmpty(InputTypes::MouseWheel);
		Wheel = wheel;
	}
	
	Input::Input(sf::Joystick::Axis joystickAxis)
	{
		InitEmpty(InputTypes::JoystickAxis);
		JoystickAxis = joystickAxis;
	}
	
	Input::Input(unsigned int joystickButton)
	{
		InitEmpty(InputTypes::JoystickButton);
		JoystickButton = JoystickButton;
	}

	void Input::InitEmpty(InputTypes inputType)
	{
		InputType = inputType;
		Key = sf::Keyboard::Unknown;
		Button = sf::Mouse::ButtonCount; // make no sense but no unknown (check wether InputType is None beforehand anyway)
		Wheel = sf::Mouse::Wheel::HorizontalWheel;
		JoystickAxis = sf::Joystick::Axis::X; // X cuz I like X
		JoystickButton = 0;
	}

	Input Input::None = Input();
}