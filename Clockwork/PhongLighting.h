#pragma once
#include "Clockwork.h"
#include <SFML/Graphics.hpp>

extern "C" class CLOCKWORK_API PhongLighting
{
public:
	PhongLighting();
	~PhongLighting();

	void get(sf::RenderStates& states, float rotation, const sf::Texture& normal, const sf::Texture& emissive);

	sf::Vector3f direction;

	sf::Vector3f color;

private:
	sf::Shader lightShader;
};

