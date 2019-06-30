#pragma once
#include "Material.h"

namespace cw {

	extern "C" class CLOCKWORK_API MaterialSimpleTexture :
		public Material
	{
	public:
		MaterialSimpleTexture(const sf::Texture& _tBase, const sf::Texture& _tNormal, const sf::Texture& _tEmissive);
		~MaterialSimpleTexture();

		const sf::Shader* base() override;

		const sf::Shader* normal() override;

		const sf::Shader* emissive() override;

	private:
		sf::Texture tBase, tNormal, tEmissive;

		sf::Shader shader;
	};

}