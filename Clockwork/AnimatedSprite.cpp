#include "stdafx.h"
#include "AnimatedSprite.h"

using namespace Clockwork;

AnimatedSprite::AnimatedSprite()
{
	
}

AnimatedSprite::AnimatedSprite(sf::Texture texture) : Sprite(texture)
{
}


AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::update(const sf::Time & timestep)
{
	animationTimer += timestep;
	while (animationTimer.asMilliseconds() > currentFrame.delay) {
		animationTimer -= sf::milliseconds(currentFrame.delay);
		nextFrame();
	}
}

void AnimatedSprite::play(std::string animationName)
{
	currentFrame = animationLibrary[animationName]->getFirst();

	setTextureRect(currentFrame.textureRect);
}

void AnimatedSprite::nextFrame()
{
	if (currentFrame.next) currentFrame = *currentFrame.next;

	setTextureRect(currentFrame.textureRect);
}

bool AnimatedSprite::addAnimation(std::string name, const Animation * animation)
{
	return animationLibrary.emplace(name, animation).second != false;
}