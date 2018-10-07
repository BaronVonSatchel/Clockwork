#include "stdafx.h"
#include "MainMenu.h"
#include "ShipDesigner.h"
#include "SystemRegistry.h"

MainMenu::MainMenu()
{
}


MainMenu::~MainMenu()
{
}

void MainMenu::setup(const sf::RenderTarget& target)
{
	//tgui::Theme theme{ "assets/black.txt" };

	SystemRegistry::init();

	red.loadFromFile("assets/biglogo.png");

	testShape.setOrigin(128, 32);
	testShape.setPosition(800, 450);
	testShape.setScale(2, 2);
	testShape.setTexture(red);

	emissive.loadFromFile("assets/emissive.png");
	testNormal.loadFromFile("assets/biglogo_nrm.png");

	diffuse.loadFromFile("assets/lighting.frag", sf::Shader::Fragment);
	diffuse.setUniform("ambient", 0.3f);

	blueprint.loadFromFile("assets/blueprint.png");
	blueprint.setRepeated(true);
	bg.setTexture(blueprint);
	bg.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)target.getView().getSize()));

	timer = 0;
}

void MainMenu::tick(const sf::Time& timestep)
{
	timer += timestep.asSeconds();
	light = { -(timer / 3.f) + 0.5f, -0.5, -0.5 };
	if (timer > 3) scheduleSceneChange(new ShipDesigner());
}

void MainMenu::drawWorld(sf::RenderTarget & target)
{
	target.draw(bg);

	diffuse.setUniform("texBase", sf::Shader::CurrentTexture);
	diffuse.setUniform("normal", testNormal);
	diffuse.setUniform("emissive", emissive);
	diffuse.setUniform("lightVector", light);
	diffuse.setUniform("lightColor", sf::Vector3f(1, 1, 1));

	target.draw(testShape, &diffuse);
}