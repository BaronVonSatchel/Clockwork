#pragma once

#include "Clockwork.h"
#include <SFML/Graphics.hpp>

namespace Clockwork {
	extern "C" class CLOCKWORK_API Material {
	public: 
		virtual const sf::Shader* base() const = 0;

		virtual const sf::Shader* normal() const = 0;

		virtual const sf::Shader* emissive() const = 0;
	};
}