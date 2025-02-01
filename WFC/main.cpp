#include <iostream>
#include<time.h>
#include <SDL_image.h>

#include "Grid.h"
#include "SDL_Window.h"

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

int main(int argc, char* argv[])
{
	srand(time(NULL));

	SDL_Window* SDLWindow = new SDL_Window();

	std::array<SDL_Texture*, 3> textures = {

		//GRASS
		CreateTexture("Images/GRASS.png", SDLWindow->getRenderer()),

		//SAND
		CreateTexture("Images/SAND.png", SDLWindow->getRenderer()),

		//SEA
		CreateTexture("Images/SEA.png", SDLWindow->getRenderer()) 
	};

	Grid* grid = new Grid(30, 30, {'L', 'C', 'S'}, textures);
	WFC::WFCBody(grid);

	while (true)
	{
		if (!SDLWindow->Events()) break;

		SDLWindow->BeginRender();

		grid->RenderGrid(SDLWindow->getRenderer());

		SDLWindow->EndRender();
	}

	delete grid;
	delete SDLWindow;
	SDL_DestroyTexture(textures[0]);
	SDL_DestroyTexture(textures[1]);
	SDL_DestroyTexture(textures[2]);
	return 0;
}

