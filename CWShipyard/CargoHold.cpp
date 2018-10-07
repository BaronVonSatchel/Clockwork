#include "stdafx.h"
#include "CargoHold.h"
#include "ShipBlueprint.h"

using namespace sf;

CargoHold::CargoHold(std::string _sysID, std::string _displayName) : VariableSizeSystem(_sysID, _displayName)
{
	exteriorTextures.base.loadFromFile("assets/system/" + sysID + "/exb.png");
	exteriorTextures.normal.loadFromFile("assets/system/" + sysID + "/exn.png");
}


CargoHold::~CargoHold()
{
}

void CargoHold::drawInterior(const SystemBlueprintData & data, const ShipBlueprint& ship, RenderTarget & target, const RenderStates & states) const
{
	Sprite s1;
	s1.setTexture(getInteriorTexture());
	Vector2i dimensions(data.intVars.at(V_SIZE_X), data.intVars.at(V_SIZE_Y));

	if (dimensions.y < 2) {
		//left
		s1.setPosition((Vector2f)(TILE_SIZE * data.position));
		s1.setTextureRect({ ship[data.position + Vector2i(-1, 0)] >= 0 ? 8 : 0, 0, 8, 16 });
		target.draw(s1, states);

		//right
		s1.setPosition(Vector2f(TILE_SIZE * (data.position.x + dimensions.x - 0.5f), TILE_SIZE * data.position.y));
		s1.setTextureRect({ ship[data.position + Vector2i(dimensions.x, 0)] >= 0 ? 32 : 40, 0, 8, 16 });
		target.draw(s1, states);

		//middle
		s1.setTextureRect({ 16, 0, 16, 16 });
		for (int i = 0; i < dimensions.x - 1; i++) {
			s1.setPosition(Vector2f(TILE_SIZE * (data.position.x + i + 0.5f), TILE_SIZE * data.position.y));
			target.draw(s1, states);
		}
	}
	else {
		//top left corner
		s1.setPosition((Vector2f)(TILE_SIZE * data.position));
		s1.setTextureRect({ ship[data.position + Vector2i(-1, 0)] >= 0 ? 8 : 0, 16, 8, 16 });
		target.draw(s1, states);

		//top right corner
		s1.setPosition(Vector2f(TILE_SIZE * (-0.5f + data.position.x + dimensions.x), TILE_SIZE * data.position.y));
		s1.setTextureRect({ ship[data.position + Vector2i(dimensions.x, 0)] >= 0 ? 32 : 40, 16, 8, 16 });
		target.draw(s1, states);

		//bottom left corner
		s1.setPosition(Vector2f(TILE_SIZE * data.position.x, TILE_SIZE * (data.position.y + dimensions.y - 1)));
		s1.setTextureRect({ ship[data.position + Vector2i(-1, dimensions.y - 1)] >= 0 ? 8 : 0, 48, 8, 16 });
		target.draw(s1, states);

		//bottom right corner
		s1.setPosition(Vector2f((data.position + dimensions) * TILE_SIZE) - Vector2f(TILE_SIZE / 2, TILE_SIZE));
		s1.setTextureRect({ ship[data.position + Vector2i(dimensions.x, dimensions.y - 1)] >= 0 ? 32 : 40, 48, 8, 16 });
		target.draw(s1, states);

		//top and bottom edges
		Sprite s2(getInteriorTexture());
		s2.setTexture(getInteriorTexture());

		s1.setTextureRect({ 16, 16, 16, 16 });
		s2.setTextureRect({ 16, 48, 16, 16 });
		for (int i = 0; i < dimensions.x - 1; i++) {
			s1.setPosition((float)TILE_SIZE * Vector2f(0.5f + data.position.x + i, data.position.y));
			target.draw(s1, states);

			s2.setPosition((float)TILE_SIZE * Vector2f(0.5f + data.position.x + i, data.position.y + dimensions.y - 1));
			target.draw(s2, states);
		}

		//left and right edges
		for (int j = 1; j < dimensions.y - 1; j++) {
			s1.setPosition((float)TILE_SIZE * Vector2f(data.position.x, data.position.y + j));
			s1.setTextureRect({ ship[data.position + Vector2i(-1, j)] >= 0 ? 8 : 0, 32, 8, 16 });
			target.draw(s1, states);

			s2.setPosition((float)TILE_SIZE * Vector2f(-0.5f + data.position.x + dimensions.x, data.position.y + j));
			s2.setTextureRect({ ship[data.position + Vector2i(dimensions.x, j)] >= 0 ? 32 : 40, 32, 8, 16 });
			target.draw(s2, states);
		}

		//center
		s1.setTextureRect({ 16, 32, 16, 16 });
		for (int j = 1; j < dimensions.y - 1; j++) {
			for (int i = 0; i < dimensions.x - 1; i++) {
				s1.setPosition((float)TILE_SIZE * sf::Vector2f(0.5f + data.position.x + i, data.position.y + j));
				target.draw(s1, states);
			}
		}
	}
}

void CargoHold::drawExterior(const SystemBlueprintData & data, const ShipBlueprint & ship, sf::RenderTarget & base, sf::RenderTarget & normal, sf::RenderTarget & emissive) const
{
	Sprite sb(exteriorTextures.base);
	Sprite sn(exteriorTextures.normal);

	int w = data.intVars.at(V_SIZE_X);
	for (int j = 0; j < data.intVars.at(V_SIZE_Y); j++) {
		//left
		bool test = ship[data.position + Vector2i(-1, j)] >= 0;
		sb.setPosition((float)TILE_SIZE * Vector2f(data.position.x, data.position.y + j));
		sb.setTextureRect({ test ? 8 : 0, 0, 8, 16 });
		base.draw(sb);
		sn.setPosition((float)TILE_SIZE * Vector2f(data.position.x, data.position.y + j));
		sn.setTextureRect({ test ? 8 : 0, 0, 8, 16 });
		normal.draw(sn);

		//right
		test = ship[data.position + Vector2i(w, j)] >= 0;
		sb.setPosition((float)TILE_SIZE * Vector2f(-0.5f + data.position.x + w, data.position.y + j));
		sb.setTextureRect({ test ? 32 : 40, 0, 8, 16 });
		base.draw(sb);
		sn.setPosition((float)TILE_SIZE * Vector2f(-0.5f + data.position.x + w, data.position.y + j));
		sn.setTextureRect({ test ? 32 : 40, 0, 8, 16 });
		normal.draw(sn);

		//middle
		sb.setPosition((float)TILE_SIZE * Vector2f(0.5f + data.position.x, data.position.y + j));
		sb.setTextureRect({ 16, 0, 16, 16 });
		sb.setScale(w - 1.0f, 1.0f);
		base.draw(sb);
		sn.setPosition((float)TILE_SIZE * Vector2f(0.5f + data.position.x, data.position.y + j));
		sn.setTextureRect({ 16, 0, 16, 16 });
		sn.setScale(w - 1.0f, 1.0f);
		normal.draw(sn);

		sb.setScale(1.0f, 1.0f);
		sn.setScale(1.0f, 1.0f);
	}
}
