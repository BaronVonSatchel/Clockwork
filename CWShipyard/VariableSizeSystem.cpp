#include "stdafx.h"
#include "VariableSizeSystem.h"
#include "ShipBlueprint.h"
#include <iostream>

VariableSizeSystem::VariableSizeSystem(std::string _sysID, std::string _displayName)
	: ShipSystem(_sysID), displayName(_displayName)
{
	if (!interiorTexture.loadFromFile("assets/system/" + sysID + "/inb.png")) {
		std::cout << "ERROR: failed to load \"assets/system/" << sysID << "/inb.png\"!" << std::endl;
	}
}

VariableSizeSystem::~VariableSizeSystem()
{
}

std::string VariableSizeSystem::getDisplayName() const
{
	return displayName;
}

int VariableSizeSystem::getWidth(const SystemBlueprintData & data) const
{
	return data.intVars.at(V_SIZE_X);
}

int VariableSizeSystem::getLength(const SystemBlueprintData & data) const
{
	return data.intVars.at(V_SIZE_Y);
}

void VariableSizeSystem::drawInterior(const SystemBlueprintData & data, const ShipBlueprint& ship, sf::RenderTarget & target, const sf::RenderStates & states) const
{
	sf::Vector2i dimensions(data.intVars.at(V_SIZE_X), data.intVars.at(V_SIZE_Y));

	sf::Sprite s(interiorTexture);
	sf::IntRect tr( 0, 0, TILE_SIZE, TILE_SIZE );
	for (int j = 0; j < dimensions.y; j++) {
		for (int i = 0; i < dimensions.x; i++) {
			if (dimensions.x > 1) tr.left = (i > 0) ? ((i < dimensions.x - 1) ? TILE_SIZE * 2 : TILE_SIZE * 3) : TILE_SIZE;
			if (dimensions.y > 1) tr.top  = (j > 0) ? ((j < dimensions.y - 1) ? TILE_SIZE * 2 : TILE_SIZE * 3) : TILE_SIZE;

			s.setTextureRect(tr);
			s.setPosition(sf::Vector2f((data.position + sf::Vector2i(i, j)) * TILE_SIZE));
			target.draw(s, states);
		}
	}
}

void VariableSizeSystem::editorInit(SystemBlueprintData & temp) const
{
	temp = SystemBlueprintData(this, { 0, 0 });
	temp.intVars[V_SIZE_X] = 1;
	temp.intVars[V_SIZE_Y] = 1;
}

bool VariableSizeSystem::editorMouseDown(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile) const
{
	temp.intVars[V_TEMP_X] = tile.x;
	temp.intVars[V_TEMP_Y] = tile.y;
	return false;
}

bool VariableSizeSystem::editorMouseUp(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile) const
{
	updatePreviewSize(temp, tile);
	temp.intVars.erase(V_TEMP_X);
	temp.intVars.erase(V_TEMP_Y);
	bool success = ship.tryPlaceSystem(temp);
	temp.position = tile;
	temp.intVars[V_SIZE_X] = 1;
	temp.intVars[V_SIZE_Y] = 1;
	return success;
}

bool VariableSizeSystem::editorMouseMove(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile, bool drag) const
{
	if (tile == temp.position) return false;
	if (drag) {
		updatePreviewSize(temp, tile);
	}
	else {
		temp.position = tile;
	}
	return false;
}

std::vector<std::string> VariableSizeSystem::getInstructions() const
{
	std::vector<std::string> lines;
	lines.push_back("Left click and drag to create a box of any size");

	return lines;
}

const sf::Texture& VariableSizeSystem::getInteriorTexture() const
{
	return interiorTexture;
}

void VariableSizeSystem::updatePreviewSize(SystemBlueprintData & temp, sf::Vector2i tile) const
{
	sf::Vector2i origin(temp.intVars[V_TEMP_X], temp.intVars[V_TEMP_Y]);
	sf::Vector2i currentSize(getWidth(temp), getLength(temp));

	temp.position.x = std::min(tile.x, origin.x);
	temp.position.y = std::min(tile.y, origin.y);
	temp.intVars[V_SIZE_X] = abs(origin.x - tile.x) + 1;
	temp.intVars[V_SIZE_Y] = abs(origin.y - tile.y) + 1;
}
