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

#define width 10
#define height 10

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
SDL_Texture* CreateTexture(const char* filePath, SDL_Renderer* renderer);

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
	SDL_Texture* grassTEX = CreateTexture("Images/GRASS.png", renderer);

	//SAND
	SDL_Texture* sandTEX = CreateTexture("Images/SAND.png", renderer);

	//SEA
	SDL_Texture* seaTEX = CreateTexture("Images/SEA.png", renderer);



	//start of WFC

	srand(time(NULL));
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int count = 0;
    vector<char> allTypes = { 'L', 'C', 'S' };
    Grid* grid = new Grid(width, height , allTypes);

    while (count < (width * height))
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
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			SDL_Rect newRec{ x * (SDL_GetWindowSurface(window)->w / width), y * (SDL_GetWindowSurface(window)->h / height), SDL_GetWindowSurface(window)->w / width, SDL_GetWindowSurface(window)->h / height};
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
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
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


SDL_Texture* CreateTexture(const char* filePath, SDL_Renderer* renderer)
{
	SDL_Surface* IMG = IMG_Load(filePath);
	if (IMG == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return nullptr;
	}

	SDL_Texture* TEX = SDL_CreateTextureFromSurface(renderer, IMG);
	if (TEX == NULL) {
		std::cout << "Error creating texture";
		return nullptr;
	}

	SDL_FreeSurface(IMG);
	return TEX;
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
		if (tile->pos[1] + 1 < height)
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
		if (tile->pos[0] + 1 < width)
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
