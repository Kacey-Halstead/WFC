#pragma once
#include "Tile.h"

class Grid
{
public:
	int sizeX;
	int sizeY;
	std::vector<std::vector<Tile>> Tiles;
	std::array<char, 3> allTypes;

	Grid(int x, int y, std::array<char, 3> types, std::array<SDL_Texture*, 3> textures);
	~Grid();

	void RenderGrid(SDL_Renderer* renderer);

	Tile* SmallestEntropy();

private:
	std::array<SDL_Texture*, 3> tileTextures;
};

