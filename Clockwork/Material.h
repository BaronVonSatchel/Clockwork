#pragma once

#include "Clockwork.h"
#include <SFML/Graphics.hpp>

namespace Clockwork {
	extern "C" struct CLOCKWORK_API Material {
		bool lit;

		const sf::Texture& base;

		const sf::Texture& normal;

		const sf::Texture& emissive;
	};
}