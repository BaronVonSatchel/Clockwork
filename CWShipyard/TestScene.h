#pragma once

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "AnimatedSprite.h"

class TestScene : public Clockwork::Scene
{
public:
	TestScene();
	~TestScene();

	void setup() override;

	void tick(const sf::Time& timestep) override;

	void draw(sf::RenderTarget& target) override;
private:
	int tickCounter = 0;

	sf::Texture katTexture;
	Clockwork::AnimatedSprite katSprite;

	sf::Texture red;
	sf::Texture testNormal;
	sf::Texture emissive;
	sf::Sprite testShape;
	sf::Shader diffuse;
	sf::Clock clock;
};

