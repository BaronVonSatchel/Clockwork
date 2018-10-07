#pragma once
#include <set>
#include <unordered_map>
#include "ShipSystem.h"

class SystemRegistry
{
public:
	///<summary>Registers a system type in the with the given system ID. Returns true if the system was registered, false if another already has the specified ID.</summary>
	static bool registerSystemType(ShipSystem*);

	///<summary>Gets the system type (if any) registered to the given system ID.</summary>
	static const ShipSystem* getSystemType(std::string);

	static const std::set<const ShipSystem*, compareSystems> getAllSystems();

	static void init();
private:
	static std::unordered_map<std::string, const ShipSystem*> idToSystem;
};

