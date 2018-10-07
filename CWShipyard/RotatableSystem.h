#pragma once
#include "GenericSystem.h"

///<summary>A simple system with fixed stats and multiple visual styles that can be cycled with the scroll wheel in the editor</summary>
class RotatableSystem :
	public GenericSystem
{
public:
	RotatableSystem(std::string _sysID, std::string _displayName, sf::Vector2i _dimensions, unsigned int _directions);
	~RotatableSystem();

	virtual void drawInterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const override;

	virtual void drawExterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& base, sf::RenderTarget& normal, sf::RenderTarget& emissive) const override;

	void editorInit(SystemBlueprintData& temp) const override;

	bool editorScrollWheel(ShipBlueprint& ship, SystemBlueprintData& temp, float amount) const override;

	virtual std::vector<std::string> getInstructions() const;
private:
	unsigned int directions;
};

