#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Clockwork.h"
#include "Animation.h"

#define ANIM_NONE "NONE"

namespace Clockwork {

	extern "C" class CLOCKWORK_API AnimatedSprite : public sf::Sprite
	{
	public: 
		AnimatedSprite();
		AnimatedSprite(sf::Texture texture);

		~AnimatedSprite();

		void update(const sf::Time& timestep);

		void play(std::string animationName);

		void nextFrame();

		bool addAnimation(std::string, const Animation*);
	private:
		sf::Time animationTimer;

		std::unordered_map<std::string, const Animation*> animationLibrary;

		AnimationFrame currentFrame;

		bool isLooping;
	};
}


