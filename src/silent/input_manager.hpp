// input_manager.hpp

#pragma once

#include <unordered_set>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "input.hpp"

namespace sl
{
	class InputManager
	{
	public:
		InputManager(sf::RenderWindow* window);

		void Update(sf::Event& e);
		static void ResetInputState();

		static bool GetWhetherAnyInput();
		static Input GetAnyInput();

		static bool GetAnyKeyDown();
		static bool GetWhichKeyDown(sf::Keyboard::Key& key);
		static bool GetKeyDown(const sf::Keyboard::Key key);
		static bool GetKey(const sf::Keyboard::Key key);
		static bool GetKeyUp(const sf::Keyboard::Key key);

		static bool GetMouseButtonDown(const sf::Mouse::Button button);
		static bool GetWhichMouseButtonDown(sf::Mouse::Button& button);
		static bool GetAnyMouseButtonDown();
		static bool GetMouseButton(const sf::Mouse::Button button);
		static bool GetMouseButtonUp(const sf::Mouse::Button button);

		static sf::Vector2i GetMousePosition();
		static bool GetWhetherAnyMouseMotion();
		static sf::Vector2i GetMouseDelta();

		static int GetMouseWheelDelta();
	private:
		static InputManager* instance;

		sf::RenderWindow* window_ptr;

		std::unordered_set<sf::Keyboard::Key> keysHeld;
		std::unordered_set<sf::Keyboard::Key> keysDown;
		std::unordered_set<sf::Keyboard::Key> keysUp;

		std::unordered_set<sf::Mouse::Button> mouseButtonsDown;
		std::unordered_set<sf::Mouse::Button> mouseButtonsUp;

		sf::Vector2i mousePosition;
		sf::Vector2i mouseDelta;

		int mouseWheelDelta = 0;
	};
}
