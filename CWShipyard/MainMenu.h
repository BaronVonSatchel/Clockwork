#pragma once
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include "Scene.h"

class MainMenu : public Clockwork::Scene
{
public:
	MainMenu();
	~MainMenu();

	void setup(const sf::RenderTarget& target) override;

	void tick(const sf::Time& timestep) override;

	void drawWorld(sf::RenderTarget& target) override;

private:
	sf::Texture red;
	sf::Texture testNormal;
	sf::Texture emissive;
	sf::Sprite testShape;
	sf::Shader diffuse;

	sf::Sprite bg;
	sf::Texture blueprint;

	float timer;
	sf::Vector3f light;
};

