#include "stdafx.h"
#include "PhongLighting.h"
#include "Vecmath.h"

PhongLighting::PhongLighting()
{
	lightShader.loadFromMemory(
		"   #version 120"
		"\n uniform sampler2D texBase;"
		"\n uniform sampler2D normal;"
		"\n uniform sampler2D emissive;"
		"\n uniform vec3 lightVector;"
		"\n uniform vec3 lightColor;"
		"\n uniform float ambient;"

		"\n void main() {"
		"\n		vec3 lightNorm = normalize(lightVector);"
		"\n		vec3 n = normalize((texture2D(normal, gl_TexCoord[0].xy).rgb * 2.0) - 1.0);"
		"\n		vec4 baseColor = texture2D(texBase, gl_TexCoord[0].xy);"
		"\n		vec4 ems = texture2D(emissive, gl_TexCoord[0].xy);"

		//diffuse
		"\n		float diffuse = clamp(-dot(lightNorm, n), 0, 1);"

		//specular
		"\n		vec3 reflected = reflect(lightNorm, n);"
		"\n		float spec = pow(max(reflected.z, 0.0), 32);"

		"\n		gl_FragColor = vec4(clamp((baseColor.rgb * lightColor * (diffuse + ambient)) + (lightColor * spec) + ems.rgb, 0, 1), baseColor.a);"
		"\n }", sf::Shader::Fragment);
}


PhongLighting::~PhongLighting()
{
}

void PhongLighting::get(sf::RenderStates& states, float rotation, const sf::Texture& normal, const sf::Texture& emissive)
{
	lightShader.setUniform("texBase", sf::Shader::CurrentTexture);
	lightShader.setUniform("normal", normal);
	lightShader.setUniform("emissive", emissive);
	lightShader.setUniform("lightColor", color);
	lightShader.setUniform("ambient", 0.3f);

	sf::Vector2f adjustedHorizontal(direction.x, direction.y);
	Clockwork::revolve(adjustedHorizontal, -rotation);
	lightShader.setUniform("lightVector", sf::Vector3f(adjustedHorizontal.x, adjustedHorizontal.y, direction.z));
	states.shader = &lightShader;
}
