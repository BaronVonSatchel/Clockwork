#include "stdafx.h"
#include "ShipBlueprint.h"
#include "ShipSystem.h"
#include "Scene.h"
#include <iostream>


ShipBlueprint::ShipBlueprint()
{
	for (int i = 0; i < MAX_SHIP_SIZE; i++) {
		for (unsigned j = 0; j < MAX_SHIP_SIZE; j++) {
			coordinateToSystemMap[i][j] = -1;
		}
	}

	stitchedInterior.create(MAX_SHIP_SIZE * TILE_SIZE, MAX_SHIP_SIZE * TILE_SIZE);
	stitchedExteriorBase.create(MAX_SHIP_SIZE * TILE_SIZE, MAX_SHIP_SIZE * TILE_SIZE);
	stitchedExteriorNormal.create(MAX_SHIP_SIZE * TILE_SIZE, MAX_SHIP_SIZE * TILE_SIZE);
	stitchedExteriorEmissive.create(MAX_SHIP_SIZE * TILE_SIZE, MAX_SHIP_SIZE * TILE_SIZE);
	temp.create(MAX_SHIP_SIZE * TILE_SIZE, MAX_SHIP_SIZE * TILE_SIZE);

	bHeight.loadFromFile("assets/bevel_height.frag", sf::Shader::Fragment);
	bNorm.loadFromFile("assets/bevel_normal.frag", sf::Shader::Fragment);
	paint.loadFromFile("assets/paint.frag", sf::Shader::Fragment);

	drawInterior = true;
}

ShipBlueprint::~ShipBlueprint()
{
}

void ShipBlueprint::drawActor(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (drawInterior) {
		target.draw(sf::Sprite(getStitchedInterior()), states);
	}
	else {
		//getScene()->lighting.get(states, getRotation(), getStitchedExteriorNormal(), getStitchedExteriorEmissive());
		target.draw(sf::Sprite(getStitchedExteriorBase()), states);
	}
}

bool ShipBlueprint::tryPlaceSystem(const SystemBlueprintData& system)
{
	if (canPlaceSystem(system) != LEGAL) return false;
	if (systems.insert({ (int)system, system }).second) {
		for (int i = 0; i < system.systemType->getWidth(system); i++) {
			for (int j = 0; j < system.systemType->getLength(system); j++) {
				coordinateToSystemMap[system.position.x + i][system.position.y + j] = (int)system;
			}
		}
		return true;
	}
	else return false;
}

bool ShipBlueprint::removeSystem(int id)
{
	if (systems.count(id) < 1) return false;
	SystemBlueprintData data = systems.at(id);
	if (data >= 0) {
		for (int j = 0; j < data.systemType->getLength(data); j++) {
			for (int i = 0; i < data.systemType->getWidth(data); i++) {
				coordinateToSystemMap[data.position.x + i][data.position.y + j] = -1;
			}
		}
		systems.erase(id);
		return true;
	} else return false;
}

std::set<SystemBlueprintData> ShipBlueprint::getSystems() const
{
	std::set<SystemBlueprintData> systemData;
	for (auto& pair : systems) systemData.insert(pair.second);
	return systemData;
}

unsigned int ShipBlueprint::systemCount() const
{
	return systems.size();
}

const SystemBlueprintData ShipBlueprint::operator[](sf::Vector2i v) const
{
	if (v.x < 0 || v.x >= MAX_SHIP_SIZE || v.y < 0 || v.y >= MAX_SHIP_SIZE) return SystemBlueprintData();
	int id = coordinateToSystemMap[v.x][v.y];
	return systems.count(id) > 0 ? systems.at(id) : SystemBlueprintData();
}

