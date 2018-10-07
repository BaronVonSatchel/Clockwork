#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <set>
#include "Actor.h"
#include "SystemBlueprintData.h"
#include "TextureSet.h"

#define MAX_SHIP_SIZE 256u

sf::Glsl::Vec4 colorToGlVec4(const sf::Color& color);

enum PlacementLegality {
	LEGAL, ILLEGAL_OUT_OF_BOUNDS, ILLEGAL_OVERLAP, ILLEGAL_NOT_ADJACENT, ILLEGAL_SPECIAL_RULES
};

class ShipBlueprint : public Clockwork::Actor
{
public:
	ShipBlueprint();
	~ShipBlueprint();

	virtual void drawActor(sf::RenderTarget& target, sf::RenderStates states) const override;

	///<summary>Attempt to add a system to the blueprint following all placement rules. Returns true if successful.</summary>
	bool tryPlaceSystem(const SystemBlueprintData&);

	///<summary>Attempt to remove a system from the blueprint. Returns true if successful.</summary>
	bool removeSystem(int id);

	///<summary>Get a set of all systems in the blueprint.</summary>
	std::set<SystemBlueprintData> getSystems() const;

	///<summary>Returns the number of systems in the blueprint.</summary>
	unsigned int systemCount() const;

	///<summary>Gets the system, if any, at the specified tile coordinates.</summary>
	const SystemBlueprintData operator[](sf::Vector2i) const;

	///<summary>Determines whether the given system can be added to the blueprint. Outputs an additional message to explain the reason.</summary>
	PlacementLegality canPlaceSystem(const SystemBlueprintData& data, std::string& message) const;

	///<summary>Determines whether the given system can be added to the blueprint.</summary>
	PlacementLegality canPlaceSystem(const SystemBlueprintData& data) const;

	///<summary>Regenerate the blueprint's stiched interior and exterior textures from individual systems.
	///This is a costly operation so it should only be called when changes are made to the blueprint.</summary>
	void reformTextures();

	const sf::Texture& getStitchedInterior() const;
	const sf::Texture& getStitchedExteriorBase() const;
	const sf::Texture& getStitchedExteriorNormal() const;
	const sf::Texture& getStitchedExteriorEmissive() const;

	bool drawInterior;
private:
	std::unordered_map<int, SystemBlueprintData> systems;

	int coordinateToSystemMap[MAX_SHIP_SIZE][MAX_SHIP_SIZE];

	sf::RenderTexture temp;
	sf::RenderTexture stitchedInterior;
	sf::RenderTexture stitchedExteriorBase;
	sf::RenderTexture stitchedExteriorNormal;
	sf::RenderTexture stitchedExteriorEmissive;

	sf::Shader bHeight, bNorm, paint;
};

