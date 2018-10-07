#include "stdafx.h"
#include "ShipDesigner.h"
#include "SystemRegistry.h"
#include "Game.h"
#include <sstream>
#include <algorithm>

#define COLOR_LEGAL sf::Glsl::Vec4( 1.0f, 1.0f, 1.0f, 0.5f )
#define COLOR_ILLEGAL sf::Glsl::Vec4( 1.0f, 0.0f, 0.0f, 0.5f )

#define SCROLL_SPEED 512.f
#define SCROLL_EDGE_DISTANCE 50

#define RAD_DEG (float)(3.141592654 / 180)

#define between(x, a, b) (x > a && x < b)

using namespace Clockwork;

ShipDesigner::ShipDesigner()
{
	lighting.color = sf::Vector3f(1, 1, 1);
}


ShipDesigner::~ShipDesigner()
{
}

void ShipDesigner::setup(const sf::RenderTarget& target)
{

	world.setView(sf::View(sf::Vector2f(TILE_SIZE * MAX_SHIP_SIZE / 2, TILE_SIZE * MAX_SHIP_SIZE / 2), (sf::Vector2f)world.getSize()));

	silkscreen.loadFromFile("assets/fonts/slkscr.ttf");

	bgTexture.loadFromFile("assets/blueprint.png");
	bgTexture.setRepeated(true);
	bg.setTexture(bgTexture);
	bg.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(MAX_SHIP_SIZE * TILE_SIZE, MAX_SHIP_SIZE * TILE_SIZE)));

	toolbox = tgui::Panel::create();
	toolbox->setSize(250, world.getSize().y);

	systemSelect = tgui::ListBox::create();
	systemSelect->setPosition(0, 0);
	systemSelect->setSize(250, 200);
	for (auto& sysType : SystemRegistry::getAllSystems()) {
		systemSelect->addItem(sysType->getDisplayName(), sysType->sysID);
	}
	systemSelect->connect("ItemSelected", &ShipDesigner::selectType, this);
	toolbox->add(systemSelect);

	tgui::Label::Ptr lightLabel = tgui::Label::create("Light Direction");
	lightLabel->setPosition(5, world.getSize().y - 125.f);
	lightLabel->setTextSize(18);
	toolbox->add(lightLabel);

	lightKnob = tgui::Knob::create();
	lightKnob->setPosition(25, world.getSize().y - 100.f);
	lightKnob->setSize(90, 90);
	lightKnob->setValue(135);
	toolbox->add(lightKnob);

	lightSlider = tgui::Slider::create();
	lightSlider->setPosition(5, world.getSize().y - 100.f);
	lightSlider->setSize(5, 90);
	lightSlider->setMaximum(90);
	lightSlider->setValue(45);
	toolbox->add(lightSlider);

	gui.add(toolbox);

	cursorLabel = tgui::Label::create();
	cursorLabel->getRenderer()->setFont(silkscreen);
	cursorLabel->setTextSize(16);
	gui.add(cursorLabel);

	preview.loadFromFile("assets/editorPreview.frag", sf::Shader::Fragment);

	outline.loadFromFile("assets/outline.frag", sf::Shader::Fragment);
	outline.setUniform("outlineColor", sf::Glsl::Vec4(0, 0, 0, 1.0f));
	outline.setUniform("texBase", sf::Shader::CurrentTexture);
	outline.setUniform("threshold", 1.0f);

	spawnActor(&blueprint);
}

