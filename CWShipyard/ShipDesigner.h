#pragma once
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "ShipBlueprint.h"

class ShipDesigner : public Clockwork::Scene
{
public:
	ShipDesigner();
	~ShipDesigner();

	void setup(const sf::RenderTarget& target) override;

	void tick(const sf::Time& timestep) override;

	void drawWorld(sf::RenderTarget& target) override;

	void handleEvent(sf::Event) override;

	void postProcess(sf::RenderTarget& target, const sf::Texture& texture) override;
private:
	void selectType(std::string name, std::string sysID);

	tgui::Panel::Ptr toolbox;
	tgui::ListBox::Ptr systemSelect;
	tgui::Knob::Ptr lightKnob;
	tgui::Slider::Ptr lightSlider;

	tgui::Label::Ptr cursorLabel;

	sf::Sprite bg;
	sf::Texture bgTexture;

	ShipBlueprint blueprint;

	const ShipSystem* currentSystemType;

	SystemBlueprintData tempSystemData;

	sf::Shader preview, outline;
	sf::Font silkscreen;

	bool modeInterior = true;

	PlacementLegality currentLegality;
	std::string legalityMessage;
};

