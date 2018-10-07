#include "stdafx.h"
#include "SystemRegistry.h"
#include "ShipSystem.h"
#include "GenericSystem.h"
#include "VariableSizeSystem.h"
#include "RotatableSystem.h"
#include "CargoHold.h"

std::unordered_map<std::string, const ShipSystem*> SystemRegistry::idToSystem;

bool SystemRegistry::registerSystemType(ShipSystem * system)
{
	if (idToSystem.count(system->sysID) > 0) return false;
	idToSystem[system->sysID] = system;
	return true;
}

const ShipSystem * SystemRegistry::getSystemType(std::string sysID)
{
	return idToSystem[sysID];
}

const std::set<const ShipSystem*, compareSystems> SystemRegistry::getAllSystems()
{
	std::set<const ShipSystem*, compareSystems> systemSet;
	for (auto& pair : idToSystem) systemSet.insert(pair.second);
	return systemSet;
}

void SystemRegistry::init()
{
	GenericSystem* bridge2 = new GenericSystem("bridge_2", "Bridge II", { 3, 2 });
	bridge2->setUseExterior();
	SystemRegistry::registerSystemType(bridge2);

	GenericSystem* engine2 = new GenericSystem("engine_2", "Engine II", { 3, 4 });
	engine2->setUseExterior(true);
	SystemRegistry::registerSystemType(engine2);

	GenericSystem* engine1 = new GenericSystem("engine_1", "Engine I", { 2, 3 });
	engine1->setUseExterior(true);
	SystemRegistry::registerSystemType(engine1);

	GenericSystem* cannon2 = new GenericSystem("cannon_2", "Cannon II", { 2, 2 });
	//cannon2->setUseExterior();
	SystemRegistry::registerSystemType(cannon2);

	ShipSystem* reactor2 = new GenericSystem("reactor_2", "Reactor II", { 3, 3 });
	SystemRegistry::registerSystemType(reactor2);

	ShipSystem* cq = new VariableSizeSystem("crew_quarters", "Crew's Quarters");
	SystemRegistry::registerSystemType(cq);

	ShipSystem* corridor = new VariableSizeSystem("corridor", "Corridor");
	SystemRegistry::registerSystemType(corridor);

	ShipSystem* cargoHold = new CargoHold("cargo", "Cargo Hold");
	SystemRegistry::registerSystemType(cargoHold);

	ShipSystem* slope1x1 = new RotatableSystem("slope1x1", "Decorative Slope 1x1", { 1, 1 }, 4);
	SystemRegistry::registerSystemType(slope1x1);

	ShipSystem* slope1x2 = new RotatableSystem("slope1x2", "Decorative Slope 1x2", { 1, 2 }, 4);
	SystemRegistry::registerSystemType(slope1x2);
}
