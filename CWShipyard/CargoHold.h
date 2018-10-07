#pragma once
#include "VariableSizeSystem.h"
class CargoHold :
	public VariableSizeSystem
{
public:
	CargoHold(std::string _sysID, std::string _displayName);
	~CargoHold();

	virtual void drawInterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const override;

	virtual void drawExterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& base, sf::RenderTarget& normal, sf::RenderTarget& emissive) const override;

private:
	TextureSet exteriorTextures;
};

