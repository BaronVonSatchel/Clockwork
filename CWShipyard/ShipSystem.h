#pragma once
#include <vector>
#include <SFML/System.hpp>
#include "TextureSet.h"
#include "SystemBlueprintData.h"

#define TILE_SIZE 16

class ShipBlueprint;

struct compareSystems {
	bool operator()(const ShipSystem* a, const ShipSystem* b);
};

///<summary>Defines a type of system that can be placed on a blueprint.</summary>
class ShipSystem abstract
{
public:
	ShipSystem(std::string _sysID);
	~ShipSystem();

	///<summary>Gets the width (X dimension) of this system</summary>
	virtual int getWidth(const SystemBlueprintData&) const = 0;

	///<summary>Gets the length (Y dimension) of this system</summary>
	virtual int getLength(const SystemBlueprintData&) const = 0;

	///<summary>Returns the human-readable name of this system.</summary>
	virtual std::string getDisplayName() const;

	///<summary>Called when this system type is first selected in the editor.</summary>
	virtual void editorInit(SystemBlueprintData& data) const;

	///<summary>Called when the left mouse button is pressed in the editor.
	///Returns true if something was changed that requires recalculating
	///the blueprint's stats and textures (such as placing a system)</summary>
	virtual bool editorMouseDown(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile) const;

	///<summary>Called when the mouse is moved while the left button is pressed in the editor.
	///Returns true if something was changed that requires recalculating
	///the blueprint's stats and textures (such as placing a system)</summary>
	virtual bool editorMouseMove(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile, bool drag) const;

	///<summary>Called when the left mouse button is released in the editor.
	///Returns true if something was changed that requires recalculating
	///the blueprint's stats and textures (such as placing a system)</summary>
	virtual bool editorMouseUp(ShipBlueprint& ship, SystemBlueprintData& temp, sf::Vector2i tile) const;

	///<summary>Called when the scroll wheel is moved in the editor.
	///Returns true if something was changed that requires recalculating
	///the blueprint's stats and textures (such as placing a system)</summary>
	virtual bool editorScrollWheel(ShipBlueprint& ship, SystemBlueprintData& temp, float amount) const;

	///<summary>Allows the system to assert additional placement rules on top of the basic adjacency and non-overlap rules. Return true if the system can be placed.</summary>
	virtual bool canPlace(const ShipBlueprint& ship, const SystemBlueprintData& data, std::string& message) const;

	///<summary>Draw the interior of the system to the supplied RenderTarget</summary>
	virtual void drawInterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const = 0;

	///<summary>Draw the exterior of the system to the supplied RenderTarget (will be drawn on top of the default exterior)</summary>
	virtual void drawExterior(const SystemBlueprintData& data, const ShipBlueprint& ship, sf::RenderTarget& base, sf::RenderTarget& normal, sf::RenderTarget& emissive) const;

	///<summary>Called after the system is successfully placed to perform any additional setup</summary>
	virtual void onSystemPlaced(ShipBlueprint& ship, SystemBlueprintData& data);

	virtual std::vector<std::string> getInstructions() const;

	const std::string sysID;

	///<summary>Called once when the game is started to perform global setup. No effect when called subsequently.</summary>
	static void initialize();

protected:
	static sf::Shader previewShader;

	static sf::Texture noEmissive;
private:

	static bool hasInitialized;
};

///<summary>Compares two systems by their display names. Useful to sort alphabetically.</summary>
bool operator<(const ShipSystem& a, const ShipSystem& b);

///<summary>Compares two systems by their system ID (which is unique as long as they are registered).</summary>
bool operator==(const ShipSystem& a, const ShipSystem& b);