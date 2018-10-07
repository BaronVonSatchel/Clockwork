#include "stdafx.h"
#include "TestScene.h"
#include "Game.h"

#include <sstream>

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

void TestScene::setup()
{
	tgui::Font typewriter("assets/fonts/atwriter.ttf");
	gui.setFont(typewriter);
	tgui::Theme::Ptr theme = tgui::Theme::create("assets/black.txt");

	katTexture.loadFromFile("assets/katerina.png");
	katSprite.setTexture(katTexture);
	katSprite.setPosition(100, 0);

	Clockwork::Animation* fire = new Clockwork::Animation();
	fire->addFrame({ 0, 0, 64, 128 }, 250);
	fire->addFrame({ 64, 0, 64, 128 }, 100);
	fire->addFrame({ 128, 0, 64, 128 }, 250);
	fire->addFrame({ 192, 0, 64, 128 }, 100);
	fire->addFrame({ 256, 0, 64, 128 }, 100);
	fire->addFrame({ 128, 0, 64, 128 }, 100);
	fire->addFrame({ 64, 0, 64, 128 }, 100);
	fire->setLoop(true);

	katSprite.addAnimation("fire", fire);
	katSprite.play("fire");

	red.loadFromFile("assets/red.png");

	testShape.setPosition(400, 300);
	testShape.setOrigin({ 32, 32 });
	testShape.setTexture(red);

	emissive.loadFromFile("assets/emissive.png");
	testNormal.loadFromFile("assets/nrm.png");

	diffuse.loadFromFile("assets/diffuse.frag", sf::Shader::Fragment);
}

void TestScene::tick(const sf::Time& timestep)
{
	/*if (tickCounter < 2000 && (++tickCounter % 60) == 0) {
		std::stringbuf buffer;
		std::ostream os(&buffer);
		os << tickCounter << " ticks and " << 
			Clockwork::Game::getGameTime() <<
			" seconds have elapsed" << std::endl;
		logBox->addText(buffer.str());
	}*/
	katSprite.update(timestep);
}

void TestScene::draw(sf::RenderTarget * target)
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(0x886644FF));
	rect.setSize({ 800, 600 });
	rect.setPosition({ 0, 0 });
	target->draw(rect);
	target->draw(katSprite);

	diffuse.setUniform("texBase", sf::Shader::CurrentTexture);
	diffuse.setUniform("normal", testNormal);
	diffuse.setUniform("emissive", emissive);
	diffuse.setUniform("lightVector", sf::Vector3f(.707f * cos(clock.getElapsedTime().asSeconds()), .707f * sin(clock.getElapsedTime().asSeconds()), .707f));
	diffuse.setUniform("lightColor", sf::Vector3f(1, 1, 1));

	testShape.setRotation(clock.getElapsedTime().asSeconds() * 180.f / 3.14159f - 90);

	target->draw(testShape, &diffuse);
}