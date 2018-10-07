#include "stdafx.h"
#include "RotatableSystem.h"

#define V_DIRECTION "dir"

RotatableSystem::RotatableSystem(std::string _sysID, std::string _displayName, sf::Vector2i _dimensions, unsigned int _directions)
	: GenericSystem(_sysID, _displayName, _dimensions), directions(_directions)
{
	interiorTexture.loadFromFile("assets/system/" + sysID + "/inb.png");
}


RotatableSystem::~RotatableSystem()
{
}

void RotatableSystem::drawInterior(const SystemBlueprintData & data, const ShipBlueprint& ship, sf::RenderTarget & target, const sf::RenderStates & states) const
{
	sf::Sprite sprite;
	sprite.setTexture(interiorTexture);
	sprite.setPosition(sf::Vector2f(TILE_SIZE * data.position));

	sprite.setTextureRect({data.intVars.at(V_DIRECTION) * TILE_SIZE, 0, dimensions.x * TILE_SIZE, dimensions.y * TILE_SIZE});

	target.draw(sprite, states);
}

void RotatableSystem::drawExterior(const SystemBlueprintData & data, const ShipBlueprint & ship, sf::RenderTarget & base, sf::RenderTarget & normal, sf::RenderTarget & emissive) const
{
	if (!shouldDrawExterior) return;

	sf::Sprite sprite;
	sprite.setPosition(sf::Vector2f(TILE_SIZE * data.position));
	sprite.setTextureRect({ data.intVars.at(V_DIRECTION) * TILE_SIZE, 0, dimensions.x * TILE_SIZE, dimensions.y * TILE_SIZE });

	sprite.setTexture(exteriorBase);
	base.draw(sprite);

	sprite.setTexture(exteriorNormal);
	normal.draw(sprite);

	sprite.setTexture(exteriorEmissive);
	emissive.draw(sprite);
}

void RotatableSystem::editorInit(SystemBlueprintData & temp) const
{
	GenericSystem::editorInit(temp);
	temp.intVars[V_DIRECTION] = 0;
}

bool RotatableSystem::editorScrollWheel(ShipBlueprint & ship, SystemBlueprintData & temp, float amount) const
{
	if (fabs(amount) > 0.5) {
		temp.intVars[V_DIRECTION] = (temp.intVars[V_DIRECTION] + ((amount > 0) ? 1 : directions - 1)) % directions;
	}
	return false;
}

std::vector<std::string> RotatableSystem::getInstructions() const
{
	std::vector<std::string> lines = GenericSystem::getInstructions();

	lines.push_back("Scroll wheel to rotate");

	return lines;
}
