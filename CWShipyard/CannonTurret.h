#pragma once
#include "GenericSystem.h"
#include "ShipWeapon.h"
class CannonTurret :
	public GenericSystem, public ShipWeapon
{
public:
	CannonTurret(std::string _sysID, std::string _displayName, int _size);
	~CannonTurret();

private:
};

