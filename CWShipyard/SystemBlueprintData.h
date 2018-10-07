#pragma once
#include <unordered_map>
#include <SFML/System.hpp>

class ShipSystem;

///<summary>Information about a system which is part of the "design" of the blueprint (i.e. information which can only change in the editor, not the world)</summary>
struct SystemBlueprintData {
	const ShipSystem* systemType;
	sf::Vector2i position;
	std::unordered_map<std::string, int> intVars;
	std::unordered_map<std::string, float> floatVars;

	///<summary>Default constructor has NULL system type and negative position. Cast to int returns -1. Useful for checking empty tiles.</summary>
	SystemBlueprintData() : systemType(NULL), position({ -1, 0 }) {};

	SystemBlueprintData(const ShipSystem* _systemType, const sf::Vector2i _position) : systemType(_systemType), position(_position) {};

	bool operator<(const SystemBlueprintData& other) const {
		return (int)(*this) < (int)other;
	}

	bool operator==(const SystemBlueprintData& other) const {
		return (int)(*this) == (int)other;
	}

	///<summary> Returns an integer equal to (y position * 256) + x position. Because systems may not have overlapping coordinates, this can be used as a unique identifier within a single blueprint.</summary>
	operator int() const {
		return (position.y << 8) + position.x;
	}
};