void ShipDesigner::tick(const sf::Time & timestep)
{
	sf::View worldView = world.getView();
	sf::Vector2i mouse = Game::getMousePosition();

	if (currentSystemType) {
		currentLegality = blueprint.canPlaceSystem(tempSystemData, legalityMessage);

		cursorLabel->setPosition(sf::Vector2f(mouse) + sf::Vector2f(12, 0));
		cursorLabel->setText(legalityMessage);
		cursorLabel->getRenderer()->setTextColor(currentLegality == LEGAL ? sf::Color::White : sf::Color::Red);
	}
	else {
		cursorLabel->setText("");
	}

	int left = toolbox->isVisible() ? toolbox->getSize().x : 0;
	if (mouse.x > left) {
		if (mouse.x < left + SCROLL_EDGE_DISTANCE) {
			float speed = SCROLL_SPEED * std::min(1.0f - (mouse.x - left) / (float)SCROLL_EDGE_DISTANCE, 1.0f);
			worldView.setCenter(sf::Vector2f(std::max(worldView.getCenter().x - speed * timestep.asSeconds(), (float)world.getSize().x / 2), worldView.getCenter().y));
		}
		else if (mouse.x > world.getSize().x - SCROLL_EDGE_DISTANCE) {
			float speed = SCROLL_SPEED * std::min(1.0f - ((int)world.getSize().x - mouse.x) / (float)SCROLL_EDGE_DISTANCE, 1.0f);
			worldView.setCenter(sf::Vector2f(std::min(worldView.getCenter().x + speed * timestep.asSeconds(), MAX_SHIP_SIZE * TILE_SIZE - ((float)world.getSize().x / 2)), worldView.getCenter().y));
		}

		if (mouse.y < SCROLL_EDGE_DISTANCE) {
			float speed = SCROLL_SPEED * std::min(1.0f - mouse.y / (float)SCROLL_EDGE_DISTANCE, 1.0f);
			worldView.setCenter(sf::Vector2f(worldView.getCenter().x, std::max(worldView.getCenter().y - speed * timestep.asSeconds(), (float)world.getSize().y / 2)));
		}
		else if (mouse.y > world.getSize().y - SCROLL_EDGE_DISTANCE) {
			float speed = SCROLL_SPEED * std::min(1.0f - (((int)world.getSize().y - mouse.y) / (float)SCROLL_EDGE_DISTANCE), 1.0f);
			worldView.setCenter(sf::Vector2f(worldView.getCenter().x, std::min(worldView.getCenter().y + speed * timestep.asSeconds(), MAX_SHIP_SIZE * TILE_SIZE - ((float)world.getSize().y / 2))));
		}
	}

	world.setView(worldView);

	float angle1 = ((float)lightKnob->getValue() - 90.f) * RAD_DEG;
	float angle2 = (90.f - (float)lightSlider->getValue()) * RAD_DEG;
	lighting.direction = sf::Vector3f(cos(angle2) * cos(angle1), cos(angle2) * -sin(angle1), -sin(angle2));
}

void ShipDesigner::drawWorld(sf::RenderTarget & target)
{
	target.draw(bg);

	if (currentSystemType) {
		preview.setUniform("previewColor", currentLegality == LEGAL ? COLOR_LEGAL : COLOR_ILLEGAL);
		currentSystemType->drawInterior(tempSystemData, blueprint, target, &preview);
	}
}

void ShipDesigner::handleEvent(sf::Event e)
{
	Scene::handleEvent(e);

	if (e.type == e.MouseButtonPressed) {
		sf::Vector2f worldMouse = world.mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
		sf::Vector2i tile(worldMouse.x / TILE_SIZE, worldMouse.y / TILE_SIZE);
		if (e.mouseButton.button == sf::Mouse::Left) {
			if (currentSystemType) {
				if(currentSystemType->editorMouseDown(blueprint, tempSystemData, tile)) blueprint.reformTextures();
			}
		}
		else if (e.mouseButton.button == sf::Mouse::Right) {
			if (blueprint.removeSystem(blueprint[tile])) blueprint.reformTextures();
		}
	}
	else if (e.type == e.MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f worldMouse = world.mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
		sf::Vector2i tile(worldMouse.x / TILE_SIZE, worldMouse.y / TILE_SIZE);
		if (currentSystemType) {
			if (currentSystemType->editorMouseUp(blueprint, tempSystemData, tile)) blueprint.reformTextures();
		}
	}
	else if (e.type == e.MouseMoved) {
		sf::Vector2f worldMouse = world.mapPixelToCoords(sf::Vector2i(e.mouseMove.x, e.mouseMove.y));
		sf::Vector2i tile(worldMouse.x / TILE_SIZE, worldMouse.y / TILE_SIZE);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			if (blueprint.removeSystem(blueprint[tile])) blueprint.reformTextures();
		}
		if (currentSystemType) {
			if (currentSystemType->editorMouseMove(blueprint, tempSystemData, tile, sf::Mouse::isButtonPressed(sf::Mouse::Left))) blueprint.reformTextures();
		}
	}
	else if (e.type == e.MouseWheelScrolled) {
		if (currentSystemType) {
			if (currentSystemType->editorScrollWheel(blueprint, tempSystemData, e.mouseWheelScroll.delta)) blueprint.reformTextures();
		}
	}
	else if (e.type == e.KeyPressed) {
		if (e.key.code == sf::Keyboard::Escape) {
			if (toolbox->isVisible()) toolbox->hideWithEffect(tgui::ShowAnimationType::SlideToLeft, sf::seconds(0.25f));
			else toolbox->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, sf::seconds(0.25f));
		}
		if (e.key.code == sf::Keyboard::Space) blueprint.drawInterior = !blueprint.drawInterior;
	}
}

void ShipDesigner::postProcess(sf::RenderTarget& target, const sf::Texture& texture)
{
	outline.setUniform("texSize", sf::Vector2i(texture.getSize()));
	target.draw(sf::Sprite(texture), &outline);
}

void ShipDesigner::selectType(std::string name, std::string sysID)
{
	currentSystemType = SystemRegistry::getSystemType(sysID);
	currentSystemType->editorInit(tempSystemData);
	gui.unfocusAllWidgets();
}
