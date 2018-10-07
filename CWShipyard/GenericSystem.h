#pragma once
#include "ShipSystem.h"

///<summary>A simple system with fixed stats and dimensions and no special placement rules</summary>
class GenericSystem :
	public ShipSystem
{
public:
	GenericSystem(std::string _sysID, std::string _displayName, sf::Vector2i _dimensions);
	~GenericSystem();

	int getWidth(const SystemBlueprintData&) const override;

	int getLength(const SystemBlueprintData&) const override;

	virtual std::string getDisplayName() const override;

	virtual void drawInterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const override;

	virtual void drawExterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& base, sf::RenderTarget& normal, sf::RenderTarget& emissive) const override;

	virtual void editorInit(SystemBlueprintData& temp) const override;

	virtual bool editorMouseDown(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile) const override;

	virtual bool editorMouseMove(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile, bool drag) const override;

	virtual std::vector<std::string> getInstructions() const override;

	///<summary> Sets this system to use a custom exterior texture instead of the default
	///<param name = 'useEmissive'>If true, load a custom emissive texture.</param></summary>
	void setUseExterior(bool useEmissive = false);
protected:
	sf::Vector2i dimensions;
	sf::Texture interiorTexture;

	sf::Texture exteriorBase;
	sf::Texture exteriorNormal;
	sf::Texture exteriorEmissive;

	std::string displayName;

	bool shouldDrawExterior = false;
};

