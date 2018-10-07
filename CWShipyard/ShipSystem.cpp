#include "stdafx.h"
#include "ShipSystem.h"
#include "SystemRegistry.h"
#include <iostream>

sf::Texture ShipSystem::noEmissive;
sf::Shader ShipSystem::previewShader;

bool ShipSystem::hasInitialized = false;

ShipSystem::ShipSystem(std::string _sysID) : sysID(_sysID)
{
}

ShipSystem::~ShipSystem()
{
}

std::string ShipSystem::getDisplayName() const
{
	return "Unnamed";
}

void ShipSystem::editorInit(SystemBlueprintData & data) const
{
}

bool ShipSystem::editorMouseDown(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile) const
{
	return false;
}

bool ShipSystem::editorMouseMove(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile, bool drag) const
{
	return false;
}

bool ShipSystem::editorMouseUp(ShipBlueprint & ship, SystemBlueprintData & temp, sf::Vector2i tile) const
{
	return false;
}

bool ShipSystem::editorScrollWheel(ShipBlueprint & ship, SystemBlueprintData & temp, float amount) const
{
	return false;
}

bool ShipSystem::canPlace(const ShipBlueprint & ship, const SystemBlueprintData & data, std::string& failMessage) const
{
	return true;
}

void ShipSystem::drawExterior(const SystemBlueprintData & data, const ShipBlueprint& ship, sf::RenderTarget& base, sf::RenderTarget& normal, sf::RenderTarget& emissive) const
{
}

void ShipSystem::onSystemPlaced(ShipBlueprint & ship, SystemBlueprintData & data)
{
}

std::vector<std::string> ShipSystem::getInstructions() const
{
	return std::vector<std::string>();
}

void ShipSystem::initialize()
{
	if (hasInitialized) return;
	previewShader.loadFromFile("assets/editorPreview.frag", sf::Shader::Fragment);
	noEmissive.loadFromFile("assets/emissive.png");
	hasInitialized = true;
}

bool operator<(const ShipSystem & a, const ShipSystem & b)
{
	return a.getDisplayName() < b.getDisplayName();
}

bool operator==(const ShipSystem & a, const ShipSystem & b)
{
	return a.sysID == b.sysID;
}

bool compareSystems::operator()(const ShipSystem * a, const ShipSystem * b)
{
	return *a < *b;
}
