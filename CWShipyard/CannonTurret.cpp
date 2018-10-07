#include "stdafx.h"
#include "CannonTurret.h"


CannonTurret::CannonTurret(std::string _sysID, std::string _displayName, int _size)
	: GenericSystem(_sysID, _displayName, sf::Vector2i(_size, _size))
{
	
}


CannonTurret::~CannonTurret()
{
}