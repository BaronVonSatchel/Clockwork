#pragma once
#include "SFML\Graphics.hpp"

#include "Clockwork.h"

namespace Clockwork {
	///<summary>Defines a single frame of animation</summary>
	extern "C" struct CLOCKWORK_API AnimationFrame {
		///<summary>Portion of this sprite's texture to draw</summary>
		sf::IntRect textureRect;
		///<summary>Time in milliseconds before moving on to the next frame</summary>
		int delay;
		///<summary>Next frame in the linked list. Not recommended to initialize manually.</summary>
		AnimationFrame* next = NULL;
	};

	extern "C" class CLOCKWORK_API Animation
	{
	public:
		Animation();
		~Animation();

		AnimationFrame getFirst() const;

		void setLoop(bool loop);

		void addFrame(sf::IntRect textureRect, int delay);

		unsigned int getFrameCount() const;
	private:
		void deleteFrame(AnimationFrame* frame);

		AnimationFrame* first;
		AnimationFrame* last;

		unsigned int fCount = 0;

		bool loop = false;
	};
}

