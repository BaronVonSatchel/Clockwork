#include "stdafx.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>

using namespace Clockwork;

sf::Clock Game::gameClock;
Game* Game::instance;

Game::Game()
{
	scene = new Scene();
}

Game::~Game()
{
}

void Clockwork::Game::init(const sf::String & title, unsigned int framerate, sf::ContextSettings windowSettings)
{
	std::srand(std::time(0));

	if (instance) {
		std::cout << "ERROR: Game already running" << std::endl;
		exit(-1);
	}
	instance = this;

	std::cout << "GLSL Version: " << windowSettings.majorVersion << windowSettings.minorVersion << std::endl;
	window.create({ 1600, 900 }, title, sf::Style::Titlebar | sf::Style::Close, windowSettings);
	if(framerate > 0) window.setFramerateLimit(framerate);	
	else window.setVerticalSyncEnabled(true);

	consoleWindow = tgui::ChildWindow::create();
	consoleWindow->setSize({ 400, 300 });
	consoleWindow->setPosition({ 100, 100 });
	consoleWindow->setTitle("Debug Console");

	consoleLog = tgui::ChatBox::create();
	consoleLog->setSize({ 400, 280 });
	consoleLog->setPosition({ 0, 0 });
	consoleLog->setTextSize(12);

	consoleCommand = tgui::EditBox::create();
	consoleCommand->setSize({ 400, 20 });
	consoleCommand->setPosition({ 0, 280 });
	consoleCommand->setTextSize(12);
	consoleCommand->setDefaultText("Enter debug command...");
	consoleCommand->connect("ReturnKeyPressed", &Game::processDebugCommand, this);

	consoleWindow->add(consoleLog, "consoleLog");
	consoleWindow->add(consoleCommand, "consoleCommand");
}

void Game::start()
{
	if (isRunning) return;
	isRunning = true;

	gameClock.restart();
	sf::Time lastTime = gameClock.getElapsedTime();
	sf::Time time;

	while (window.isOpen()) {
		time = gameClock.getElapsedTime();
		if (scene->isSceneChangeScheduled) {
			setScene(scene->nextScene);
		}

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				quit();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
				toggleConsoleWindow();
			}
			if(!scene->gui.handleEvent(event)) scene->handleEvent(event);
		}

		tick(time - lastTime);

		window.clear();
		draw();
		window.display();

		lastTime = time;
	}
}

void Clockwork::Game::quit()
{
	window.close();
}

void Clockwork::Game::setScene(Scene * newScene)
{
	Scene* oldScene = scene;
	scene = newScene;
	scene->init(window);
	delete oldScene;
}

float Clockwork::Game::getGameTime()
{
	return gameClock.getElapsedTime().asSeconds();
}

sf::Int32 Clockwork::Game::getGameTimeMillis()
{
	return gameClock.getElapsedTime().asMilliseconds();
}

sf::Vector2i Clockwork::Game::getMousePosition()
{
	return sf::Mouse::getPosition(instance->window);
}

void Game::tick(const sf::Time& timestep)
{
	scene->tick(timestep);
}

void Game::draw()
{
	scene->drawScene(window);
}

void Clockwork::Game::toggleConsoleWindow()
{
	if (!scene->gui.remove(consoleWindow)) {
		scene->gui.add(consoleWindow, "consoleWindow");
	}
}

void Clockwork::Game::processDebugCommand(const sf::String & command)
{
	if (command.getSize() == 0) return;
	consoleLog->addLine("> " + command);
	consoleCommand->setText("");
}
