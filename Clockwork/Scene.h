#pragma once

#include <set>
#include <vector>
#include "Clockwork.h"
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "Actor.h"
#include "PhongLighting.h"

namespace Clockwork {
	///<summary>Various global parameters regarding the world</summary>
	extern "C" struct CLOCKWORK_API WorldSettings {

		///<summary>How many times per tick to attwmpt to resolve collisions.
		///Higher numbers will use more CPU but lower numbers may cause
		///instability in multi-object collisions.</summary>
		unsigned int maxCollisionPasses = 4;
	};

	///<summary>The scene controls what is currently happening in the game,
	///such as gameplay or a menu screen. It creates and handles the user
	///interface, receives input events, and determines what is drawn on screen.
	///An instance of Game may have only one Scene active at a time.</summary>
	extern "C" class CLOCKWORK_API Scene
	{
	friend class Game;

	public:
		Scene(WorldSettings _worldSettings = WorldSettings());
		virtual ~Scene();

		///<summary>Called when first changing to the scene. Use this to initialuze the GUI.</summary>
		virtual void setup(const sf::RenderTarget& target);

		///<summary>Called every tick to advance time. Do not call manually.</summary>
		virtual void tick(const sf::Time& timestep);

		///<summary>Called every frame to draw the whole scene.</summary>
		void drawScene(sf::RenderTarget& target);

		///<summary>Called every frame to draw the game world.</summary>
		virtual void drawWorld(sf::RenderTarget& target);

		///<summary>Called whenever there is an event to process.</summary>
		virtual void handleEvent(sf::Event event);

		///<summary>Tells the game to change scene at the end of the current tick</summary>
		void scheduleSceneChange(Scene* scene);

		///<summary>Ticks the scene and all current actors. Do not call manually.</summary>
		void tickMaster(const sf::Time& timestep);

		///<summary>Add an actor to the scene.</summary>
		///<returns>True if successful, false if actor is null or already a part of the scene</returns>
		bool spawnActor(Actor* actor);

		///<summary>Remove an actor from the scene (does not delete it from memory).</summary>
		///<returns>True if successful, false if actor is null or never a part of the scene</returns>
		bool removeActor(Actor* actor);

		void resetBaseBuffer();

		void resetNormalBuffer();

		void resetEmissiveBuffer();

		virtual void postProcess(sf::RenderTarget& target, const sf::Texture& texture);

		const WorldSettings worldSettings;

		sf::RenderTexture world;

		bool lightingEnabled;

		PhongLighting lighting;

		sf::View worldView;

	protected:
		///<summary>The GUI for this Scene.</summary>
		tgui::Gui gui;

		bool enablePostProcess = true;

		//sf::RenderTexture actorZBuffer;
	private:

		void init(sf::RenderTarget& target);

		bool isSceneChangeScheduled = false;

		Scene* nextScene;

		std::set<Actor*, compareLayer> actors;

		sf::Shader depthShader;

		sf::RenderTexture baseBuffer, normalBuffer, emissiveBuffer, postProcessBuffer;
	};
}