PlacementLegality ShipBlueprint::canPlaceSystem(const SystemBlueprintData & data, std::string & message) const
{
	//check if the system is out of bounds
	if (data.position.x < 0 || data.position.y < 0) return ILLEGAL_OUT_OF_BOUNDS;
	const ShipSystem* type = data.systemType;
	sf::Vector2i extents(type->getWidth(data), type->getLength(data));
	if (data.position.x + extents.x > MAX_SHIP_SIZE - 1 || data.position.y + extents.y > MAX_SHIP_SIZE - 1) {
		message = "Out of build area!";
		return ILLEGAL_OUT_OF_BOUNDS;
	}

	//only do if there is at least one other system already placed
	if (systemCount() > 0) {
		//check if the system overlaps with an existing system
		for (int j = 0; j < extents.y; j++) {
			for (int i = 0; i < extents.x; i++) {
				if ((*this)[data.position + sf::Vector2i(i, j)] >= 0) {
					message = "Overlaps an existing system!";
					return ILLEGAL_OVERLAP;
				}
			}
		}

		//ensure that the system is adjacent to at least one other system
		bool hasAdjacency = false;
		for (int i = 0; i < extents.x; i++) {
			if ((data.position.y > 0 && (*this)[data.position + sf::Vector2i(i, -1)] >= 0)
				|| (data.position.y < MAX_SHIP_SIZE - 1 && (*this)[data.position + sf::Vector2i(i, extents.y)] >= 0)) {
				hasAdjacency = true;
				break;
			}
		}
		if (!hasAdjacency) {
			for (int j = 0; j < extents.y; j++) {
				if ((data.position.x > 0 && (*this)[data.position + sf::Vector2i(-1, j)] >= 0)
					|| (data.position.x < MAX_SHIP_SIZE - 1 && (*this)[data.position + sf::Vector2i(extents.x, j)] >= 0)) {
					hasAdjacency = true;
					break;
				}
			}
			if (!hasAdjacency) {
				message = "Must connect to an existing system!";
				return ILLEGAL_NOT_ADJACENT;
			}
		}
	}

	//if nothing else, check the system's special rules
	message = "Placement OK";
	return type->canPlace(*this, data, message) ? LEGAL : ILLEGAL_SPECIAL_RULES;
}

PlacementLegality ShipBlueprint::canPlaceSystem(const SystemBlueprintData & data) const
{
	std::string dummy;
	return canPlaceSystem(data, dummy);
}

void ShipBlueprint::reformTextures()
{
	//std::cout << "Reforming textures!" << std::endl;
	temp.clear(sf::Color::Transparent);
	stitchedInterior.clear(sf::Color::Transparent);
	stitchedExteriorBase.clear(sf::Color::Transparent);
	stitchedExteriorNormal.clear(sf::Color::Transparent);
	stitchedExteriorEmissive.clear(sf::Color::Transparent);

	for (auto& system : getSystems()) {
		system.systemType->drawInterior(system, *this, stitchedInterior);
	}

	stitchedInterior.display();

	sf::Sprite sprite;
	sprite.setTexture(stitchedInterior.getTexture());
	
	paint.setUniform("texBase", sf::Shader::CurrentTexture);
	paint.setUniform("paintColor", colorToGlVec4(sf::Color(0xAA, 0xAA, 0xAA)));
	stitchedExteriorBase.draw(sprite, &paint);
	stitchedExteriorBase.display();

	bHeight.setUniform("texBase", sf::Shader::CurrentTexture);
	bHeight.setUniform("texSize", (sf::Vector2i)stitchedInterior.getSize());
	bHeight.setUniform("limit", 4);
	temp.draw(sprite, &bHeight);
	temp.display();

	sprite.setTexture(temp.getTexture());

	bNorm.setUniform("texBase", sf::Shader::CurrentTexture);
	bNorm.setUniform("texSize", (sf::Vector2i)stitchedInterior.getSize());
	stitchedExteriorNormal.draw(sprite, &bNorm);
	stitchedExteriorNormal.display();

	for (auto& pair : systems) {
		pair.second.systemType->drawExterior(pair.second, *this, stitchedExteriorBase, stitchedExteriorNormal, stitchedExteriorEmissive);
	}

	stitchedExteriorBase.display();
	stitchedExteriorNormal.display();
	stitchedExteriorEmissive.display();
}

const sf::Texture& ShipBlueprint::getStitchedInterior() const
{
	return stitchedInterior.getTexture();
}

const sf::Texture & ShipBlueprint::getStitchedExteriorBase() const
{
	return stitchedExteriorBase.getTexture();
}

const sf::Texture & ShipBlueprint::getStitchedExteriorNormal() const
{
	return stitchedExteriorNormal.getTexture();
}

const sf::Texture & ShipBlueprint::getStitchedExteriorEmissive() const
{
	return stitchedExteriorEmissive.getTexture();
}

sf::Glsl::Vec4 colorToGlVec4(const sf::Color & color)
{
	return sf::Glsl::Vec4(color.r / 255.F, color.g / 255.F, color.b / 255.F, color.a / 255.F);
}
