#include "particule_system.hpp"


void sl::Particule_System::AddParticules(int amount, Particule particule)
{
	for (int i = 0; i < amount; i++) 
	{
		particule_array.push_back(particule);
	}
}

void sl::Particule_System::RenderParticule(sf::RenderWindow& window, Particule particule) 
{
	for (int i = 0; i < amount; i++)
	{
		window.draw(particule.Get_Shape());
	}
}

