#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<algorithm>
#include <windows.h>
#include "Grid.h"
#include <SDL.h>
#include <SDL_image.h>



int sizeX = 10;
int sizeY = 10;

using namespace std;

enum directions
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

void Evaluate(Grid* grid, Tile* tile, directions dir);
void ResetNeighbours(vector<Tile*> tiles);

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) //initialise SDL
	{
		std::cout << "Unable to initialise SDL: " << SDL_GetError();
		return 1;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return 2;
	}

	//Create window
	SDL_Window* window = SDL_CreateWindow("WFC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL); //name, horizontal pos, vertical pos, width, height, mode
	if (window == NULL) {
		std::cout << "Error window creation";
		return 3;
	}

	//Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //window to render to, index of rendering driver (-1 = first available), mode
	if (renderer == NULL) {
		std::cout << "Error renderer creation";
		return 4;
	}

	//GRASS

	//Load image
	SDL_Surface* grassIMG = IMG_Load("Images/GRASS.png");
	if (grassIMG == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return 5;
	}

	//Create texture
	SDL_Texture* grassTEX = SDL_CreateTextureFromSurface(renderer, grassIMG); //renderer, surface
	if (grassTEX == NULL) {
		std::cout << "Error creating texture";
		return 6;
	}

	SDL_FreeSurface(grassIMG); //do not need surface anymore. free = free memory

	//SAND

	SDL_Surface* sandIMG = IMG_Load("Images/SAND.png");
	if (sandIMG == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return 5;
	}

	SDL_Texture* sandTEX = SDL_CreateTextureFromSurface(renderer, sandIMG);
	if (sandTEX == NULL) {
		std::cout << "Error creating texture";
		return 6;
	}

	SDL_FreeSurface(sandIMG);

	//SEA
	SDL_Surface* seaIMG = IMG_Load("Images/SEA.png");
	if (seaIMG == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return 5;
	}

	SDL_Texture* seaTEX = SDL_CreateTextureFromSurface(renderer, seaIMG);
	if (seaTEX == NULL) {
		std::cout << "Error creating texture";
		return 6;
	}

	SDL_FreeSurface(seaIMG);



	//start of WFC

	srand(time(NULL));
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int count = 0;
    vector<char> allTypes = { 'L', 'C', 'S' };
    Grid* grid = new Grid(sizeX, sizeY , allTypes);

    while (count < (sizeX * sizeY))
    {
        Tile* selectedTile = grid->SmallestEntropy();

        //assign cell to type at random
        int randomType = rand() % selectedTile->availableTypes.size();
        selectedTile->SetType(selectedTile->availableTypes[randomType]);
		selectedTile->availableTypes.clear();

        //update all neighbours(up, down, left, right)
        Evaluate(grid, selectedTile, UP);
		Evaluate(grid, selectedTile, DOWN);
		Evaluate(grid, selectedTile, LEFT);
		Evaluate(grid, selectedTile, RIGHT);

        count++;
    }

	std::cout << endl;

	//stores all rects to render tiles to
	vector<SDL_Rect> rects;
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			SDL_Rect newRec{ x * (SDL_GetWindowSurface(window)->w / sizeX), y * (SDL_GetWindowSurface(window)->h / sizeY), SDL_GetWindowSurface(window)->w / sizeX, SDL_GetWindowSurface(window)->h / sizeY};
			rects.push_back(newRec);
		}
	}

	while (true)
	{
		SDL_Event e; 
		if (SDL_PollEvent(&e)) //ends loop if exit button pressed
		{ 
			if (e.type == SDL_QUIT)
			{
				break;
			}
		}

		SDL_RenderClear(renderer); //remove anything already rendered
		int counter = 0;
		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				switch (grid->Tiles[x][y]->type)
				{
				case 'S':
					SDL_RenderCopy(renderer, seaTEX, NULL, &rects[counter]);
					break;
				case 'L':
					SDL_RenderCopy(renderer, grassTEX, NULL, &rects[counter]);
					break;
				case 'C':
					SDL_RenderCopy(renderer, sandTEX, NULL, &rects[counter]);
					break;
				default:
					break;
				}
				counter++;
			}
			std::cout << endl;
		}
		SDL_RenderPresent(renderer); //moving from back buffer to screen
	}

	SDL_DestroyTexture(grassTEX);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}

void Evaluate(Grid* grid, Tile* tile, directions dir)
{
	Tile* neighbour = nullptr;

	int x_index = tile->pos[0];
	int y_index = tile->pos[1];

	switch (dir)
	{
	case UP:
		if (tile->pos[1] - 1 >= 0)
		{
			neighbour = grid->Tiles[x_index][y_index - 1]; //up

		}
		break;
	case DOWN:
		if (tile->pos[1] + 1 < sizeY)
		{
			neighbour = grid->Tiles[x_index][y_index + 1]; //down
		}
		break;
	case LEFT:
		if (tile->pos[0] - 1 >= 0)
		{

			neighbour = grid->Tiles[x_index - 1][y_index]; //left
		}
		break;
	case RIGHT:
		if (tile->pos[0] + 1 < sizeX)
		{

			neighbour = grid->Tiles[x_index + 1][y_index]; //right
		}
		break;
	default:
		break;
	}


	if (neighbour == nullptr) return;
	

	char toRemove;

	switch (tile->type)
	{
	case 'L':
		toRemove = 'S';
		break;

	case 'S':
		toRemove = 'L';
		break;
	default:
		return;
	}

	//remove option from neighbours available tiles list
	auto it = find(neighbour->availableTypes.begin(), neighbour->availableTypes.end(), toRemove);
	if (it != neighbour->availableTypes.end() && neighbour->type == '0')
	{
		neighbour->availableTypes.erase(it);

	}

	//if neighbour has no options left, clear that tile and its neighbours
	if (neighbour->availableTypes.size() == 0 && neighbour->type == '0')
	{
		vector<Tile*> toReset;
		toReset.push_back(neighbour);
		toReset.push_back(grid->Tiles[tile->pos[0]][tile->pos[1] - 1]); //up
		toReset.push_back(grid->Tiles[tile->pos[0]][tile->pos[1] + 1]); //down
		toReset.push_back(grid->Tiles[tile->pos[0] - 1][tile->pos[1]]); //left
		toReset.push_back(grid->Tiles[tile->pos[0] + 1][tile->pos[1]]); //right

		ResetNeighbours(toReset);
	}

    
}

void ResetNeighbours(vector<Tile*> tiles)
{
    for (Tile* t : tiles)
    {
        t->Reset();
    }
}
