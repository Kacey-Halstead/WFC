#pragma once
#include <vector>
#include <array>
#include <SDL.h>

class Tile
{
public:
	char type = '0';

	std::vector<std::pair<char, float>> typesAndWeights;

	std::array<char, 3> allTypes = { 'L', 'C', 'S' };

	int index = 0;

	SDL_Point pos = {};

	Tile(SDL_Point position, int tileIndex);
	void UpdateTypeandWeight(char c, float weightChange);
	~Tile();
	void Reset();

private:

};

