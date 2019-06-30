#include "stdafx.h"
#include "Actor.h"
#include "Vecmath.h"
#include <algorithm>
#include <limits>

using namespace cw; 

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

Scene * cw::Actor::getScene() const
{
	return scene;
}

sf::Transform cw::Actor::getWorldTransform() const
{
	if (parent) return parent->getWorldTransform() * getTransform();
	else return getTransform();
}

float cw::Actor::getWorldRotation() const
{
	Actor* parent = getParent();
	return getRotation() + (parent ? parent->getWorldRotation() : 0.0f);
}

char cw::Actor::getActorPhysics() const
{
	if (parent) return 0;
	else return physics;
}

float cw::Actor::getMass() const
{
	return (physics & PHYSICS_DYNAMIC) ? std::max(mass, 0.0f) : 0.0f;
}

void cw::Actor::setMaterial(Material * material)
{
	this->material = material;
}

float cw::Actor::getLayerZ() const
{
	return layerZ;
}

void cw::Actor::tick(const sf::Time & timestep)
{
}

void cw::Actor::drawActor(sf::RenderTarget & target, sf::RenderStates states) const
{
}

bool cw::Actor::attachTo(Actor * parent)
{
	if (this->parent) detach();

	scene = parent->scene;

	if (parent->children.insert(this).second) {
		this->parent = parent;
		return true;
	}
	else return false;
}

void cw::Actor::detach()
{
	parent->children.erase(this);
	parent = nullptr;
}

Actor * cw::Actor::getParent() const
{
	return parent;
}

Actor * cw::Actor::getRoot()
{
	if (parent) return parent->getRoot();
	else return this;
}

const std::set<Actor*, compareLayer> cw::Actor::getChildren() const
{
	return children;
}

void cw::Actor::update(const sf::Time & timestep)
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

bool cw::compareLayer::operator()(const Actor * a, const Actor * b) const
{
	return (float)a->layer + a->getLayerZ() < (float)b->layer + b->getLayerZ();
}
