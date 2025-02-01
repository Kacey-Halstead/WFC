#pragma once
#include <vector>
#include <SDL.h>
#include <array>

#include "Grid.h"
#include "Tile.h"

enum directions
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

namespace WFC
{
	std::vector<char> GetTypeAndRules(char input, directions dir);

	bool IsInGrid(const SDL_Point& pos, const SDL_Point& offset);
	bool EveryTileHasType();

	void WFCReset();
	void WFCBody(Grid* grid);
	void Evaluate(Tile* tile, directions dir);
	void ChangeTileWeighting(Tile* tile);
	void FindAndErase(Tile* tile, char toFind);
	void CheckForEmptyTiles(Tile* tile);
	void TypeIncrement(char typeToIncrement);

	SDL_Point operator *(const SDL_Point& a, const int& b);

};

