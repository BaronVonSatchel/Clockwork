#pragma once

#include "Clockwork.h"
#include <SFML/Graphics.hpp>

namespace cw {
	extern "C" class CLOCKWORK_API Material {
	public: 
		virtual const sf::Shader* base() = 0;

		virtual const sf::Shader* normal() = 0;

		virtual const sf::Shader* emissive() = 0;
	};
}