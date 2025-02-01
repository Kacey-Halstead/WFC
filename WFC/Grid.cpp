#include "Grid.h"

Grid::Grid(int x, int y, std::array<char, 3> types, std::array<SDL_Texture*, 3> textures)
{
	sizeX = x;
	sizeY = y;
	allTypes = types;
	tileTextures = textures;

	//for every cell, define possible tiles
	int index = 0;
	Tiles.reserve(sizeX);
	for (int x = 0; x < sizeX; x++)
	{
		std::vector<Tile> newTiles;
		newTiles.reserve(sizeY);
		for (int y = 0; y < sizeY; y++)
		{
			newTiles.emplace_back(SDL_Point{ x, y }, index);
			index++;
		}
		Tiles.push_back(newTiles);
	}
}

Grid::~Grid()
{

}

Tile* Grid::SmallestEntropy()
{
	//find cell with smallest number of options
	std::vector<Tile*> smallest;
	int numOptions = 3;

	for (int x = 0; x < sizeX; x++)
	{

		for (int y = 0; y < sizeY; y++)
		{
			if (Tiles[x][y].typesAndWeights.size() < numOptions && Tiles[x][y].type == '0')
			{
				smallest.clear();
				smallest.push_back(&Tiles[x][y]);
				numOptions = Tiles[x][y].typesAndWeights.size();
			}
			else if (Tiles[x][y].typesAndWeights.size() == numOptions && Tiles[x][y].type == '0')
			{
				smallest.push_back(&Tiles[x][y]);
			}
		}
	}

	if (smallest.size() == 1)
	{
		return smallest[0];
	}
	else if (smallest.size() > 1)
	{
		int random = rand() % (smallest.size());
		return smallest[random];
	}
	else
	{
		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				if (Tiles[x][y].type == '0')
				{
					return &Tiles[x][y];
				}
			}
		}
	}

	return nullptr;
}

void Grid::RenderGrid(SDL_Renderer* renderer)
{
	int counter = 0;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			SDL_Rect destRect{ x * (900 / sizeX), y * (900 / sizeY), (900 / sizeX), (900 / sizeY) };

			switch (Tiles[x][y].type)
			{
			case 'S':
				SDL_RenderCopy(renderer, tileTextures[2], NULL, &destRect);
				break;
			case 'L':
				SDL_RenderCopy(renderer, tileTextures[0], NULL, &destRect);
				break;
			case 'C':
				SDL_RenderCopy(renderer, tileTextures[1], NULL, &destRect);
				break;
			default:
				break;
			}

			counter++;
		}
	}
}