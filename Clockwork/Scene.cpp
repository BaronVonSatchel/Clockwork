#include "stdafx.h"
#include "Scene.h"
#include "Vecmath.h"

using namespace cw;

bool Scene::hasInitialized = false;

Scene::Scene(WorldSettings _worldSettings) : worldSettings(_worldSettings)
{
	depthShader.loadFromMemory(
		"   uniform sampler2D texBase;"
		"\n uniform float depth;"

		"\n void main() {"
		"\n 	if (texture2D(texBase, gl_TexCoord[0].xy).a > 0) {"
		"\n 		gl_FragColor = vec4(depth, depth, depth, 1.0);"
		"\n 	}"
		"\n 	else gl_FragColor = vec4(0, 0, 0, 0);"
		"\n }", sf::Shader::Fragment);

	lightingShader.loadFromMemory(
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

		//put it all together
		"\n		gl_FragColor = vec4(clamp((baseColor.rgb * lightColor * (diffuse + ambient)) + (lightColor * spec) + ems.rgb, 0, 1), baseColor.a);"
		"\n }", sf::Shader::Fragment);

	depthShader.setUniform("texBase", sf::Shader::CurrentTexture);
}

Scene::~Scene()
{
}

void Scene::setup(const sf::RenderTarget& target)
{
}

void Scene::tick(const sf::Time& timestep)
{
}

void Scene::drawScene(sf::RenderTarget& target)
{
	world.clear();
	drawWorld(world);
	world.display();

	target.draw(sf::Sprite(world.getTexture()));

	for (auto& actor : actors) {
		if (!actor->material) continue;
		//world.clear(sf::Color::Transparent);

		resetBaseBuffer();
		baseBuffer.draw(actor->getShape(), actor->material->base());
		baseBuffer.display();
		resetNormalBuffer();
		normalBuffer.draw(actor->getShape(), actor->material->normal());
		normalBuffer.display();
		resetEmissiveBuffer();
		emissiveBuffer.draw(actor->getShape(), actor->material->emissive());
		emissiveBuffer.display();

		sf::Sprite sprite;
		sprite.setTexture(baseBuffer.getTexture());

		//setup lighting
		lightingShader.setUniform("texBase", sf::Shader::CurrentTexture);
		lightingShader.setUniform("normal", normalBuffer.getTexture());
		lightingShader.setUniform("emissive", emissiveBuffer.getTexture());
		lightingShader.setUniform("lightColor", sf::Vector3f( lightColor.r, lightColor.g, lightColor.b));
		lightingShader.setUniform("ambient", 0.3f);

		sf::Vector2f adjustedHorizontal(lightDirection.x, lightDirection.y);
		cw::revolve(adjustedHorizontal, -actor->getWorldRotation());
		lightingShader.setUniform("lightVector", sf::Vector3f(adjustedHorizontal.x, adjustedHorizontal.y, lightDirection.z));

		if (enablePostProcess) {
			postProcessBuffer.draw(sprite, &lightingShader);
			postProcessBuffer.display();
			postProcess(target, postProcessBuffer.getTexture());
		}
		else {
			target.draw(sprite, &lightingShader);
		}
		//world.display();

		//postProcess(target, world.getTexture());
	}

	gui.draw();
}

void cw::Scene::drawWorld(sf::RenderTarget & target)
{
}

void cw::Scene::handleEvent(sf::Event event)
{
}

void cw::Scene::scheduleSceneChange(Scene * scene)
{
	isSceneChangeScheduled = true;

	nextScene = scene;
}

void cw::Scene::init(sf::RenderTarget & target)
{
	worldView = target.getView();
	world.create(target.getSize().x, target.getSize().y);
	baseBuffer.create(target.getSize().x, target.getSize().y);
	normalBuffer.create(target.getSize().x, target.getSize().y);
	emissiveBuffer.create(target.getSize().x, target.getSize().y);
	postProcessBuffer.create(target.getSize().x, target.getSize().y);
	gui.setTarget( target );
	setup(target);
}

void cw::Scene::tickMaster(const sf::Time & timestep)
{
	tick(timestep);
	for (auto& actor : actors) actor->update(timestep);
}

bool cw::Scene::spawnActor(Actor * actor)
{
	if (actor && actors.insert(actor).second) {
		actor->scene = this;
		return true;
	} else return false;
}

bool cw::Scene::removeActor(Actor * actor)
{
	return actors.erase(actor);
}

void cw::Scene::resetBaseBuffer()
{
	baseBuffer.clear(sf::Color::Transparent);
	baseBuffer.setView(worldView);
}

void cw::Scene::resetNormalBuffer()
{
	normalBuffer.clear(sf::Color::Black);
	normalBuffer.setView(worldView);
}

void cw::Scene::resetEmissiveBuffer()
{
	emissiveBuffer.clear(sf::Color::Black);
	emissiveBuffer.setView(worldView);
}

void cw::Scene::postProcess(sf::RenderTarget & target, const sf::Texture & texture)
{
	target.draw(sf::Sprite(texture));
}

void cw::Scene::staticInit()
{
	if (hasInitialized) return;

	hasInitialized = true;
}
