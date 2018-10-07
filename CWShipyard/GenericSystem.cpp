#include "stdafx.h"
#include "GenericSystem.h"
#include "ShipBlueprint.h"


GenericSystem::GenericSystem(std::string _sysID, std::string _displayName, sf::Vector2i _dimensions) :
	ShipSystem(_sysID), displayName(_displayName), dimensions(_dimensions)
{
	interiorTexture.loadFromFile("assets/system/" + sysID + "/inb.png");
}


GenericSystem::~GenericSystem()
{
}

int GenericSystem::getWidth(const SystemBlueprintData&) const
{
	return dimensions.x;
}

int GenericSystem::getLength(const SystemBlueprintData&) const
{
	return dimensions.y;
}

std::string GenericSystem::getDisplayName() const
{
	return displayName;
}

void GenericSystem::drawInterior(const SystemBlueprintData & data, const ShipBlueprint& ship, sf::RenderTarget & target, const sf::RenderStates & states) const
{
	sf::Sprite sprite;
	sprite.setTexture(interiorTexture);
	sprite.setPosition(sf::Vector2f(TILE_SIZE * data.position));

	target.draw(sprite, states);
}

void GenericSystem::drawExterior(const SystemBlueprintData & data, const ShipBlueprint& ship, sf::RenderTarget & base, sf::RenderTarget & normal, sf::RenderTarget & emissive) const
{
	if (!shouldDrawExterior) return;

	sf::Sprite sprite;
	sprite.setPosition(sf::Vector2f(TILE_SIZE * data.position));

	sprite.setTexture(exteriorBase);
	base.draw(sprite);

	sprite.setTexture(exteriorNormal);
	normal.draw(sprite);

	sprite.setTexture(exteriorEmissive);
	emissive.draw(sprite);
}

void GenericSystem::editorInit(SystemBlueprintData & temp) const
{
	temp = SystemBlueprintData(this, { 0, 0 });
}

bool GenericSystem::editorMouseDown(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile) const
{
	return ship.tryPlaceSystem(temp);
}

bool GenericSystem::editorMouseMove(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile, bool drag) const
{
	tile += sf::Vector2i(1, 1) - dimensions;
	if(temp.position != tile && tile.x >= 0 && tile.y >= 0 && tile.x < MAX_SHIP_SIZE && tile.y < MAX_SHIP_SIZE) temp.position = tile;
	if (drag) return ship.tryPlaceSystem(temp);
	else return false;
}

std::vector<std::string> GenericSystem::getInstructions() const
{
	std::vector<std::string> lines;

	lines.push_back("Left click to place system");

	return lines;
}

void GenericSystem::setUseExterior(bool useEmissive)
{
	shouldDrawExterior = true;
	exteriorBase.loadFromFile("assets/system/" + sysID + "/exb.png");
	exteriorNormal.loadFromFile("assets/system/" + sysID + "/exn.png");
	if(useEmissive) exteriorEmissive.loadFromFile("assets/system/" + sysID + "/exe.png");
	else {
		sf::RenderTexture rt;
		rt.create(dimensions.x * TILE_SIZE, dimensions.y * TILE_SIZE);
		rt.clear(sf::Color::Black);
		exteriorEmissive = rt.getTexture();
	}
}
