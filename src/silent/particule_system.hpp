#pragma once
#include <SFML/Graphics.hpp>

#include <vector>

namespace sl
{
	class Particule
	{
	public:

		Particule(float R, sf::Vector2f P, sf::Time T, sf::Color C) : radius(R), position(P), life_span(T), color(C)
		{
			particule_shape.setRadius(radius);
			particule_shape.setPosition(position);
		};


		sf::CircleShape Get_Shape() { return particule_shape; };

	private:
		float radius;

		sf::Vector2f position;
		sf::CircleShape particule_shape;

		sf::Time life_span;
		sf::Color color;
	};

	class Particule_System
	{
	public:
		void AddParticules(int amount, Particule particule);
		void DeleteParticule();
		void UpdateParticule();
		void MoveParticule();
		void RenderParticule(sf::RenderWindow& window, Particule particule);

	private:
		std::vector<Particule> particule_array;
		int amount = particule_array.size();
	};
}