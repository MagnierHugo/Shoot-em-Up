// input.hpp

#pragma once

#include <SFML/Window.hpp>

namespace sl
{
	enum class InputTypes
	{
		None,
		Keyboard,
		MouseButton,
		MouseWheel,
		JoystickButton,
		JoystickAxis,
	};

	class Input
	{
	public:
		static Input None;
		
		Input();
		Input(sf::Keyboard::Key key);
		Input(sf::Mouse::Button button);
		Input(sf::Mouse::Wheel wheel);
		Input(sf::Joystick::Axis joystickAxis);
		Input(unsigned int joystickButton);
		
		InputTypes InputType;
		sf::Keyboard::Key Key;
		sf::Mouse::Button Button;
		sf::Mouse::Wheel Wheel;
		int WheelDelta = 0;
		sf::Joystick::Axis JoystickAxis;
		unsigned int JoystickButton;
	private:
		void InitEmpty(InputTypes inputType = InputTypes::None);
	};
}
