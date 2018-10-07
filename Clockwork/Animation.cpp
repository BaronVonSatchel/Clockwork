#include "stdafx.h"
#include "Animation.h"

using namespace Clockwork;

Animation::Animation()
{
}

Animation::~Animation()
{
	deleteFrame(first);
}


AnimationFrame Clockwork::Animation::getFirst() const
{
	return *first;
}

void Clockwork::Animation::setLoop(bool loop)
{
	this->loop = loop;
	if (loop) last->next = first;
	else last->next = last;
}

void Clockwork::Animation::addFrame(sf::IntRect textureRect, int delay)
{
	AnimationFrame* frame = new AnimationFrame();
	frame->textureRect = textureRect;
	frame->delay = delay;
	if (fCount > 0) {
		last->next = frame;
		last = frame;

		setLoop(loop);
	}
	else {
		frame->next = frame;
		first = frame;
		last = frame;
	} 

	fCount++;
}

unsigned int Clockwork::Animation::getFrameCount() const
{
	return fCount;
}

void Clockwork::Animation::deleteFrame(AnimationFrame * frame)
{
	if(frame != last) deleteFrame(frame->next);
	delete frame;
}
