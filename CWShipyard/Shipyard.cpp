
#include "stdafx.h"
#include <Windows.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include <sstream>
#include "Clockwork.h"
#include "Game.h"
#include "Scene.h"
#include "MainMenu.h"
#include "ShipSystem.h"

int main()
{

	#ifndef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	#endif

	std::cout << "Starting Shipyard!" << std::endl;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	ShipSystem::initialize();

	cw::Game game;
	game.init("Shipyard: Now With 100% More Shaders!", 0, settings);
	game.setScene(new MainMenu());
	game.start();

	#ifndef _DEBUG
	FreeConsole();
	#endif

	return 0;
}