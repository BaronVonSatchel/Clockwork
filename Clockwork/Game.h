#pragma once

#include "Clockwork.h"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Scene.h"

namespace cw {

	
	///<summary>Game is the "master" class which enforces the game loop and manages the game window.</summary>
	extern "C" class CLOCKWORK_API Game
	{
	public:
		Game();
		virtual ~Game();

		///<summary>Initializes the game window with the given title and framerate limit, but does not start the game loop</summary>
		void init(const sf::String& title, unsigned int framerate, sf::ContextSettings windowSettings = sf::ContextSettings());

		///<summary>Zeroes the game clock and begins the game loop.
		///Will return immediately if the game is already running.
		///Otherwise, will NOT return until quit() is called!
		///</summary>
		void start();

		///<summary>Closes the game window and exits the program</summary>
		void quit();

		///<summary>Changes the current scene. Unsafe to use while the game is running.</summary>
		void setScene(Scene* newScene);

		///<summary>Returns the elapsed game time in seconds</summary>
		static float getGameTime();

		///<summary>Returns the elapsed game time in milliseconds</summary>
		static sf::Int32 getGameTimeMillis();

		///<summary>Returns the mouse position relative to the top left of the window</summary>
		static sf::Vector2i getMousePosition();

	protected:
		virtual void tick(const sf::Time& timestep);

		virtual void draw();

	private:
		static Game* instance;

		sf::RenderWindow window;

		void toggleConsoleWindow();

		void processDebugCommand(const sf::String& command);
		
		static sf::Clock gameClock;

		Scene* scene;

		bool isRunning = false;

		tgui::Theme theme;
		tgui::ChildWindow::Ptr consoleWindow;
		tgui::ChatBox::Ptr consoleLog;
		tgui::EditBox::Ptr consoleCommand;

	};

}