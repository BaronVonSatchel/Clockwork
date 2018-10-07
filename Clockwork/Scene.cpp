#include "stdafx.h"
#include "Scene.h"

using namespace Clockwork;

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

		world.clear(sf::Color::Transparent);
		world.draw(*actor);
		world.display();

		postProcess(target, world.getTexture());
	}

	gui.draw();
}

void Clockwork::Scene::drawWorld(sf::RenderTarget & target)
{
}

void Clockwork::Scene::handleEvent(sf::Event event)
{
}

void Clockwork::Scene::scheduleSceneChange(Scene * scene)
{
	isSceneChangeScheduled = true;

	nextScene = scene;
}

void Clockwork::Scene::init(sf::RenderTarget & target)
{
	world.create(target.getSize().x, target.getSize().y);
	//actorZBuffer.create(target.getSize().x, target.getSize().y);
	gui.setTarget( target );
	setup(target);
}

void Clockwork::Scene::tickMaster(const sf::Time & timestep)
{
	tick(timestep);
	for (auto& actor : actors) actor->update(timestep);
}

bool Clockwork::Scene::spawnActor(Actor * actor)
{
	if (actor && actors.insert(actor).second) {
		actor->scene = this;
		return true;
	} else return false;
}

bool Clockwork::Scene::removeActor(Actor * actor)
{
	return actors.erase(actor);
}

void Clockwork::Scene::postProcess(sf::RenderTarget & target, const sf::Texture & texture)
{
	target.draw(sf::Sprite(texture));
}
