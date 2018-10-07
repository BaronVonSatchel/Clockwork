#pragma once

#include <SFML/Graphics.hpp>

///<summary>Convenient structure for passing a base texture, normal map, and emissive texture at the same time</summary>
struct TextureSet {
	sf::Texture base;
	sf::Texture normal;
	sf::Texture emissive;
};