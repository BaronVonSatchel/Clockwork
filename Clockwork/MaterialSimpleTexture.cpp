#include "stdafx.h"
#include "MaterialSimpleTexture.h"

using namespace cw;

MaterialSimpleTexture::MaterialSimpleTexture(const sf::Texture& _tBase, const sf::Texture& _tNormal, const sf::Texture& _tEmissive)
{
	shader.loadFromMemory(
		"   uniform sampler2D texBase;"
		"\n void main() {"
		"\n 	gl_FragColor = texture2D(texBase, gl_TexCoord[0].xy);"
		"\n }", sf::Shader::Fragment);

	tBase = _tBase;
	tNormal = _tNormal;
	tEmissive = _tEmissive;
}


MaterialSimpleTexture::~MaterialSimpleTexture()
{
}

const sf::Shader * cw::MaterialSimpleTexture::base()
{
	shader.setUniform("texBase", tBase);
	return &shader;
}

const sf::Shader * cw::MaterialSimpleTexture::normal()
{
	shader.setUniform("texBase", tNormal);
	return &shader;
}

const sf::Shader * cw::MaterialSimpleTexture::emissive()
{
	shader.setUniform("texBase", tEmissive);
	return &shader;
}
