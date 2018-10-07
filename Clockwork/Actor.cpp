#include "stdafx.h"
#include "Actor.h"
#include "Vecmath.h"
#include <algorithm>
#include <limits>

using namespace Clockwork; 

//std::unordered_map<std::string, Actor*(*)()> Actor::actorClasses;

Actor::Actor()
{
	layer = 0;
	physics = 0;
	layerZ = (float)std::rand() / INT_MAX + 0.5f;
}


Actor::~Actor()
{
}

Scene * Clockwork::Actor::getScene() const
{
	return scene;
}

void Clockwork::Actor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!visible) return;
	states.transform *= getTransform();
	drawActor(target, states);
	for (auto& child : children) child->draw(target, states);
}

sf::Transform Clockwork::Actor::getWorldTransform() const
{
	if (parent) return parent->getWorldTransform() * getTransform();
	else return getTransform();
}

char Clockwork::Actor::getActorPhysics() const
{
	if (parent) return 0;
	else return physics;
}

float Clockwork::Actor::getMass() const
{
	return (physics & PHYSICS_DYNAMIC) ? std::max(mass, 0.0f) : 0.0f;
}

float Clockwork::Actor::getLayerZ() const
{
	return layerZ;
}

void Clockwork::Actor::tick(const sf::Time & timestep)
{
}

void Clockwork::Actor::drawActor(sf::RenderTarget & target, sf::RenderStates states) const
{
}

bool Clockwork::Actor::attachTo(Actor * parent)
{
	if (this->parent) detach();

	scene = parent->scene;

	if (parent->children.insert(this).second) {
		this->parent = parent;
		return true;
	}
	else return false;
}

void Clockwork::Actor::detach()
{
	parent->children.erase(this);
	parent = nullptr;
}

Actor * Clockwork::Actor::getParent() const
{
	return parent;
}

Actor * Clockwork::Actor::getRoot()
{
	if (parent) return parent->getRoot();
	else return this;
}

const std::set<Actor*, compareLayer> Clockwork::Actor::getChildren() const
{
	return children;
}

void Clockwork::Actor::update(const sf::Time & timestep)
{
	for (auto& child : children) child->update(timestep);
	tick(timestep);

	if (physics & PHYSICS_DYNAMIC) {
		velocity += forces * timestep.asSeconds();
		angularVelocity += torque * timestep.asSeconds();

		move(velocity * timestep.asSeconds());
		rotate(angularVelocity * timestep.asSeconds());

		forces = { 0, 0 };
		torque = 0;
	}
}

bool Clockwork::compareLayer::operator()(const Actor * a, const Actor * b) const
{
	return (float)a->layer + a->getLayerZ() < (float)b->layer + b->getLayerZ();
}
