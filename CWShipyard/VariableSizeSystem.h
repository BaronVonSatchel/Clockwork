#pragma once
#include "ShipSystem.h"

#define V_SIZE_X "sizeX"
#define V_SIZE_Y "sizeY"
#define V_TEMP_X "tempX"
#define V_TEMP_Y "tempY"

///<summary>A system which can be placed in a rectangle of any size by
///dragging in the editor, and whose stats scale its total area.</summary>
class VariableSizeSystem :
	public ShipSystem
{
public:
	VariableSizeSystem(std::string _sysID, std::string _displayName);
	~VariableSizeSystem();

	std::string getDisplayName() const override;

	int getWidth(const SystemBlueprintData&) const override;
	int getLength(const SystemBlueprintData&) const override;

	virtual void drawInterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const override;

	virtual void editorInit(SystemBlueprintData& temp) const override;

	virtual bool editorMouseDown(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile) const override;

	virtual bool editorMouseUp(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile) const override;

	virtual bool editorMouseMove(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile, bool drag) const override;

	virtual std::vector<std::string> getInstructions() const;
protected:
	const sf::Texture& getInteriorTexture() const;
	
private:
	sf::Texture interiorTexture;

	std::string displayName;

	void updatePreviewSize(SystemBlueprintData& temp, sf::Vector2i tile) const;
};

